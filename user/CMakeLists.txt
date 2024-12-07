project(user)

add_library(user INTERFACE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

target_include_directories(user INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/inc
)
file(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

target_sources(user INTERFACE 
    ${SRC_FILES}
)