#pragma once

// optional allows nullable value type semantics

#include <utility>
#include <new>
#include <type_traits>

namespace Aii{

struct NullOptType{};
static inline constexpr NullOptType NullOpt{};

template<typename T>
class Optional{
  public:;
    constexpr Optional() noexcept;
    constexpr Optional(NullOptType) noexcept;
    constexpr Optional(const Optional& src) noexcept;
    template<typename U> 
      requires std::is_constructible_v<T, U>
    constexpr Optional(const U& src) noexcept;
    constexpr Optional(Optional&& src) noexcept;
    constexpr ~Optional() noexcept;

    constexpr const T* operator->() const noexcept{return &m_val;}
    constexpr T* operator->() noexcept{return &m_val;}

    constexpr const T& operator*() const noexcept{return m_val;}
    constexpr T& operator*() noexcept{return m_val;}

    constexpr explicit operator bool() const noexcept{return HasVal();}
    constexpr bool HasVal() const noexcept{return !m_empty;}

    constexpr const T& Val() const noexcept{return m_val;}
    constexpr T& Val() noexcept{return m_val;}

    template<typename U = std::remove_cv_t<T>> 
    constexpr T ValOr(U&& defaultVal) const;

    constexpr void Swap(Optional& other) noexcept;
    constexpr void Reset() noexcept;

  private:
    union{
      char m_dummy;
      T m_val;
    };
    bool m_empty;
};

} // namespace Aii

template<typename T>
constexpr Aii::Optional<T>::Optional() noexcept
  :
    m_dummy{0},
    m_empty{true}
{

}

template<typename T>
constexpr Aii::Optional<T>::Optional(Aii::NullOptType) noexcept
  :
    m_dummy{0},
    m_empty{true}
{

}

template<typename T>
constexpr Aii::Optional<T>::Optional(const Aii::Optional<T>& src) noexcept
  : 
    m_empty{src.m_empty}
{
  m_empty = src.m_empty;
  if(m_empty){
    m_dummy = 0;
  }
  else{
    m_val = src.m_val;
  }
}

template<typename T> template<typename U>
  requires std::is_constructible_v<T, U>
constexpr Aii::Optional<T>::Optional(const U& src) noexcept
  :
    m_val{src},
    m_empty{false}
{
  
}

template<typename T>
constexpr Aii::Optional<T>::Optional(Aii::Optional<T>&& src) noexcept
  : 
    m_empty{src.m_empty}
{
  if(m_empty){
    m_dummy = 0;
  }
  else{
    m_val = std::move(src.m_val);
    src.m_val = 0;
  }
}

template<typename T>
constexpr Aii::Optional<T>::~Optional() noexcept
{
  if(m_empty){
    return;
  }
  else{
    m_val.~T();
  }
}

template<typename T> template<typename U>
constexpr T Aii::Optional<T>::ValOr(U&& defaultVal) const{
  if(!HasVal()){
    return defaultVal;
  }
  else{
    return m_val;
  }
}

template<typename T>
constexpr void Aii::Optional<T>::Swap(Optional& other) noexcept{
  if(HasVal()){
    if(other.HasVal()){
      using std::swap;
      swap(*(*this), *other);
    }
    else{
      new(&other.m_val) T{std::move(m_val)};
      other.m_empty = false;

      m_val.~T();
      m_empty = true;
    }
  }
  else{
    if(other.HasVal()){
      new(&m_val) T{std::move(other.m_val)};
      m_empty = false;

      other.m_val.~T();
      other.m_empty = true;
    }
    else{
      return;
    }
  }
}

template<typename T>
constexpr void Aii::Optional<T>::Reset() noexcept{
  if(HasVal()){
    m_val.T::~T();
    m_empty = true;
  }
  else{
    return;
  }
}
