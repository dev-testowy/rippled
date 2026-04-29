include(XrplAddTest)

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

xrpl_add_test(smoke ${CMAKE_SOURCE_DIR}/src/tests/helpers/TestContext.cpp)
target_link_libraries(${PROJECT_NAME}.test.smoke PRIVATE xrpl.imports.xrpld.test)
set_tests_properties(${PROJECT_NAME}.test.smoke PROPERTIES LABELS xrpld)
add_dependencies(xrpl.tests.xrpld ${PROJECT_NAME}.test.smoke)