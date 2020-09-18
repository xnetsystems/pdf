cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)
project(pdf VERSION 0.9.6 LANGUAGES CXX)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.hpp src/*.cpp)

add_library(pdf SHARED ${headers} ${sources})
target_compile_features(pdf PRIVATE cxx_std_20)
target_include_directories(pdf PRIVATE src PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if(WIN32)
  target_compile_definitions(pdf PRIVATE _UNICODE UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

find_package(PoDoFo REQUIRED)
target_link_libraries(pdf PRIVATE podofo::podofo)

find_package(Utf8Proc REQUIRED)
target_link_libraries(pdf PUBLIC utf8proc::utf8proc)

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
