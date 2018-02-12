#pragma once

#include "variant/Variant.h"
#include "Array.h"

#define DEBUG 1

#define GC GarbageCollector::singleton

//#if DEBUG
//static void* operator new(std::size_t sz, bool b) {
//	std::cout << "custom placement new called, b = " << b << '\n';
//	return ::operator new(sz);
//	}
//#endif

class GarbageCollector
{
public:
	void queue_clean(const Variant &p_var);
	void queue_free(const Variant &p_var);

	void free();
	void clean();

	static void init();

	static GarbageCollector *singleton;

private:
	Array<Variant> clean_queue, free_queue;
};

