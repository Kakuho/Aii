#include "doctest.h"

#include "aii/expected.hpp"
#include "aii/error.hpp"

TEST_CASE("Expected<T, E> Constructed with expected type"){
  Aii::Expected<int, Aii::Error> exp{100};
  CHECK(exp.HasVal());
}

TEST_CASE("Expected<T, E> Constructed with error type"){
  Aii::Expected<int, Aii::Error> exp{Aii::Error::Undefined};
  CHECK(!exp.HasVal());
}

TEST_CASE("Expected<T, E> Constructed with unexpected type"){
  Aii::Expected<int, Aii::Error> exp{Aii::Unexpected{Aii::Error::InvalidArgument}};
  CHECK(!exp.HasVal());
}

TEST_CASE("Expected<T, E> copy assignment should copy the presence value"){
  Aii::Expected<int, Aii::Error> exp{Aii::Error::Undefined};

  SUBCASE("copying when the source has a value"){
    exp = Aii::Error::Undefined;
    REQUIRE(exp.HasVal() == false);

    Aii::Expected<int, Aii::Error> src{10};
    REQUIRE(src.HasVal() == true);
    
    exp = src;
    CHECK(exp.HasVal() == true);
  }

  SUBCASE("copying when the source does not have a value"){
    exp = 10;
    REQUIRE(exp.HasVal() == true);

    Aii::Expected<int, Aii::Error> src{Aii::Error::InvalidArgument};
    REQUIRE(src.HasVal() == false);
    
    exp = src;
    CHECK(exp.HasVal() == false);
    CHECK(exp.Error() == Aii::Error::InvalidArgument);
  }
}

TEST_CASE("Expected<T, E> value or monadic operation"){
  SUBCASE("case with unexpected value"){
    Aii::Expected<int, Aii::Error> exp{Aii::Error::Undefined};
    int i = exp.ValueOr(100);
    CHECK(i == 100);
  }
  SUBCASE("case with expected value"){
    Aii::Expected<int, Aii::Error> exp{25};
    int i = exp.ValueOr(100);
    CHECK(i == 25);
  }
}

TEST_CASE("Expected<T, E> error or monadic operation"){
  using Aii::Error;
  SUBCASE("case with unexpected value"){
    Aii::Expected<int, Aii::Error> exp{Error::Undefined};
    Error error = exp.ErrorOr(Error::InvalidArgument);
    CHECK(error == Error::Undefined);

  }
  SUBCASE("copying when the source does not have a value"){
    Aii::Expected<int, Aii::Error> exp{25};
    Error error = exp.ErrorOr(Error::InvalidArgument);
    CHECK(error == Error::InvalidArgument);
  }
}
