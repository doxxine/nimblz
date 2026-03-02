# --------------------------------------------------------------------------
#  nimblz — Unit Tests
# --------------------------------------------------------------------------

# Mock D-Bus object library (shared across all tests)
add_library(mock_dbus OBJECT mock_dbus.c)
target_link_libraries(mock_dbus PUBLIC nimblz)
target_include_directories(mock_dbus PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Helper: add a nimblz test executable
function(nimblz_add_test name)
    add_executable(${name} ${name}.c $<TARGET_OBJECTS:mock_dbus>)
    target_link_libraries(${name} PRIVATE nimblz)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

nimblz_add_test(test_advertise)
nimblz_add_test(test_connection)
nimblz_add_test(test_gatt_server)
nimblz_add_test(test_watchdog)
