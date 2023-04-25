#pragma once

#include <iostream>
#include <tuple>
#include "memseg.h"

struct dlNode {
    dlNode* prev = nullptr;
    dlNode* next = nullptr;
    memory_segment* segment = nullptr;
};

struct dlList {
    dlNode* head = nullptr;
    dlNode* tail = head;

    bool isEmpty() {
        return head == nullptr && head == tail;
    }

    void push_front(memory_segment* seg) {
        if (isEmpty()) {
            head = new dlNode({nullptr, nullptr, seg});
            tail = head;
            return;
        }
        if (head == tail) {
            head = new dlNode({nullptr, nullptr, seg});
            head->next = tail;
            return;
        }
        dlNode* tmp = new dlNode({nullptr, head, seg});
        tmp->next = head;
        head->prev = tmp;
        head = tmp;
    }

    void pop_front() {
        if (isEmpty()) {
            return;
        }
        if (head == tail) {
            delete head;
            head = nullptr;
            tail = head;
        }
        dlNode* tmp = head->next;
        tmp->prev = nullptr;
        delete head;
        head = tmp;
    }

    memory_segment* front() {
        if (isEmpty()) {
            return nullptr;
        }
        return head->segment;
    }

    void push_back(memory_segment* seg) {
        if (isEmpty()) {
            tail = new dlNode({nullptr, nullptr, seg});
            head = tail;
        }
        if (head == tail) {
            tail = new dlNode({head, nullptr, seg});
            head->next = tail;
        }
        dlNode* tmp = new dlNode({tail, nullptr, seg});
        tail->next = tmp;
        tail = tmp;
    }

    void pop_back() {
        if (isEmpty()) {
            return;
        }
        if (head == tail) {
            delete tail;
            tail = nullptr;
            head = tail;
        }
        dlNode* tmp = tail->prev;
        delete tail;
        tmp->next = nullptr;
        tail = tmp->next;
    }

    memory_segment* back() {
        if (isEmpty()) {
            return nullptr;
        }
        return tail->segment;
    }

    void print_list() {
        dlNode* tmp = head;
        while (tmp != nullptr) {
            std::cout << "length : " << tmp->segment->length << " status : " << tmp->segment->status << "\n";
            tmp = tmp->next;
        }
    }

    void free_node(dlNode* node) {
        if (node == nullptr) {
            return;
        }
        free_node(node->next);
        delete node->segment;
        delete node;
    }

    std::pair<dlNode*, dlNode*> split_segment(dlNode* node, int mem) {
        if (mem == 0 || node == nullptr) {
            return std::pair<dlNode*, dlNode*>(nullptr, nullptr);
        }

        int init = node->segment->length;
        if (init - mem == 0) {
            memory_segment* alloc_seg = new memory_segment(1, mem);
            dlNode* allocated = new dlNode({node->prev, node->next, alloc_seg});
            if (node->prev != nullptr){
                node->prev->next = allocated;
            } else {
                head = allocated;
            }
            if (node->next != nullptr) {
                node->next->prev = allocated;
            } else {
                tail = allocated;
            }
            delete node;
            return std::pair<dlNode*, dlNode*>(allocated, nullptr);
        }

        memory_segment* alloc_seg = new memory_segment(1, mem);
        memory_segment* free_seg = new memory_segment(0, init - mem);
        dlNode* allocated = new dlNode({node->prev, nullptr, alloc_seg});
        dlNode* left = new dlNode({allocated, node->next, free_seg});
        allocated->next = left;

        if (node->prev != nullptr) {
            node->prev->next = allocated;
        } else {
            head = allocated;
        }
        if (node->next != nullptr) {
            node->next->prev = left;
        } else {
            tail = left;
        }

        delete node->segment;
        delete node;
        return std::pair<dlNode*, dlNode*>(allocated, left);
    }

    dlNode* join_segments(dlNode* node) {
        if (node == nullptr) {
            return nullptr;
        }

        node->segment->status = segment_status::IS_DELETED;
        if ((node->next == nullptr || node->next->segment->status != 0) &&
            (node->prev == nullptr || node->prev->segment->status != 0)) {
            node->segment->status = segment_status::IS_FREE;
            return node;
        }

        size_t sum = node->segment->length;
        memory_segment* new_segment = new memory_segment(0, sum);
        dlNode* newNode = new dlNode({node->prev, node->next, new_segment});
        dlNode* nextNode = node->next;
        dlNode* prevNode = node->prev;

        if (nextNode != nullptr) {
            if (nextNode->segment->status == 0) {
                nextNode->segment->status = segment_status::IS_DELETED;
                sum += nextNode->segment->length;
                if (nextNode->next != nullptr) {
                    nextNode->next->prev = newNode;
                } else {
                    tail = newNode;
                }
                newNode->next = nextNode->next;
            } else {
                nextNode->prev = newNode;
            }
        } else {
            tail = newNode;
        }
        if (prevNode != nullptr) {
            if (prevNode->segment->status == 0) {
                prevNode->segment->status = segment_status::IS_DELETED;
                sum += prevNode->segment->length;
                if (prevNode->prev != nullptr) {
                    prevNode->prev->next = newNode;
                } else {
                    head = newNode;
                }
                newNode->prev = prevNode->prev;
            } else {
                prevNode->next = newNode;
            }
        } else {
            head = newNode;
        }

        newNode->segment->length = sum;
        return newNode;
    }
};