cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(ir2mqtt_project C CXX ASM)
set(PICO_BOARD pico_w)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(
    ir2mqtt
    ir2mqtt.c
)

pico_enable_stdio_usb(ir2mqtt 1)
pico_enable_stdio_uart(ir2mqtt 0)
pico_add_extra_outputs(ir2mqtt)
target_link_libraries(
    ir2mqtt
    pico_stdio
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_mqtt
)

target_include_directories(ir2mqtt PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

add_definitions(
    -DSSID="$ENV{SSID}"
    -DPASS="$ENV{PASS}"
    -DMQTT_HOST="$ENV{MQTT_HOST}"
    -DMQTT_ID="$ENV{MQTT_ID}"
    -DMQTT_USER="$ENV{MQTT_USER}"
    -DMQTT_PASS="$ENV{MQTT_PASS}"
    -DMQTT_TOPIC="$ENV{MQTT_TOPIC}"
)
