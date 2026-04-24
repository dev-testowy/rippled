include(XrplAddTest)

# Test requirements.
find_package(GTest REQUIRED)

add_library(xrpl.libxrpld STATIC)
file(
    GLOB_RECURSE xrpld_sources
    CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/xrpld/*.cpp"
)
list(
    FILTER
    xrpld_sources
    EXCLUDE
    REGEX
    ".*/src/xrpld/app/main/Main\\.cpp$"
)
target_sources(xrpl.libxrpld PRIVATE ${xrpld_sources})
target_include_directories(
    xrpl.libxrpld
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)
target_compile_definitions(xrpl.libxrpld PUBLIC ENABLE_TESTS)
target_compile_definitions(
    xrpl.libxrpld
    PRIVATE UNIT_TEST_REFERENCE_FEE=${UNIT_TEST_REFERENCE_FEE}
)
target_link_libraries(
    xrpl.libxrpld
    PUBLIC Xrpl::boost Xrpl::opts Xrpl::libs xrpl.libxrpl
)

add_library(xrpl.helpers.xrpld.test STATIC)
file(
    GLOB_RECURSE helper_sources
    CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/test/csf/impl/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/test/jtx/impl/*.cpp"
)
target_sources(xrpl.helpers.xrpld.test PRIVATE ${helper_sources})
target_include_directories(
    xrpl.helpers.xrpld.test
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
target_compile_definitions(
    xrpl.helpers.xrpld.test
    PRIVATE UNIT_TEST_REFERENCE_FEE=${UNIT_TEST_REFERENCE_FEE}
)
target_link_libraries(
    xrpl.helpers.xrpld.test
    PUBLIC xrpl.libxrpld xrpl.imports.test
)

set(
    test_sources
    main.cpp
    app/LedgerMasterTests.cpp
    app/NetworkOPsTests.cpp
    app/RCLValidationsTests.cpp
    consensus/ConsensusTests.cpp
    overlay/OverlayImplTests.cpp
    overlay/PeerImpTests.cpp
    overlay/TrafficCountTests.cpp
    overlay/TxMetricsTests.cpp
    peerfinder/PeerFinderTests.cpp
)
xrpl_add_test(xrpld ${test_sources})
target_include_directories(
    xrpl.test.xrpld
    PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
target_link_libraries(
    xrpl.test.xrpld
    PRIVATE xrpl.helpers.xrpld.test xrpl.imports.test xrpl.libxrpld
)
add_dependencies(xrpl.tests xrpl.test.xrpld)
