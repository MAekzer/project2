#pragma once

#include <vector>
#include <iostream>
#include <bits/stdc++.h>
#include "memseg.h"
#include "dl.h"

using std::vector;

struct my_heap
{
    private:
        vector<dlNode*> heap;

    public:
        dlNode* front() {
            if (heap.empty()) {
                return nullptr;
            }
            return heap[0];
        }

        void push() {
            heap.push_back();
            std::push_heap(heap.begin(), heap.end());
        }

        void pop() {
            if (heap.empty()) {
                return;
            }

            std::pop_heap(heap.begin(), heap.end());
            heap.pop_back();
        }

        my_heap() {
            heap = vector<dlNode*>();
        }

        my_heap(const vector<int>& vec) {
            heap = vector<dlNode*>();
            if (vec.empty()) {
                return;
            }
            for (auto item : vec) {
                heap.push_back(item);
            }
            std::make_heap(heap.begin(), heap.end());
        }
};
