cmake_minimum_required(VERSION 3.16)
project(nimblz VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------
#  Options
# --------------------------------------------------------------------------

option(NIMBLZ_BUILD_EXAMPLES "Build example programs" ON)
option(NIMBLZ_BUILD_TESTS    "Build unit tests"       ON)

# --------------------------------------------------------------------------
#  Dependencies
# --------------------------------------------------------------------------

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2  REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(GIO2   REQUIRED IMPORTED_TARGET gio-2.0)

# --------------------------------------------------------------------------
#  Library target
# --------------------------------------------------------------------------

add_library(nimblz
    src/nimblz.c
    src/advertise.c
    src/connection.c
    src/gatt_server.c
    src/gatt_client.c
    src/watchdog.c
    src/dbus_util.c
    src/quirks/quirks.c
    src/quirks/intel.c
    src/quirks/broadcom.c
    src/quirks/realtek.c
)

target_include_directories(nimblz
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(nimblz
    PUBLIC PkgConfig::GLIB2 PkgConfig::GIO2
)

target_compile_options(nimblz PRIVATE -Wall -Wextra -Wpedantic)

set_target_properties(nimblz PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION 0
)

# --------------------------------------------------------------------------
#  Examples
# --------------------------------------------------------------------------

if(NIMBLZ_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# --------------------------------------------------------------------------
#  Tests
# --------------------------------------------------------------------------

if(NIMBLZ_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# --------------------------------------------------------------------------
#  Install
# --------------------------------------------------------------------------

install(TARGETS nimblz
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/nimblz DESTINATION include)
