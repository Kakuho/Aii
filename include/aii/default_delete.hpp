#pragma once

// Stateless delete helper class for types using dynamic memory

#include "stubs.hpp"

namespace Aii{

template<typename T>
class DefaultDelete{
  public:
    DefaultDelete() = default;

    void operator()(T* pointer){
      Details::Delete(pointer);
    }
};

} // namespace Aii
