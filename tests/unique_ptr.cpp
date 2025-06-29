#include "doctest.h"

// Tests for Aii::UniquePtr<T, D>

#include "aii/unique_ptr.hpp"
#include "aii/error.hpp"
#include <cstddef>

TEST_CASE("UniquePtr<T, D> constructors tests"){
  SUBCASE("Default constructor should be nullptr"){
    Aii::UniquePtr<int> uptr{};
    CHECK(uptr.Get() == nullptr);
  }
  SUBCASE("Constructor with a std::nullptr_t should be nullptr"){
    Aii::UniquePtr<int> uptr{nullptr};
    CHECK(uptr.Get() == nullptr);
  }
  SUBCASE("Constructor with a raw pointer should store the pointer"){
    int* pmyNum = new int{100};
    Aii::UniquePtr<int> uptr{pmyNum};
    CHECK(uptr.Get() == pmyNum);
  }
  SUBCASE("Constructor with a raw pointer should store the pointer"){
    int* pmyNum = new int{100};
    Aii::UniquePtr<int> uptr{pmyNum};
    CHECK(uptr.Get() == pmyNum);
  }
  SUBCASE("Constructor by move should kill the moved from container"){
    int* pmyNum = new int{100};
    Aii::UniquePtr<int> uptr1{pmyNum};
    REQUIRE(uptr1.Get() == pmyNum);

    Aii::UniquePtr<int> uptr2{std::move(uptr1)};
    CHECK(uptr1.Get() == nullptr);
    CHECK(uptr2.Get() == pmyNum);
  }
}

TEST_CASE("UniquePtr<T, D> assignment tests"){
  SUBCASE("Assignment with a nullptr should be nullptr"){
    int* pmyNum = new int{100};
    Aii::UniquePtr<int> uptr{pmyNum};
    REQUIRE(uptr.Get() == pmyNum);

    uptr.Reset();
    CHECK(uptr.Get() == nullptr);
  }

  SUBCASE("Assignment by move should kill the moved from containter"){
    int* pmyNum1 = new int{121};
    int* pmyNum2 = new int{323};
    Aii::UniquePtr<int> uptr1{pmyNum1};
    Aii::UniquePtr<int> uptr2{pmyNum2};
    REQUIRE(uptr1.Get() == pmyNum1);
    REQUIRE(uptr2.Get() == pmyNum2);

    uptr1 = std::move(uptr2);
    CHECK(uptr1.Get() == pmyNum2);
    CHECK(uptr2.Get() == nullptr);
  }
}

TEST_CASE("UniquePtr<T, D> Reset and Release operations"){
  SUBCASE("Release should release ownership of the pointer"){
    int* pmyNum = new int{100};
    Aii::UniquePtr<int> uptr{pmyNum};
    REQUIRE(uptr.Get() == pmyNum);

    auto placeholder = uptr.Release();
    delete placeholder; 

    CHECK(uptr.Get() == nullptr);
  }

  SUBCASE("Reset should replace the conatained pointer"){
    int* pmyNum = new int{100};
    int* replacer = new int{323};

    Aii::UniquePtr<int> uptr{pmyNum};
    REQUIRE(uptr.Get() == pmyNum);

    uptr.Reset(replacer);
    CHECK(uptr.Get() == replacer);
  }

  SUBCASE("Reset without an argument should replace the contained pointer with nullptr"){
    int* pmyNum = new int{100};
    Aii::UniquePtr<int> uptr{pmyNum};
    REQUIRE(uptr.Get() == pmyNum);

    uptr.Reset();
    CHECK(uptr.Get() == nullptr);
  }
}

TEST_CASE("UniquePtr<T, D> Contextual Conversions"){
  SUBCASE("Explicit cast within if statements"){
    int* pmyNum = new int{100};
    Aii::UniquePtr<int> uptr{pmyNum};
    REQUIRE(uptr.Get() == pmyNum);
    if(uptr){
      CHECK(true);
    }
    else{
      CHECK(false);
    }
  }

  SUBCASE("Cast within if statement, false"){
    Aii::UniquePtr<int> uptr{};
    REQUIRE(uptr.Get() == nullptr);
    if(uptr){
      CHECK(true);
    }
    else{
      CHECK(false);
    }
  }

  SUBCASE("Requires a cast when used in implicit context"){
    int* pmyNum = new int{100};
    Aii::UniquePtr<int> uptr{pmyNum};
    REQUIRE(uptr.Get() == pmyNum);

    //bool v = uptr;
    bool v = static_cast<bool>(uptr);
  }
}

struct S{
  int num;
};

TEST_CASE("UniquePtr<T, D> Pointer Semantics Operations"){
  SUBCASE("Dereference Operator"){
    int* pmyNum = new int{100};
    Aii::UniquePtr<int> uptr{pmyNum};
    REQUIRE(uptr.Get() == pmyNum);
    CHECK(*uptr == *pmyNum);
  }

  SUBCASE("Structure Pointer Derference"){
    S* pmyS = new S{100};
    Aii::UniquePtr<S> uptr{pmyS};
    REQUIRE(uptr.Get() == pmyS);
    CHECK(uptr->num == pmyS->num);
  }
}
