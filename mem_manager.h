#pragma once

#include <stack>
#include <vector>
#include <priority_queue>
#include "dl.h"
#include "memseg.h"
#include "my_heap.h"

struct MemoryManager {
    size_t total_mem; // Кол-во ячеек памяти
    dlList segList; // Список всех отрезков
    std::stack<dlNode*> cache;
    my_heap heap;
    std::vector<dlNode*> place;

    MemoryManager(size_t total_mem) {
        this->total_mem = total_mem;
        place = std::vector<dlNode*>(total_mem, nullptr);
        memory_segment* init_seg = new memory_segment(0, total_mem);

        segList.push_front(init_seg);
        cache.push(segList.head);
    }

    memory_segment* malloc(size_t mem) {
        if (mem == 0) {
            return nullptr;
        }

        dlNode* tmp = cache.top();
        while (tmp->segment->status == segment_status::IS_DELETED) {
            delete tmp->segment;
            delete tmp;
            cache.pop();
            tmp = cache.top();
        }

        if (tmp->segment->length < mem) {
            return nullptr;
        }

        std::pair<dlNode*, dlNode*> alloc_free = segList.split_segment(tmp, mem);
        if (alloc_free.second != nullptr) {
            cache.push(alloc_free.second);
        }
        place[calculate_place(alloc_free.first)] = alloc_free.first;
        return alloc_free.first->segment;
    }

    memory_segment* free(size_t i) {
        if (place[i] == nullptr || i >= total_mem) {
            return nullptr;
        }
        dlNode* node = place[i];
        place[i] = nullptr;
        dlNode* newNode = segList.join_segments(node);
        cache.push(newNode);
        return newNode->segment;
    }

    void free_cache() {
        dlNode* tmp = cache.top();
        while (!cache.empty()) {
            if (tmp->segment->status == segment_status::IS_DELETED) {
                delete tmp->segment;
                delete tmp;
            }
            cache.pop();
        }
    }

    void free_list() {
        segList.free_node(segList.head);
    }

private:
    size_t calculate_place(dlNode* node) {
        dlNode* iterator = segList.head;
        size_t counter = 0;
        while (iterator != node) {
            counter += iterator->segment->length;
            iterator = iterator->next;
        }
        return counter;
    }
};