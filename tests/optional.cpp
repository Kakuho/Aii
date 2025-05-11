#include "doctest.h"

#include "aii/optional.hpp"

TEST_CASE("Optional Default Constructed"){
  Aii::Optional<int> opt{};
  CHECK(!opt.HasVal());
}
