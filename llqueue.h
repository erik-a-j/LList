#ifndef LLQUEUE_H
#define LLQUEUE_H

#include "llnode.h"
#include <utility>

namespace ll {

template <typename T, typename Alloc = std::allocator<T>>
class Queue {
    using Node = NodeType<T, Alloc>;
    using NodeAllocator = Node::NodeAllocator;
    using NodeTraits = Node::NodeTraits;

    using NodePair = std::pair<Node*, Node*>;

    struct NodePos {
        Node* prev;
        Node* curr;
    };

    NodeAllocator m_alloc{};
    NodePos m_head{nullptr, nullptr};
    NodePos m_tail{nullptr, nullptr};
    size_t m_capacity{0};
    size_t m_size{0};

public:

    Queue(size_t capacity)
    {
        auto [head, tail] = create_nodes(capacity);
        if (head && tail)
        {
            head->updateLink(nullptr, tail);
            tail->updateLink(nullptr, head);
            m_head.curr = head;
            m_tail.curr = head;
            m_head.prev = tail;
            m_tail.prev = tail;
            m_capacity = capacity;
        }
    }

private:
    NodePair create_nodes(size_t n)
    {
        Node* head{nullptr};
        Node* tail{nullptr};
        size_t alloc_n;
        try
        {
            for (alloc_n = 0; alloc_n < n; ++alloc_n)
            {
                Node* node = NodeTraits::allocate(m_alloc, 1);
                NodeTraits::construct(m_alloc, node);

                node->setLinks(tail, nullptr);
                if (head == nullptr) head = node;
                else                 tail->updateLink(nullptr, node);
                tail = node;
            }
        }
        catch (...)
        {
            destroy_nodes(NodePos{nullptr, head}, alloc_n);
            head = nullptr;
            tail = nullptr;
        }

        return NodePair{head, tail};
    }
    void destroy_nodes(NodePos pos, size_t n)
    {
        (void)pos;
        (void)n;
    }
};

} // namespace ll

#endif /* #ifndef LLQUEUE_H */
