set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(TESTS "Compile Tests" OFF)
if(TESTS)
  message("Compile Tests On")

  add_compile_options(-g -O0 -Wall -Wextra)

  set(SRCS
        main.cpp
        array.cpp
        expected.cpp
        optional.cpp
  )

  add_executable(tests ${SRCS})

  add_custom_target(run-tests tests
    DEPENDS tests
  )
endif(TESTS)
