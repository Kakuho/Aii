#pragma once

// Wrapper for functions which are allowed to fail

#include <concepts>
#include <type_traits>
#include <utility>

namespace Aii{

template<typename E>
class Unexpected{
  public:
    using ErrorValue = E;
    constexpr explicit Unexpected(ErrorValue&& val): m_unexpectedVal{val}{}
    constexpr ErrorValue Error() const{ return m_unexpectedVal;}
  private:
    E m_unexpectedVal;
};

template<typename T, typename E>
class Expected{
  public:
    using ExpectedType = T;
    using ErrorType = E;

    constexpr Expected() noexcept = delete;
    constexpr Expected(const Expected& src) noexcept;
    constexpr Expected(Expected&& src) noexcept;
    constexpr Expected(Unexpected<ErrorType>&& src) noexcept;
    constexpr Expected(std::remove_cv_t<ExpectedType>&& src) noexcept;
    constexpr Expected(std::remove_cv_t<ErrorType>&& src) noexcept;
    ~Expected() noexcept;

    constexpr Expected& operator=(std::remove_cv_t<T> val)
    { m_val = val; m_isVal = true; return *this;}
    constexpr Expected& operator=(E error)
    { m_error = error; m_isVal = false; return *this;}
    constexpr Expected& operator=(const Expected<T, E>& src);

    constexpr const T* operator->() const noexcept{ return &m_val;}
    constexpr T* operator->() noexcept{ return &m_val;}

    constexpr const T& operator*() const noexcept{ return m_val;}
    constexpr T& operator*() noexcept{ return m_val;}

    constexpr explicit operator bool() const noexcept{ return m_isVal;}
    constexpr bool HasVal() const noexcept{ return m_isVal;}

    constexpr const ExpectedType& Val() const noexcept{ return m_val;}
    constexpr ExpectedType& Val() noexcept{ return m_val;}

    constexpr const ErrorType& Error() const noexcept{ return m_error;}
    constexpr ErrorType& Error() noexcept{ return m_error;}

    constexpr ExpectedType ValueOr(std::remove_cv_t<ExpectedType>&& dval) const;
    constexpr ErrorType ErrorOr(std::remove_cv_t<ErrorType>&& dval) const;

  private:
    union{
      ExpectedType m_val;
      ErrorType m_error;
    };
    bool m_isVal;
};

template<typename E>
class Expected<void, E>{
  public:
    using ExpectedType = void;
    using ErrorType = E;

    constexpr Expected() noexcept;
    constexpr Expected(const Expected& src) noexcept;
    constexpr Expected(Expected&& src) noexcept;
    constexpr Expected(Unexpected<ErrorType>&& src) noexcept;
    constexpr Expected(std::remove_cv_t<ErrorType>&& src) noexcept;
    ~Expected() noexcept;

    constexpr Expected& operator=(E error)
    { m_error = error; m_isVal = false; return *this;}
    constexpr Expected& operator=(const Expected& src);

    constexpr void operator*() const noexcept{ return;}

    constexpr explicit operator bool() const noexcept{ return m_isVal;}
    constexpr bool HasVal() const noexcept{ return m_isVal;}

    constexpr void Val() const noexcept{ return;}

    constexpr const ErrorType& Error() const noexcept{ return m_error;}
    constexpr ErrorType& Error() noexcept{ return m_error;}

    constexpr ErrorType ErrorOr(std::remove_cv_t<ErrorType>&& dval) const;

  private:
    union{
      char m_dummy;
      ErrorType m_error;
    };
    bool m_isVal;
};

} // namespace Aii

// Primary Template Aii::Expected<T, E>

template<typename T, typename E>
constexpr Aii::Expected<T, E>::Expected(const Expected& src) noexcept{
  m_isVal = src.m_isVal;
  if(m_isVal){
    m_val = src.m_val;
  }
  else{
    m_error = src.m_error;
  }
}

template<typename T, typename E>
constexpr Aii::Expected<T, E>::Expected(Expected&& src) noexcept{
  m_isVal = src.m_isVal;
  if(m_isVal){
    m_val = std::move(src.m_val);
  }
  else{
    m_error = std::move(src.m_error);
  }
}

template<typename T, typename E>
constexpr Aii::Expected<T, E>::Expected(Unexpected<ErrorType>&& unexp) noexcept
  :
    m_error{unexp.Error()},
    m_isVal{false}
{
}

template<typename T, typename E>
constexpr Aii::Expected<T, E>::Expected(std::remove_cv_t<ExpectedType>&& src) noexcept
  :
    m_val{src},
    m_isVal{true}
{

}

template<typename T, typename E>
constexpr Aii::Expected<T, E>::Expected(std::remove_cv_t<ErrorType>&& src) noexcept
  :
    m_error{src},
    m_isVal{false}
{

}

template<typename T, typename E>
Aii::Expected<T, E>::~Expected() noexcept{
  if(HasVal()){
    m_val.~T();
  }
  else{
    m_error.~E();
  }
}


template<typename T, typename E>
constexpr Aii::Expected<T, E>& 
Aii::Expected<T, E>::operator=(const Expected<T, E>& src){
  if(src){
    m_isVal = true;
    m_val = src.m_val;
  }
  else{
    m_isVal = false;
    m_error = src.m_error;
  }
  return *this;
}

template<typename T, typename E>
constexpr typename Aii::Expected<T, E>::ExpectedType
Aii::Expected<T, E>::ValueOr(std::remove_cv_t<ExpectedType>&& dval) const{
  if(m_isVal){
    return m_val;
  }
  else{
    return dval;
  }
}

template<typename T, typename E>
constexpr typename Aii::Expected<T, E>::ErrorType
Aii::Expected<T, E>::ErrorOr(std::remove_cv_t<ErrorType>&& dval) const{
  return m_isVal ? dval : m_error;
}

// Partial Specialisation Template Aii::Expected<void, E>


template<typename E>
constexpr Aii::Expected<void, E>::Expected() noexcept
  :
    m_dummy{0},
    m_isVal{true}
{
}

template<typename E>
constexpr Aii::Expected<void, E>::Expected(const Expected& src) noexcept{
  m_isVal = src.m_isVal;
  if(m_isVal){
    m_dummy = src.m_dummy;
  }
  else{
    m_error = src.m_error;
  }
}

template<typename E>
constexpr Aii::Expected<void, E>::Expected(Expected&& src) noexcept{
  m_isVal = src.m_isVal;
  if(m_isVal){
    m_dummy = std::move(src.m_dummy);
  }
  else{
    m_error = std::move(src.m_error);
  }
}

template<typename E>
constexpr Aii::Expected<void, E>::Expected(Unexpected<ErrorType>&& src) noexcept
  :
    m_error{src.Error()},
    m_isVal{false}
{
}

template<typename E>
constexpr Aii::Expected<void, E>::Expected(std::remove_cv_t<ErrorType>&& src) noexcept
  :
    m_error{src},
    m_isVal{false}
{
}

template<typename E>
Aii::Expected<void, E>::~Expected() noexcept{
  if(HasVal()){
  }
  else{
    m_error.~E();
  }
}

template<typename E>
constexpr Aii::Expected<void, E>& 
Aii::Expected<void, E>::operator=(const Expected<void, E>& src){
  if(src){
    m_isVal = true;
  }
  else{
    m_isVal = false;
    m_error = src.m_error;
  }
  return *this;
}

template<typename E>
constexpr typename Aii::Expected<void, E>::ErrorType 
Aii::Expected<void, E>::ErrorOr(std::remove_cv_t<ErrorType>&& dval) const{
  return m_isVal ? dval : m_error;
}
