// -*- C++ -*-
// HeXu's
// 2013 Nov

#ifndef MINI_STL_UNORDERED_MAP_INC
#define MINI_STL_UNORDERED_MAP_INC

#include "singly_linked_list.hpp"

namespace Hx {

/**
 * A helper node class for unordered_map.
 * This is just a linked list with uninitialized storage for a
 * data value in each node.
 */
template <typename T>
struct unordered_map_node: public singly_linked::list_node_t {
	// raw storage buffer for type T
    // typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type storage;     // deprecated in C++23
    struct alignas(alignof(T)) { char data[sizeof(T)]; } storage;

    T* valptr() noexcept
    {
        return static_cast<T*>(static_cast<void*>(&storage));
    }

    const T* valptr() const noexcept
    {
        return static_cast<const T*>(static_cast<const void*>(&storage));
    }
};

} // namespace Hx

#endif  // MINI_STL_UNORDERED_MAP_INC
