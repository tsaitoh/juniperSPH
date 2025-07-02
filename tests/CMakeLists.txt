cmake_minimum_required(VERSION 3.31)
project(sph_tests)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create a test for the kernel.cpp module:
add_executable(KernelTest test_kernel.cpp) # First create an initial executable from the test case code.
target_link_libraries(KernelTest PRIVATE gtest_main) # Link the test module to the gtest framework
target_link_libraries(KernelTest PRIVATE juniperSRC) # Add the kernel code modules to a `kernel_src` library
add_test(NAME sph_example_tests COMMAND KernelTest) # Add the test as a new run configuration `KernelTest`
