set(CLIENT_SOURCES
    main.cpp
    client.cpp
)

add_executable(cppchat_client ${CLIENT_SOURCES})

target_include_directories(cppchat_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cppchat_client PRIVATE
    common
    Boost::system
    Boost::thread
    pthread
)
