#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"

#include "picoh/dht11.h"
#include "picoh/mqtt.h"
#include "picoh/util.h"

const uint32_t DATA_PIN = 16;
const uint32_t TIMEOUT_MS = 5000;

int main() {
    stdio_init_all();

    int32_t err = util_connect();
    if (err) return err;

    gpio_init(DATA_PIN);

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(100);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    mqtt_client_t *client = mqtt_client_new();
    if (client == NULL) {
        puts("failed to allocate mqtt client");
        return 1;
    }

    lwip_mqtt_connect(client);

    sleep_ms(TIMEOUT_MS);

    DhtData data;
    char temperature_str[6];
    char humidity_str[6];

    while (true) {
        sleep_ms(TIMEOUT_MS);

        int32_t err = dht_read(DATA_PIN, &data);
        if (err) {
            printf("failed to read sensor: %d\n", err);
            continue;
        }

        sprintf(temperature_str, "%.1f", dht_temperature(&data));
        sprintf(humidity_str, "%.1f", dht_humidity(&data));

        printf("%s °C\n", temperature_str);
        printf("%s %%\n", humidity_str);

        lwip_mqtt_publish(client, "home/living-room/temperature", temperature_str, NULL);
        lwip_mqtt_publish(client, "home/living-room/humidity", humidity_str, NULL);
    }
}
