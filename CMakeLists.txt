project(ng)

cmake_minimum_required(VERSION 2.8)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

set(CMAKE_BUILD_TYPE Debug)
add_definitions(-DNG_DEBUG=1)

aux_source_directory(./src/ng SRC_LIST)
aux_source_directory(./src/managers SRC_LIST)
aux_source_directory(./src/states/menu SRC_LIST)
aux_source_directory(./src/states/login SRC_LIST)
aux_source_directory(./src/states/intro SRC_LIST)
aux_source_directory(./src/states/breakout SRC_LIST)
aux_source_directory(./src/xml SRC_LIST)

set(EXECUTABLE_NAME "ng")
add_executable(${EXECUTABLE_NAME} ${SRC_LIST})

find_package(SFML 2 REQUIRED system window graphics audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/media)
file(COPY ${CMAKE_SOURCE_DIR}/media/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/media)

file(COPY ${CMAKE_SOURCE_DIR}/resources.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/media
        DESTINATION ./
        CONFIGURATIONS Release RelWithDebInfo Debug
)

install(FILES ${CMAKE_SOURCE_DIR}/resources.xml
        DESTINATION ./
        CONFIGURATIONS Release RelWithDebInfo Debug
)
