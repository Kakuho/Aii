#pragma once

// Stateless Heap Memory Allocator. 
// Must ensure that the functions in stubs.hpp are implemented before this can be used

#include <cstdint>

#include "aii/stubs.hpp"

namespace Aii{
  template<typename T>
  class Allocator{
    constexpr Allocator() noexcept = default;
    constexpr ~Allocator() noexcept = default;

    T* Allocate() noexcept{
      return Details::Allocate<T>();
    }

    T* Allocate(std::size_t n) noexcept{
      return Details::Allocate<T>();
    }

    void Deallocate(T* obj) noexcept{
      Details::Delete(obj);
    }
  };
}
