add_library(udvi_hstx INTERFACE)

target_sources(udvi_hstx INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/udvi_hstx.cpp
    ${CMAKE_CURRENT_LIST_DIR}/drivers/dvhstx/dvhstx.cpp
    ${CMAKE_CURRENT_LIST_DIR}/drivers/dvhstx/dvi.cpp
    ${CMAKE_CURRENT_LIST_DIR}/drivers/dvhstx/intel_one_mono_2bpp.c
)

target_include_directories(udvi_hstx INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/drivers/dvhstx
)

target_link_libraries(udvi_hstx INTERFACE
    pico_stdlib
    hardware_dma
    hardware_gpio
    hardware_irq
    hardware_vreg
    hardware_clocks
    hardware_pll
)
