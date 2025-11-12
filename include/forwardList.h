#pragma once

#include <iostream>
#include <memory_resource>
#include <vector>
#include <concepts>
#include <memory>
#include <exception>
#include <algorithm>

template <class T, class allocatorType>
    requires std::is_default_constructible_v<T> && 
             std::is_same_v<allocatorType, std::pmr::polymorphic_allocator<T>>
class forwardList {
    private:
        struct Node {
            T data;
            Node* next;

            Node() : data(), next(nullptr) {}
            Node(const T& value) : data(value), next(nullptr) {}
            Node(T&& value) : data(std::move(value)), next(nullptr) {}

            ~Node() = default;
        };

        allocatorType allocator;
        Node* head = nullptr;
        Node* tail = nullptr;
        size_t size = 0;
    
    public:
        explicit forwardList(const allocatorType& alloc = {}) {
            : allocator(alloc) {}
        }

        void pushFront(const T& value) {
            Node* newNode = allocator.allocate(1);
            allocator.construct(newNode, value);

            newNode->next = head;
            head = newNode;

            if (!tail) tail = head;
            size++;
        }

        void pushBack(const T& value) {
            Node* newNode = allocator.allocate(1);
            allocator.construct(newNode, value);

            if (!head) {
                head = tail;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            size++;
        }

        void insert(const T& value, size_t pos) {
            if (pos == 0) {
                pushFront(value);
                return;
            }

            if (pos > size) {
                pushBack(value);
                return;
            }

            Node* curr = head;
            for (size_t i = 0; i < pos - 1; ++i) {
                curr = curr->next;
            }

            Node* newNode = allocator.allocate(1);
            allocator.construct(newNode, value);

            newNode.next = curr->next;
            curr->next = newNode;
            size++;
        }

        void popFront() {
            if (!head) return;

            Node* temp = head;
            head = head->next;

            allocator.destroy(temp);
            allocator.deallocate(temp, 1);
            size--;

            if (!head) tail = nullptr;
        }

        void popBack() {
            if (!head) return;

            if (head == tail) {
                popFront();
                return;
            }

            Node* curr = head;
            while (curr->next != tail) {
                curr = curr->next;
            }

            allocator.destroy(tail);
            allocator.deallocate(tail, 1);
            size--;

            tail = curr;
            tail->next = nullptr;
        }

        void remove(size_t pos) {
            if (pos == 0) {
                popFront();
                return;
            }

            if (pos > size - 1) {
                popBack();
                return;
            }

            Node* curr = head;
            for (size_t i = 0; i < pos - 1; ++i) {
                curr = curr->next;
            }

            Node* disappearing = curr->next;
            curr->next = disappearing->next;

            allocator.destroy(disappearing);
            allocator.deallocate(disappearing, 1);
            size--;
        }

        int find(const T& value) const {
            Node* curr = head;
            int pos = 0;

            while (curr) {
                if (curr->data == value) {
                    return pos;
                }

                curr = curr->next;
                pos++;
            }

            return -1;
        }

        size_t size() const {return size; }
        bool isEmpty() const {return size == 0; }

        void clear() {
            while (head) {
                frontPop();
            }
        }

        void print() const {
            Node* curr = head;
            while (curr) {
                std::cout << curr->data << " -> ";
                curr = curr->next;
            }

            std::cout << "null" << std::endl;
        }

        ~forwardList() {
            clear();
        }

        class iterator {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = T;
                using difference_type = std::ptrdiff_t;
                using pointer = T*;
                using reference = T&;

                iterator() : current(nullptr) {}
                explicit iterator(Node* node) : current(node) {}

                reference operator*() const {
                    return current->data;
                }

                pointer operator->() const {
                    return &current->data;
                }

                iterator& operator++() {
                    if (current) {
                        current = current->next;
                    }

                    return *this;
                }

                iterator operator++(int) {
                    iterator temp = *this;
                    ++(*this);
                    return temp;
                }

                bool operator==(const iterator& other) const {
                    return current == other.current;
                }

                bool operator!=(const iterator& other) const {
                    return !(*this == other);
                }
                
            private:
                Node* current;
                friend class forwardList;
        }
};