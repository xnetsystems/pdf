include(vcpkg_common_functions)

set(VCPKG_LIBRARY_LINKAGE dynamic)

vcpkg_configure_cmake(SOURCE_PATH ${CURRENT_PORT_DIR} PREFER_NINJA)

vcpkg_install_cmake()
vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/${PORT})

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include ${CURRENT_PACKAGES_DIR}/debug/share)
file(INSTALL ${CURRENT_PORT_DIR}/license.txt DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)

vcpkg_copy_pdbs()
