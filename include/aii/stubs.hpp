#pragma once

//  The following lists function declarations that must be implemented in order
//  to use the full capacity of Aii. Mainly used for heap allocation, so you must provide
//  a custom heap allocator

namespace Aii::Details{
 
  inline void AssertError();

  template<typename T>
  void Delete(T* t);

  template<typename T, typename ...Args>
  T* Allocate(Args ...args);

} // namespace Aii::Details
