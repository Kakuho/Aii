#include "doctest.h"

#include "aii/array.hpp"
#include "aii/error.hpp"

TEST_CASE("Array constructed with aggegrate initialisation"){
  Aii::Array<int, 100> a = {1, 2 , 3};
  CHECK(a.Size() == 100);
  CHECK(a[1] == 2);
  CHECK(a[2] == 3);
  CHECK(a[3] == 0);
  CHECK(a[50] == 0);
}

TEST_CASE("Array operation fill"){
  Aii::Array<int, 5> a = {1, 2, 3, 4, 5};
  CHECK(a.Size() == 5);
  CHECK(a[0] == 1);
  CHECK(a[1] == 2);
  CHECK(a[2] == 3);
  CHECK(a[3] == 4);
  CHECK(a[4] == 5);
  a.Fill(10);
  CHECK(a[0] == 10);
  CHECK(a[1] == 10);
  CHECK(a[2] == 10);
  CHECK(a[3] == 10);
  CHECK(a[4] == 10);
}

TEST_CASE("Array operation swap"){
  Aii::Array<int, 5> a = {1, 2, 3, 4, 5};
  Aii::Array<int, 5> b = {101, 102, 103, 104, 105};
  CHECK(a.Size() == 5);
  a.Swap(b);
  CHECK(a == Aii::Array<int, 5>{101, 102, 103, 104, 105});
  CHECK(b == Aii::Array<int, 5>{1, 2, 3, 4, 5});
}

TEST_CASE("Array tests for equality and inequality"){
  Aii::Array<int, 5> a = {1, 2, 3, 4, 5};
  Aii::Array<int, 5> b = {101, 102, 103, 104, 105};
  Aii::Array<int, 5> c = a;
  CHECK(a != b);
  CHECK(a == c);
}
