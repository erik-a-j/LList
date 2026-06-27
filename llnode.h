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

template <typename NodeType>
struct NodePtrArg {
    uintptr_t value{};

    NodePtrArg() = default;
    NodePtrArg(NodeType* p) : value{reinterpret_cast<uintptr_t>(p)} {}
    NodePtrArg(uintptr_t p) : value{p} {}

    operator uintptr_t() const noexcept { return value; }
    operator NodeType* () const noexcept { return reinterpret_cast<NodeType*>(value); }
};

} // namespace detail



template <typename T, typename Alloc>
class NodeType {
public:
    //using Alloc = std::allocator<int>;

    using value_type = T;
    using NodeAllocator = std::allocator_traits<Alloc>::template rebind_alloc<NodeType>;
    using NodeTraits = std::allocator_traits<Alloc>::template rebind_traits<NodeType>;

    using PtrArg = detail::NodePtrArg<NodeType>;

private:
    uintptr_t m_links;
    detail::deferredInit<value_type> m_data;

public:

    NodeType(PtrArg prev = PtrArg{}, PtrArg next = PtrArg{}) : m_links{prev ^ next} {}

    NodeType* setLinks(PtrArg prev, PtrArg next) noexcept
    {
        m_links = prev ^ next;
        return this;
    }
    NodeType* updateLink(PtrArg old_link, PtrArg new_link) noexcept
    {
        m_links ^= old_link, m_links ^= new_link;
        return this;
    }
    NodeType* getLink(PtrArg other_link) noexcept
    {
        return reinterpret_cast<NodeType*>(m_links ^ other_link);
    }

    template <typename... Args>
    NodeType* constructData(Args&&... args)
    {
        new(&m_data.obj) value_type{std::forward<Args>(args)...};
        return this;
    }
    NodeType* destroyData()
    {
        m_data.obj.~value_type();
        return this;
    }
};

} // namespace ll

#endif /* #ifndef LLNODE_H */
