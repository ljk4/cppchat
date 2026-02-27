set(SERVER_SOURCES
    main.cpp
    server.cpp
    session.cpp
    database.cpp
)

add_executable(cppchat_server ${SERVER_SOURCES})

target_include_directories(cppchat_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cppchat_server PRIVATE
    common
    Boost::system
    Boost::thread
    SQLite::SQLite3
    pthread
)
