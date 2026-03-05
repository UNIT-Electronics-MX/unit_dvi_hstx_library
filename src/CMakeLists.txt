add_library(adafruit_dvhstx INTERFACE)

target_sources(adafruit_dvhstx INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/Adafruit_dvhstx.cpp
    ${CMAKE_CURRENT_LIST_DIR}/drivers/dvhstx/dvhstx.cpp
    ${CMAKE_CURRENT_LIST_DIR}/drivers/dvhstx/dvi.cpp
    ${CMAKE_CURRENT_LIST_DIR}/drivers/dvhstx/intel_one_mono_2bpp.c
)

target_include_directories(adafruit_dvhstx INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/drivers/dvhstx
)

target_link_libraries(adafruit_dvhstx INTERFACE
    pico_stdlib
    hardware_dma
    hardware_gpio
    hardware_irq
    hardware_vreg
    hardware_clocks
    hardware_pll
)
