#pragma once

#include <cassert>
#include <utility>

// Implementation of the stubs for testing purposes

namespace Aii::Details{
 
  void AssertError(){
    assert(false);
  }

  template<typename T>
  void Delete(T* t){
    delete t;
  }

  template<typename T, typename ...Args>
  T* Allocate(Args ...args){
    // perfect forward the argument to the allocation function
    return new T{std::forward<T>(args)...};
  }

} // namespace Aii::Details
