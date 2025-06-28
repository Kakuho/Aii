#pragma once

// Static stack allocated array data type

#include <cstdint>

namespace Aii{

template <typename T, std::size_t N>
struct Array{
    T m_buffer[N];

    constexpr T& Front() noexcept;
    constexpr const T& Front() const noexcept;
    constexpr T& Back() noexcept;
    constexpr const T& Back() const noexcept;

    constexpr T* Data() const noexcept { return m_buffer; }

    constexpr bool Empty() const noexcept{ return N == 0;}
    constexpr std::size_t Size() const noexcept { return N; }

    constexpr T* begin() noexcept;
    constexpr T* end() noexcept;

    constexpr T& operator[](std::size_t index) noexcept;
    constexpr const T& operator[](std::size_t index) const noexcept;

    constexpr bool operator==(const Array<T,N>& rhs) noexcept;
    constexpr bool operator==(const Array<T,N>& rhs) const noexcept;
    constexpr bool operator!=(const Array& rhs) const noexcept;

    constexpr void Fill(const T& value) noexcept;
    constexpr void Swap(Array& other) noexcept;
};



} // namespace Aii

// Impl

template<typename T, std::size_t N>
constexpr T& 
Aii::Array<T, N>::Front() noexcept{
  return m_buffer[0];
}

template<typename T, std::size_t N>
constexpr const T& 
Aii::Array<T, N>::Front() const noexcept{
  return m_buffer[0];
}

template<typename T, std::size_t N>
constexpr T& 
Aii::Array<T, N>::Back() noexcept{
  if constexpr(N == 0){
    return m_buffer[0];
  }
  else{
    return m_buffer[N-1];
  }
}

template<typename T, std::size_t N>
constexpr const T& 
Aii::Array<T, N>::Back() const noexcept{
  if constexpr(N == 0){
    return m_buffer[0];
  }
  else{
    return m_buffer[N-1];
  }
}

template<typename T, std::size_t N>
constexpr T* 
Aii::Array<T, N>::begin() noexcept{
  return &m_buffer[0];
}

template<typename T, std::size_t N>
constexpr T* 
Aii::Array<T, N>::end() noexcept{
  return &m_buffer[N];
}

template<typename T, std::size_t N>
constexpr T& 
Aii::Array<T, N>::operator[](std::size_t index) noexcept{
  return m_buffer[index];
}


template<typename T, std::size_t N>
constexpr const T& 
Aii::Array<T, N>::operator[](std::size_t index) const noexcept{
  return m_buffer[index];
}

template<typename T, std::size_t N>
constexpr bool 
Aii::Array<T, N>::operator==(const Array<T,N>& rhs) noexcept{
  for(std::size_t i = 0; i < N; i++){
    if(m_buffer[i] != rhs[i]){
      return false;
    }
  }
  return true;
}

template<typename T, std::size_t N>
constexpr bool 
Aii::Array<T, N>::operator==(const Array<T,N>& rhs) const noexcept{
  for(std::size_t i = 0; i < N; i++){
    if(m_buffer[i] != rhs[i]){
      return false;
    }
  }
  return true;
}

template<typename T, std::size_t N>
constexpr bool 
Aii::Array<T, N>::operator!=(const Array<T, N>& rhs) const noexcept{
  return !(*this == rhs);
}

template<typename T, std::size_t N>
constexpr void Aii::Array<T, N>::Fill(const T& value) noexcept{
  for(auto& i: *this){
    i = value;
  }
}

template<typename T, std::size_t N>
constexpr void Aii::Array<T, N>::Swap(Array<T, N>& other) noexcept{
  // O(N) time complexity
  Aii::Array<T, N> tmp = *this;
  for(std::size_t i = 0; i < Size(); i++){
    (*this)[i] = other[i];
    other[i] = tmp[i];
  }
}
