cmake_minimum_required (VERSION 3.13)
project(ARITH_BDD)

set (CMAKE_CXX_STANDARD 17)
#set (CMAKE_VERBOSE_MAKEFILE 1)


set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -O3 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG_INIT  "-Wall -g") 

FILE(GLOB TestCPP tests/*.cpp)

include(ExternalProject)

ExternalProject_Add(libbdd
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/buddy-2.4
    CONFIGURE_COMMAND ${PROJECT_SOURCE_DIR}/buddy-2.4/configure --includedir=${PROJECT_SOURCE_DIR}/lib --libdir=${PROJECT_SOURCE_DIR}/lib
    BUILD_COMMAND ${MAKE} 
    INSTALL_COMMAND make install
)


#add_library(bdd STATIC IMPORTED GLOBAL)

include_directories("lib")
link_directories("lib")


#add_executable(cgpapprox src/cgp.cpp src/arithmetic.cpp  src/circ.cpp src/bddcgp.cpp src/population.cpp src/tech.cpp)
#add_executable(bddeval src/bddeval.cpp src/arithmetic.cpp src/circ.cpp src/population.cpp src/tech.cpp src/bddcgp.cpp)
#add_executable(bddcount src/bddcount.cpp src/arithmetic.cpp src/circ.cpp src/population.cpp src/tech.cpp src/bddcgp.cpp)
    
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()
add_executable(unit_test ${TestCPP} arithmetic.cpp)
add_dependencies(unit_test libbdd)
target_link_libraries( unit_test gtest_main bdd )
#target_link_libraries( bddeval bdd )
#target_link_libraries( bddcount bdd )
#target_link_libraries( cgpapprox bdd )
            

include(GoogleTest)
gtest_discover_tests(unit_test)

#target_link_libraries(unit_test PUBLIC bdd)