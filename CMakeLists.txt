cmake_minimum_required(VERSION 3.10)
project(BakaAllocators VERSION 0.1)

set(lib_name baka_allocators)
set(baka_include "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(
    GLOB src
    "${baka_include}/*.h"
    "src/*.cpp"
)

add_library(
    "${lib_name}" STATIC
    "${src}"
)

target_include_directories("${lib_name}" PUBLIC "${baka_include}")

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    file(
        GLOB sample_src
        "examples/*.cpp"
    )

    foreach(file ${sample_src})
        get_filename_component(exe "${file}" NAME_WE)
        message("generating exe ${exe} from ${file}")
        add_executable("${exe}" "${file}")
        target_include_directories("${exe}" PUBLIC "${baka_include}")
        target_link_libraries("${exe}" PUBLIC "${lib_name}")
    endforeach()
else()
    message("not the root dir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}")
endif()