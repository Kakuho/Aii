#pragma once

//  namespace Aii::Details holds all the stubs necessary for Aii to work.
//  The following lists function declarations that must be implemented in order
//  to use the full capacity of Aii

namespace Aii::Details{
 
  inline void AssertError();

  template<typename T>
  void Delete(T* t);

  template<typename T, typename ...Args>
  T* Allocate(Args ...args);

} // namespace Aii::Details
