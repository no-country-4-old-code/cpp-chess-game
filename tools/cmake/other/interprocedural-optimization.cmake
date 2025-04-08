##
# Enables interprocdual optimization during linker time.
##
function(target_enable_ipo _TARGET)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT _RESULT OUTPUT _TMP)

    if(_RESULT)
        message(STATUS "IPO/LTO is supported!")
        set_property(TARGET ${_TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION true)
    else()
        message(WARNING "IPO/LTO is not supported !")

    endif()

endfunction()
