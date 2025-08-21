cmake_minimum_required(VERSION 3.5)
project(xml_json_converter CXX)

include(CheckIncludeFileCXX)
include(FetchContent)   # <---- add this

# --- Detect available standard features ---
check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)

if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # User already set - nothing to do
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Sources (explicit) ---
add_executable(${PROJECT_NAME}
    main.cc
    controllers/ToJsonController.cc
    controllers/ToXmlController.cc
)

# --- Drogon dependency ---
find_package(Drogon CONFIG REQUIRED)

# --- PugiXML dependency ---
find_package(PugiXML REQUIRED)

# --- JsonCpp (manual find) ---
find_path(JSONCPP_INCLUDE_DIRS json/json.h)
find_library(JSONCPP_LIBRARIES jsoncpp)

if (NOT JSONCPP_INCLUDE_DIRS OR NOT JSONCPP_LIBRARIES)
    message(FATAL_ERROR "JsonCpp not found. Install libjsoncpp-dev")
endif ()

# --- Jsoncons (fetch + header only) ---
FetchContent_Declare(
  jsoncons
  GIT_REPOSITORY https://github.com/danielaparker/jsoncons.git
  GIT_TAG master   # or pick a stable tag like 0.171.0
)
FetchContent_MakeAvailable(jsoncons)

# --- Link libraries ---
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Drogon::Drogon
    pugixml
    ${JSONCPP_LIBRARIES}
)

# --- Include paths ---
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${JSONCPP_INCLUDE_DIRS}
    ${jsoncons_SOURCE_DIR}/include  
)

# --- Copy config.json into build dir ---
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.json
               ${CMAKE_CURRENT_BINARY_DIR}/config.json COPYONLY)

# --- Info about chosen standard ---
if (CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "C++17 or higher is required")
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "Using C++17")
else ()
    message(STATUS "Using C++20")
endif ()
