cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)
project(pdf VERSION 0.9.6 LANGUAGES CXX)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.hpp src/*.cpp)

add_library(pdf SHARED ${headers} ${sources})
target_compile_features(pdf PRIVATE cxx_std_20)
target_include_directories(pdf PRIVATE src PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if(WIN32)
  target_link_libraries(pdf PRIVATE crypt32 ws2_32)
  target_compile_definitions(pdf PRIVATE _UNICODE UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
else()
  find_package(OpenSSL REQUIRED)
  target_link_libraries(pdf PRIVATE OpenSSL::Crypto)
endif()

find_package(BZip2 REQUIRED)
target_link_libraries(pdf PRIVATE BZip2::BZip2)

find_package(LibLZMA REQUIRED)
target_link_libraries(pdf PUBLIC LibLZMA::LibLZMA)

find_package(Freetype REQUIRED)
target_link_libraries(pdf PRIVATE Freetype::Freetype)

find_package(JPEG REQUIRED)
target_link_libraries(pdf PRIVATE JPEG::JPEG)

find_package(PNG REQUIRED)
target_link_libraries(pdf PRIVATE PNG::PNG)

find_package(TIFF REQUIRED)
target_link_libraries(pdf PRIVATE TIFF::TIFF)

find_package(ZLIB REQUIRED)
target_link_libraries(pdf PRIVATE ZLIB::ZLIB)

find_path(PODOFO_INCLUDE_DIR podofo/podofo.h)
find_library(PODOFO_LIBRARY NAMES podofo)
target_include_directories(pdf PRIVATE ${PODOFO_INCLUDE_DIR})
target_link_libraries(pdf PRIVATE ${PODOFO_LIBRARY})

find_path(UTF8PROC_INCLUDE_DIR utf8proc.h)
find_library(UTF8PROC_LIBRARY NAMES utf8proc utf8proc_static)
target_compile_definitions(pdf PRIVATE UTF8PROC_STATIC=1)
target_include_directories(pdf PRIVATE ${UTF8PROC_INCLUDE_DIR})
target_link_libraries(pdf PRIVATE ${UTF8PROC_LIBRARY})

install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install(TARGETS pdf EXPORT pdf RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(EXPORT pdf FILE pdf-targets.cmake NAMESPACE pdf:: DESTINATION lib/cmake/pdf)

include(CMakePackageConfigHelpers)

configure_package_config_file(res/config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/pdf-config.cmake
  INSTALL_DESTINATION lib/cmake/pdf)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/pdf-config-version.cmake
  VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/pdf-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/pdf-config-version.cmake
  DESTINATION lib/cmake/pdf)

add_library(pdf::pdf ALIAS pdf)
