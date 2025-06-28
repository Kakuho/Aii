#include "doctest.h"

#include "aii/expected.hpp"
#include "aii/error.hpp"

TEST_CASE("Expected<void, E> Constructed with expected type"){
  Aii::Expected<void, Aii::Error> exp;
  CHECK(exp.HasVal());
}

TEST_CASE("Expected<void, E> Constructed with error type"){
  Aii::Expected<void, Aii::Error> exp{Aii::Error::Undefined};
  CHECK(!exp.HasVal());
}


TEST_CASE("Expected<void, E> Constructed with unexpected type"){
  Aii::Expected<void, Aii::Error> exp{Aii::Unexpected<Aii::Error>{Aii::Error::InvalidArgument}};
  CHECK(!exp.HasVal());
}

TEST_CASE("Expected<void, E> copy assignment should copy the presence value"){
  Aii::Expected<void, Aii::Error> exp;

  SUBCASE("copying when the source is void"){
    exp = Aii::Error::Undefined;
    REQUIRE(exp.HasVal() == false);

    Aii::Expected<void, Aii::Error> src;
    REQUIRE(src.HasVal() == true);
    
    exp = src;
    CHECK(exp.HasVal() == true);
  }

  SUBCASE("copying when the source has an unexpected value"){
    REQUIRE(exp.HasVal() == true);

    Aii::Expected<void, Aii::Error> src{Aii::Error::InvalidArgument};
    REQUIRE(src.HasVal() == false);
    
    exp = src;
    CHECK(exp.HasVal() == false);
    CHECK(exp.Error() == Aii::Error::InvalidArgument);
  }
}
