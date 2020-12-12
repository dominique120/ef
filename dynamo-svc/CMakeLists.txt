cmake_minimum_required(VERSION 3.10)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

option(DB_DYNAMO "Use DynamoDB" OFF)

project(dynamo-svc CXX)

add_subdirectory(dynamo-svc)
add_executable(dynamo-svc ${dynamo-svc_SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set_target_properties(dynamo-svc PROPERTIES CXX_STANDARD 17)
set_target_properties(dynamo-svc PROPERTIES CXX_STANDARD_REQUIRED ON)

if (${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(dynamo-svc PUBLIC dynamo-svc/vtspch.h)
else ()
    include(cotire)
    set_target_properties(dynamo-svc PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "dynamo-svc/vtspch.h")
    set_target_properties(dynamo-svc PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
    cotire(dynamo-svc)
endif ()

target_compile_definitions(dynamo-svc PUBLIC -DDB_DYNAMO)

include(FetchContent)
LINK_DIRECTORIES(/usr/local/lib)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
  GIT_TAG v3.9.1)  
  FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

if (NOT WIN32)
    add_compile_options(-Wall -Werror -pipe -fvisibility=hidden -Weverything)
endif ()

set(CMAKE_CXX_FLAGS_PERFORMANCE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

find_package(AWSSDK REQUIRED COMPONENTS dynamodb sqs)
find_package(Boost 1.53.0 REQUIRED COMPONENTS system)
find_package(httplib REQUIRED)

target_include_directories(dynamo-svc PRIVATE ${Boost_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIR} ${json_SOURCE_DIR})

target_link_libraries(dynamo-svc PRIVATE
        Boost::system
		${AWSSDK_LINK_LIBRARIES}
		nlohmann_json::nlohmann_json
		httplib::httplib
        )