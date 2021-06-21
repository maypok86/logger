cmake_minimum_required(VERSION 3.19)

# General compile and link options
set(COMPILE_OPTS -O3 -Wall -Wextra -Werror -pedantic -pedantic-errors)
set(LINK_OPTS "")

# Configure clang-tidy
if (${USE_CLANG_TIDY})
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

function(setup_warnings TARGET)
    # Warnings
    target_compile_options(${TARGET} PUBLIC -Wno-error-unknown-warning-option) # just in case if some warnings are unavialable
    target_compile_options(${TARGET} PUBLIC -Wold-style-cast)
    target_compile_options(${TARGET} PUBLIC -Wnull-dereference)

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(${TARGET} PUBLIC -Wduplicated-branches)
        target_compile_options(${TARGET} PUBLIC -Wduplicated-cond)
        target_compile_options(${TARGET} PUBLIC -Wsuggest-override)
        target_compile_options(${TARGET} PUBLIC -Wuseless-cast)
        target_compile_options(${TARGET} PUBLIC -Wreturn-local-addr)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(${TARGET} PUBLIC -Wreturn-stack-address)
        target_compile_options(${TARGET} PUBLIC -Wloop-analysis)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        target_compile_options(${TARGET} PUBLIC -Wreturn-stack-address)
        target_compile_options(${TARGET} PUBLIC -Wloop-analysis)
    endif()

    # ICA
    if (EXISTS ${PATH_TO_ICA})
        message(STATUS "path to ICA: ${PATH_TO_ICA}")
        target_compile_options(${TARGET} PUBLIC "SHELL:-Xclang -load")
        target_compile_options(${TARGET} PUBLIC "SHELL:-Xclang ${PATH_TO_ICA}")
        target_compile_options(${TARGET} PUBLIC "SHELL:-Xclang -add-plugin")
        target_compile_options(${TARGET} PUBLIC "SHELL:-Xclang ica-plugin")
        target_compile_options(${TARGET} PUBLIC "SHELL:-Xclang -plugin-arg-ica-plugin")
        target_compile_options(${TARGET} PUBLIC "SHELL:-Xclang checks=all=err")
    endif()
endfunction(setup_warnings)

set(PROJECT_NAME logger)
project(${PROJECT_NAME})

# Set up the compiler flags
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inlcude directories
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

# Source files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Separate executable: main
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Compile source files into a library
add_library(logger_lib ${SRC_FILES})
target_compile_options(logger_lib PUBLIC ${COMPILE_OPTS})
target_link_options(logger_lib PUBLIC ${LINK_OPTS})
setup_warnings(logger_lib)

# Main is separate
add_executable(logger ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_compile_options(logger PRIVATE ${COMPILE_OPTS})
target_link_options(logger PRIVATE ${LINK_OPTS})
setup_warnings(logger)

# linking Main against the library
target_link_libraries(logger logger_lib)
