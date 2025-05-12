#pragma once

namespace Aii{
  template<typename T>
  struct LessThan{
    constexpr bool operator()(T& lhs, T& rhs){
      return lhs < rhs;
    }
  };

  template<typename T>
  struct MoreThan{
    constexpr bool operator()(T& lhs, T& rhs){
      return lhs > rhs;
    }
  };
}
