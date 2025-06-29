#include "doctest.h"

#include "aii/optional.hpp"
#include "aii/unique_ptr.hpp"

TEST_CASE("Aii::Optional<T> Constructors tests"){
  SUBCASE("Default Constructor should leave the optional empty"){
    Aii::Optional<int> opt{};
    CHECK(!opt.HasVal());
  }

  SUBCASE("Constructor with Aii::Nullopt leaves the optional empty"){
    Aii::Optional<int> opt{Aii::NullOpt};
    CHECK(!opt.HasVal());
  }
  SUBCASE("Copy Constructor when the source is empty - "
          "hasval is false"){
    Aii::Optional<int> src{};
    REQUIRE(!src.HasVal());

    Aii::Optional<int> opt{src};
    CHECK(!opt.HasVal());
  }
  SUBCASE("Copy Constructor when the source is non empty - "
          "hasval is true"){
    Aii::Optional<int> src{100};
    REQUIRE(src.HasVal());
    REQUIRE(src.Val() == 100);

    Aii::Optional<int> opt{src};
    CHECK(opt.HasVal());
    CHECK(opt.Val() == 100);
  }
  SUBCASE("Value conversion constructor should not be empty"){
    Aii::Optional<int> opt{100};
    CHECK(opt.HasVal());
    CHECK(opt.Val() == 100);
  }
  SUBCASE("Value conversion constructor should not be empty"){
    Aii::Optional<int> opt{100};
    CHECK(opt.HasVal());
    CHECK(opt.Val() == 100);
  }
  SUBCASE("Move constructor should take the source value"){
    int* pnum = new int{};
    Aii::Optional<Aii::UniquePtr<int>> opt1{pnum};
    Aii::Optional<Aii::UniquePtr<int>> opt2{std::move(opt1)};

    CHECK(opt2.HasVal());
    CHECK(opt2.Val().Get() == pnum);

    CHECK(opt1.HasVal());
    CHECK(opt1.Val().Get() == nullptr);

  }
}

TEST_CASE("Aii::Optional<T> Swap tests"){
  SUBCASE("Swap between non-empty and empty"){
    Aii::Optional<int> opt1{100};
    Aii::Optional<int> opt2;

    opt1.Swap(opt2);

    CHECK(!opt1.HasVal());

    CHECK(opt2.HasVal());
    CHECK(opt2.Val() == 100);
  }
  SUBCASE("Swap between empty and empty"){
    Aii::Optional<int> opt1;
    Aii::Optional<int> opt2;

    opt1.Swap(opt2);

    CHECK(!opt1.HasVal());
    CHECK(!opt2.HasVal());

  }
  SUBCASE("Swap between non-empty and non-empty"){
    Aii::Optional<int> opt1{100};
    Aii::Optional<int> opt2{323};

    opt1.Swap(opt2);

    CHECK(opt1.HasVal());
    CHECK(opt1.Val() == 323);

    CHECK(opt2.HasVal());
    CHECK(opt2.Val() == 100);
  }
}

TEST_CASE("Aii::Optional<T> Reset tests"){
  SUBCASE("Reset when empty"){
  }
  SUBCASE("Reset when non empty"){
  }
}

TEST_CASE("Aii::Optional<T> Dereference Operator tests"){
  SUBCASE(""){
  }
}

TEST_CASE("Aii::Optional<T> Monadic tests"){
  SUBCASE(""){
  }
}
