#include "memory.h"

#include "tmessage.h"

GarbageCollector* GarbageCollector::singleton;

void GarbageCollector::queue_clean(const Variant& p_var) {
    clean_queue.push_back(p_var);
}

void GarbageCollector::queue_free(const Variant& p_var) {
    if (!free_queue.contains(p_var)) free_queue.push_back(p_var);
}

void GarbageCollector::free() {
    for (int c = 0; c < free_queue.size(); c++) free_queue[c].free();

    free_queue.clear();
}

void GarbageCollector::clean() {
    for (int c = 0; c < clean_queue.size(); c++) clean_queue[c].clean();

    clean_queue.clear();
}
void GarbageCollector::init() { singleton = new GarbageCollector; }
