#ifndef LLIST_H
#define LLIST_H

#include <cstddef>
#include <cstdint>
#include <forward_list>
#include <vector>

//template <typename T>
class LList {
    using T = int;
    class Node {
        alignas(alignof(T)) uint8_t m_data[sizeof(T)];
    public:
        uintptr_t pn{};
        uint8_t* data() noexcept { return m_data; }
        T* value() { return static_cast<T*>(static_cast<void*>(m_data)); }
        Node& setPrevNext(Node* prev, Node* next)
        {
            pn = reinterpret_cast<uintptr_t>(prev) ^ reinterpret_cast<uintptr_t>(next);
            return *this;
        }
        Node* prevNext(Node* prev_next)
        {
            return reinterpret_cast<Node*>(pn ^ reinterpret_cast<uintptr_t>(prev_next));
        }
        Node* prevNext(uintptr_t prev_next)
        {
            return reinterpret_cast<Node*>(pn ^ prev_next);
        }
    };

    Node* m_head{nullptr};
    Node* m_tail{nullptr};
    size_t m_capacity{0};
    size_t m_size{0};

public:
    LList(size_t capacity)
    {
        Node* head;
        Node* tail;
        if (create_nodes(&head, &tail, capacity))
        {
            m_head = head;
            m_tail = tail;
            m_capacity = capacity;
        }
    }

    LList& insert(const T& data)
    {
    }
    LList& extract(T& data) {}

private:
    static bool create_nodes(Node** out_head, Node** out_tail, size_t n)
    {
        if (n == 0) return false;
        bool status{false};
        Node* head{nullptr};
        Node* tail{nullptr};
        Node* prev{nullptr};
        size_t alloc_n;
        for (alloc_n = 0; alloc_n < n; ++alloc_n)
        {
            Node* next = new(std::nothrow) Node{};
            if (next != nullptr)
            {
                if (head == nullptr)
                {
                    head = next;
                    tail = next;
                }
                else
                {
                    tail->setPrevNext(prev, next);
                    prev = tail;
                    tail = next;
                }
            }
            else
            {
                break;
            }
        }
        if (alloc_n == n)
        {
            status = true;
            *out_head = head;
            *out_tail = tail;
        }
        else
        {
            destroy_nodes(nullptr, head, alloc_n);
        }

        return status;
    }
    static void destroy_nodes(Node* prev_head, Node* head, size_t n)
    {
        uintptr_t prev = reinterpret_cast<uintptr_t>(prev_head);
        for (size_t i = 0; i < n && head != nullptr; ++i)
        {
            Node* tmp = head;
            head = head->prevNext(prev);
            prev = reinterpret_cast<uintptr_t>(tmp);
            delete tmp;
        }
    }
};

#endif /* #ifndef LLIST_H */
