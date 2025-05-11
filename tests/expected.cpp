#include "doctest.h"

#include "aii/expected.hpp"
#include "aii/error.hpp"

TEST_CASE("Expected Constructed with expected type"){
  Aii::Expected<int, Aii::Error> exp{100};
  CHECK(exp.HasVal());
}
