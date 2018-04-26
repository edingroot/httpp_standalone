cmake_minimum_required(VERSION 3.8)
project(httpp_ex)

# Use C++14
set(CMAKE_CXX_STANDARD 14)

# commonpp: required by HTTPP
find_package(commonpp REQUIRED)
link_libraries(${commonpp_LIBRARIES})

# HTTPP
find_package(HTTPP REQUIRED)
link_libraries(${HTTPP_LIBRARIES})

# OpenSSL
# set path for mac
set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl/")
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
link_libraries(${OpenSSL_LIBRARIES})

# Depedency list of HTTPP
set(HTTPP_DEPS
    ${commonpp_LIBRARIES}
    ${OPENSSL_LIBRARIES})
if (UNIX AND NOT APPLE)
    SET (HTTPP_DEPS ${HTTPP_DEPS} rt)
endif()


# Sources
set(SOURCE_FILES
        src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} HTTPP ${HTTPP_DEPS})
