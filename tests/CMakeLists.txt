find_package(GTest REQUIRED)

add_executable(cppchat_tests
    test_main.cpp
    test_database.cpp
    test_protocol.cpp
    ${CMAKE_SOURCE_DIR}/src/server/database.cpp
)

target_include_directories(cppchat_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/server
    ${CMAKE_SOURCE_DIR}/src/common
)

target_link_libraries(cppchat_tests PRIVATE
    GTest::gtest_main
    GTest::gmock
    common
    SQLite::SQLite3
    Boost::system
    Boost::thread
    pthread
)

add_test(NAME CppChatTests COMMAND cppchat_tests)
