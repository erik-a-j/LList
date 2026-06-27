#ifndef LLNODE_H
#define LLNODE_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <type_traits>

namespace ll {

namespace detail {

template <typename T>
union deferredInit {
    deferredInit() {}
    ~deferredInit() {}
    T obj;
};

} // namespace detail

//template <typename Alloc>
class Node {
public:
    using Alloc = std::allocator<int>;

    using value_type = typename std::allocator_traits<Alloc>::value_type;
    using NodeAllocator = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    using NodeTraits = typename std::allocator_traits<Alloc>::template rebind_traits<Node>;

private:
    uintptr_t m_links;
    detail::deferredInit<value_type> m_data;

public:

    Node() : m_links{reinterpret_cast<uintptr_t>(nullptr)} {}

    Node* setLinks(uintptr_t prev, uintptr_t next) noexcept
    {
        m_links = prev ^ next;
        return this;
    }
    Node* updateLink(uintptr_t old_link, uintptr_t new_link) noexcept
    {
        m_links ^= old_link, m_links ^= new_link;
        return this;
    }
    Node* getLink(uintptr_t other_link) noexcept
    {
        return reinterpret_cast<Node*>(m_links ^ other_link);
    }

    template <typename... Args>
    Node* constructData(Args&&... args)
    {
        new(&m_data.obj) value_type{std::forward<Args>(args)...};
        return this;
    }
    Node* destroyData()
    {
        m_data.obj.~value_type();
        return this;
    }
};

} // namespace ll

#endif /* #ifndef LLNODE_H */
