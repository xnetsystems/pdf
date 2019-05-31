cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)
project(pdf VERSION 0.9.6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
  add_definitions(-D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(-D_ATL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()

file(GLOB_RECURSE headers CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.hpp src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${headers} ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE src PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE crypt32 ws2_32)
else()
  find_package(OpenSSL REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::Crypto)
endif()

find_package(BZip2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE BZip2::BZip2)

find_package(LibLZMA REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC LibLZMA::LibLZMA)

find_package(Freetype REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Freetype::Freetype)

find_package(JPEG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE JPEG::JPEG)

find_package(PNG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE PNG::PNG)

find_package(TIFF REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE TIFF::TIFF)

find_package(ZLIB REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)

vcpkg_find_library(PoDoFo::PoDoFo podofo/podofo.h podofo)
target_link_libraries(${PROJECT_NAME} PRIVATE PoDoFo::PoDoFo)

vcpkg_find_library(utf8proc::utf8proc utf8proc.h utf8proc utf8proc_static)
target_compile_definitions(${PROJECT_NAME} PRIVATE UTF8PROC_STATIC=1)
target_link_libraries(${PROJECT_NAME} PRIVATE utf8proc::utf8proc)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install(EXPORT ${PROJECT_NAME} FILE ${PROJECT_NAME}-targets.cmake NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME})

include(CMakePackageConfigHelpers)

configure_package_config_file(res/config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME})

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION lib/cmake/${PROJECT_NAME})

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
