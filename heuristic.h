#ifndef heuristic_h_
#define heuristic_h_

#include "cube.h"

#include "word.h"

class Heuristic {
public:
	virtual ~Heuristic() {};

	virtual int eval(const Cube &) const = 0;
	inline int operator () (const Cube &cube) const {
		return eval(cube);
	}

	void set_metric(int);
	int get_metric() const;

private:
	int metric = METRIC_DEFAULT;
};

class DumbHeuristic : public Heuristic {
public:
	int eval(const Cube &) const;
};

class HashTableHeuristic : public Heuristic {
public:
	HashTableHeuristic() = delete;
	HashTableHeuristic(const HashTableHeuristic&) = delete;

	HashTableHeuristic(const char *name, unsigned int(*hash_func)(const Cube &), int depth, int bits, bool force_recompute = false);
	~HashTableHeuristic();

	int eval(const Cube &) const;

	size_t get_table_size() const;
	int get_table_entry(size_t i) const;

private:
	size_t table_size;
	unsigned int(*hash_func)(const Cube &) = nullptr;
	signed char *table = nullptr;
};

#endif // heuristic_h_
