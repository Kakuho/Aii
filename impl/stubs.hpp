#pragma once

#ifdef TEST_HOSTED_ENVIRONMENT
  #include "./../tests/stubs.hpp"
#else
namespace Aii::Details{
  // Implement your platform support stubs here
  // see tests/stubs.hpp for an example
  inline void AssertError(){
    // ...
  }

  template<typename T>
  void Delete(T* t){
    // ... 
  }

  template<typename T, typename ...Args>
  T* Allocate(Args ...args){
    // ...
  }

} // namespace Aii::Details

#endif
