#include "heuristic.h"

#include "word.h"

#include <fstream>
#include <iomanip>
#include <functional>
#include <vector>

int DumbHeuristic::eval(const Cube &) const {
	return 1;
}

HashTableHeuristic::HashTableHeuristic(const char *name, unsigned int(*hash_func)(const Cube &), int depth, int bits, bool force_recompute)
	: hash_func(hash_func)
{
	// allocate space
	table_size = 1 << bits;
	table = new signed char[table_size];

	// name of the text file where the results are stored
	char filename[0x100];
	sprintf(filename, "table_%s_%d_%d.txt", name, depth, bits);
	std::fstream fs;
	if (!force_recompute) {
		fs.open(filename, std::ios_base::in);
	}
	if (fs.is_open()) {
		// load table
		printf("File '%s' found. Loading...", filename);
		fflush(stdout);
		for (unsigned int i = 0; i < table_size; ++i) {
			int val;
			char c;
			fs >> val;
			fs >> c;
			table[i] = val;
		}
		printf(" Done.\n");
	}
	else {
		printf("File '%s' not found. Generating...", filename);
		fflush(stdout);
		// generate table
		for (unsigned int i = 0; i < table_size; ++i) {
			table[i] = depth + 1;
		}

		// depth search
		struct SearchState {
			Cube cube;
			int last_turn;
			int index;
			int cost;
		};
		std::vector<SearchState> stack;
		stack.resize(30);
		int stack_head = 0;
		stack[stack_head] = SearchState{ Cube::TURN_IDENTITY, -1, 0, 0 };

		// loop through search space
		while (stack_head >= 0) {
			if (stack[stack_head].index >= WORD_NUM) {
				unsigned int hash = hash_func(stack[stack_head].cube) % table_size;
				if (hash == 0x23a || hash == 0x4ba || hash == 0xeeb) {
					//printf("%08x: %d\n", hash, stack[stack_head].cost);
				}
				auto &entry = table[hash];
				entry = (signed char) std::min((int) entry, stack[stack_head].cost);
				stack_head--;
			}
			else {
				int next_index = stack[stack_head].index++;
				int next_cost = stack[stack_head].cost + get_word_cost(next_index, METRIC_QUARTER_TURN);
				// termination criterion
				if (next_cost > depth)
					continue;
				// skip nonsensical turns
				if (can_words_be_switched(next_index, stack[stack_head].last_turn))
					continue;
				// recursion
				Cube next_cube = stack[stack_head].cube + get_word_cube(next_index);
				stack[++stack_head] = SearchState{ next_cube, next_index, 0, next_cost };
			}
		}
		printf(" Done.\n");

		// store table
		printf("Storing '%s' ...", filename);
		fflush(stdout);
		fs.clear();
		fs.open(filename, std::ios::out);
		if (fs.is_open()) {
			for (unsigned int i = 0; i < table_size; ++i) {
				fs << (int) table[i] << ", ";
				if ((i + 1) % 20 == 0)
					fs << '\n';
			}
		}
		printf(" Done.\n");
	}
}

HashTableHeuristic::~HashTableHeuristic() {
	if (table)
		delete[] table;
}

int HashTableHeuristic::eval(const Cube &cube) const {
	unsigned int hash = hash_func(cube);
	return table[hash % table_size];
}

size_t HashTableHeuristic::get_table_size() const {
	return table_size;
}

int HashTableHeuristic::get_table_entry(size_t i) const {
	return table[i];
}
