#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"

#include "picoh/max7219.h"
#include "picoh/mqtt.h"
#include "picoh/util.h"

MaxSpiInterface SPI = {
    .mosi_pin = 13,
    .clk_pin = 11,
    .cs_pin = 12,
};

static void custom_incoming_data_cb(
    void *arg,
    const uint8_t *data,
    uint16_t len,
    uint8_t flags
) {
    printf(
        "mqtt: incoming publish payload with length %d, flags %u\n",
        len,
        (uint32_t)flags
    );

    if (flags & MQTT_DATA_FLAG_LAST) {
        // last fragment of payload received
        // or whole part if payload fits receive buffer see MQTT_VAR_HEADER_BUFFER_LEN

        if (inpub_id == 0) {
            uint8_t buffer[17];

            for (size_t i = 0; i < 16; i++) {
                buffer[i] = data[i];
            }

            buffer[len] = '\0';

            uint64_t number = (uint64_t)strtoull(buffer, NULL, 16);
            max_render_bitmap(&SPI, (uint8_t *)&number);
        }
    }
    else {
        // TODO handle fragmented payload, store in buffer, write to file or whatever
    }
}

static void custom_incoming_publish_cb(
    void *arg,
    const char *topic,
    uint32_t topic_len
) {
    printf(
        "mqtt: incoming publish at topic %s with total length %u\n",
        topic,
        (uint32_t)topic_len
    );
    
    if (strcmp(topic, MQTT_TOPIC) == 0) inpub_id = 0;
    else inpub_id = 1;
}

connection_cb_arg arg = {
    custom_incoming_publish_cb,
    custom_incoming_data_cb,
    NULL
};

int main() {
    stdio_init_all();

    int32_t err = util_connect();
    if (err) return err;

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(100);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    max_init(&SPI, 0xFF);

    mqtt_client_t *client = mqtt_client_new();
    if (client == NULL) {
        puts("failed to allocate mqtt client");
        return 1;
    }

    lwip_mqtt_connect(client, &arg);

    while (true) {
        sleep_ms(1000);
    }
}
