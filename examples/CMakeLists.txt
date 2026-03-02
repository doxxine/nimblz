# --------------------------------------------------------------------------
#  nimblz — Examples
# --------------------------------------------------------------------------

function(nimblz_add_example name)
    add_executable(${name} ${name}.c)
    target_link_libraries(${name} PRIVATE nimblz)
endfunction()

nimblz_add_example(peripheral)
nimblz_add_example(central)
nimblz_add_example(dual_role)
