# Aii

`Aii` aims to be a freestanding, exception free header only library, for the 
`Chisaka` operating system kernel.

## Data Structure Organisation

Data structures in `Aii` can come in a variety of forms:
  
  * `Container` - A container version which will manage its own internal data. This tries to mimic the C++ standard library.
  * `Crtp::Container` - A crtp version of the container which user written code can inherit from in order to use its interface. 
  * `Crtp::ContainerNode` - A crtp version of the container internal node, allowing more granular memory management from the user. 

Some data structures has implenetation of all the forms when suitable, 
while others only has an implementation of the `Container` form.

## stub.hpp

Each data type has a template argument `Allocator` which is `Aii::Allocator` by default. 
If you want to use `Aii` in your own kernel, and want the default memory allocations 
made with your own custom heap, implement the files in `stub.hpp` according to your 
own system. `Aii::Allocator` is a stateless allocator and calls these stubs in order 
to allocate memory.

Similarly smart pointer types has a template parameter `D` which defaults to 
`Aii::DefaultDeleter`. Implementing the stubs will allow `Aii::DefaultDeleter` to 
call your custom memory resource deallocation function by default.

You can of course, implement your own allocator however you must ensure it meets 
the requirements of the `Aii::IsAllocator<T>` concept.

## Testing

All of the tests are located in the `tests/` directory. All tests are written 
using the `doctest` framework and are compiled into the `tests` executable

If you wish to compile the `tests` executable, follow the instructions 
in `extern/README.md`

Tests are ran in a hosted environment, where the stubs are implemented using the default
`new` and `delete` operators.
