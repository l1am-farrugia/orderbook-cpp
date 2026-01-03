cmake_minimum_required(VERSION 3.20)
project(exchange_order_book_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(orderbook
    src/order.cpp
    src/order_book.cpp
)

target_include_directories(orderbook PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(ob_sim
    src/main.cpp
)

target_link_libraries(ob_sim PRIVATE orderbook)

# ---- Tests (GoogleTest via submodule) ----
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

enable_testing()
add_subdirectory(external/googletest)

add_executable(ob_tests
    tests/test_order_book.cpp
)

target_link_libraries(ob_tests PRIVATE orderbook GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(ob_tests)

