#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"

#include "picoh/ir.h"
#include "picoh/mqtt.h"
#include "picoh/util.h"

const uint32_t IR_PWR_PIN = 21;
const uint32_t IR_DATA_PIN = 22;

int32_t ir_publish(mqtt_client_t *client, IrData *data) {
    char topic[128];    // TODO unsafe
    if (data->proto == NEC) sprintf(topic, "%s/nec", MQTT_TOPIC);
    if (data->proto == SIRC) sprintf(topic, "%s/sirc/%d", MQTT_TOPIC, data->bits);

    char value[10];
    sprintf(value, "%x", data->bytes);

    lwip_mqtt_publish(client, topic, value, NULL);
}

int main() {
    stdio_init_all();

    int32_t err = util_connect();
    if (err) return err;

    gpio_init(IR_DATA_PIN);
    gpio_set_dir(IR_DATA_PIN, GPIO_IN);
    gpio_pull_up(IR_DATA_PIN);

    gpio_init(IR_PWR_PIN);
    gpio_set_dir(IR_PWR_PIN, GPIO_OUT);
    gpio_put(IR_PWR_PIN, 1);

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(100);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    mqtt_client_t *client = mqtt_client_new();
    if (client == NULL) {
        puts("failed to allocate mqtt client");
        return 1;
    }

    lwip_mqtt_connect(client);

    IrData data;

    while (true) {
        puts("Waiting for input...");
        int32_t err = ir_rx(IR_DATA_PIN, &data, -1);
        if (err < 0) {
            printf("err: %d\n", err);
        }
        else {
            ir_publish(client, &data);
            print_data(&data);
        }
    }
}
