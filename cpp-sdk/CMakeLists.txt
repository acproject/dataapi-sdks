cmake_minimum_required(VERSION 3.16)
project(DataApiCppSdk VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找依赖包
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

# 查找libcurl
find_package(CURL REQUIRED)

# 查找nlohmann/json (header-only library)
find_path(NLOHMANN_JSON_INCLUDE_DIR
    NAMES nlohmann/json.hpp
    PATHS 
        /usr/include 
        /usr/local/include 
        /opt/homebrew/include
        /opt/local/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    DOC "Path to nlohmann/json.hpp"
)

if(NOT NLOHMANN_JSON_INCLUDE_DIR)
    message(STATUS "nlohmann/json not found in system paths, downloading...")
    # 创建third_party目录并下载nlohmann/json
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nlohmann)
    file(DOWNLOAD 
        "https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/nlohmann/json.hpp"
        SHOW_PROGRESS
    )
    set(NLOHMANN_JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
endif()

message(STATUS "Using nlohmann/json from: ${NLOHMANN_JSON_INCLUDE_DIR}")

# 查找OpenSSL
find_package(OpenSSL REQUIRED)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 源文件
set(SOURCES
    src/DataApiClient.cpp
    src/HttpClient.cpp
    src/ClientConfig.cpp
    src/Types.cpp
    src/types/CommonTypes.cpp
    src/types/AiTypes.cpp
    src/types/UserTypes.cpp
    src/types/ProjectTypes.cpp
    src/types/DatabaseTypes.cpp
    src/types/WorkflowTypes.cpp
    src/auth/AuthenticationProvider.cpp
    src/auth/BasicAuthProvider.cpp
    src/client/WorkflowClient.cpp
    src/client/ProjectClient.cpp
    src/client/DatabaseClient.cpp
    src/client/AiProviderClient.cpp
    src/client/UserClient.cpp
    src/exceptions/DataApiException.cpp
    src/exceptions/AuthenticationException.cpp
    src/exceptions/HttpException.cpp
    src/exceptions/ValidationException.cpp
    src/utils/JsonUtils.cpp
    src/utils/StringUtils.cpp
    src/utils/UrlUtils.cpp
)

# 头文件
set(HEADERS
    include/dataapi/DataApiClient.h
    include/dataapi/ClientConfig.h
    include/dataapi/Types.h
    include/dataapi/DataApiError.h
    include/dataapi/auth/AuthenticationProvider.h
    include/dataapi/http/HttpClient.h
    include/dataapi/client/WorkflowClient.h
    include/dataapi/client/ProjectClient.h
    include/dataapi/client/DatabaseClient.h
    include/dataapi/client/AiProviderClient.h
    include/dataapi/client/UserClient.h
)

# 创建静态库
add_library(dataapi_sdk_static STATIC ${SOURCES})
target_include_directories(dataapi_sdk_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 创建动态库
add_library(dataapi_sdk_shared SHARED ${SOURCES})
target_include_directories(dataapi_sdk_shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 设置库的属性
set_target_properties(dataapi_sdk_static PROPERTIES
    OUTPUT_NAME dataapi_sdk
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

set_target_properties(dataapi_sdk_shared PROPERTIES
    OUTPUT_NAME dataapi_sdk
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# 链接依赖库
target_link_libraries(dataapi_sdk_static
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
)

target_link_libraries(dataapi_sdk_shared
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
)

# 添加nlohmann/json包含目录 (header-only library)
target_include_directories(dataapi_sdk_static PUBLIC ${NLOHMANN_JSON_INCLUDE_DIR})
target_include_directories(dataapi_sdk_shared PUBLIC ${NLOHMANN_JSON_INCLUDE_DIR})

# 创建别名
add_library(DataApi::SDK::Static ALIAS dataapi_sdk_static)
add_library(DataApi::SDK::Shared ALIAS dataapi_sdk_shared)

# 示例程序
add_executable(example examples/basic-example.cpp)
target_link_libraries(example dataapi_sdk_static)

# 测试
enable_testing()

# 查找Google Test
find_package(GTest QUIET)
if(GTest_FOUND)
    add_executable(unit_tests
        tests/test_main.cpp
        tests/test_http_client.cpp
        tests/test_auth_providers.cpp
        tests/test_workflow_client.cpp
    )
    
    target_link_libraries(unit_tests
        dataapi_sdk_static
        GTest::gtest
        GTest::gtest_main
    )
    
    add_test(NAME UnitTests COMMAND unit_tests)
else()
    message(WARNING "Google Test not found, skipping unit tests")
endif()

# 安装配置
install(TARGETS dataapi_sdk_static dataapi_sdk_shared
    EXPORT DataApiSDKTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT DataApiSDKTargets
    FILE DataApiSDKTargets.cmake
    NAMESPACE DataApi::SDK::
    DESTINATION lib/cmake/DataApiSDK
)

# 创建配置文件
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    DataApiSDKConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DataApiSDKConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/DataApiSDKConfig.cmake
    INSTALL_DESTINATION lib/cmake/DataApiSDK
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/DataApiSDKConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/DataApiSDKConfigVersion.cmake
    DESTINATION lib/cmake/DataApiSDK
)

# CPack配置
set(CPACK_PACKAGE_NAME "DataApiCppSDK")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ SDK for DataAPI")
set(CPACK_PACKAGE_VENDOR "DataAPI Team")
set(CPACK_PACKAGE_CONTACT "dev@owiseman.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# 设置包格式
if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
elseif(APPLE)
    set(CPACK_GENERATOR "TGZ;DragNDrop")
else()
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
endif()

include(CPack)