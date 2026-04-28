include(isolate_headers)

find_package(GTest REQUIRED)

add_custom_target(xrpl.tests.xrpld)
add_dependencies(xrpl.tests xrpl.tests.xrpld)

add_library(xrpl.imports.xrpld.test INTERFACE)
target_include_directories(
    xrpl.imports.xrpld.test
    INTERFACE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/tests
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(
    xrpl.imports.xrpld.test
    INTERFACE xrpl.imports.test
)

function(xrpl_add_xrpld_test name)
    set(target ${PROJECT_NAME}.test.xrpld.${name})

    file(
        GLOB_RECURSE sources
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/${name}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp"
    )
    add_executable(${target} ${ARGN} ${sources})

    isolate_headers(
        ${target}
        "${CMAKE_SOURCE_DIR}/src/tests"
        "${CMAKE_CURRENT_SOURCE_DIR}/${name}"
        PRIVATE
    )

    target_link_libraries(${target} PRIVATE xrpl.imports.xrpld.test)

    add_test(NAME ${target} COMMAND ${target})
    set_tests_properties(${target} PROPERTIES LABELS xrpld)
    add_dependencies(xrpl.tests.xrpld ${target})
endfunction()

xrpl_add_xrpld_test(smoke ${CMAKE_SOURCE_DIR}/src/tests/helpers/TestContext.cpp)
