#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"

#include "picoh/dht11.h"
#include "picoh/mqtt.h"
#include "picoh/util.h"

enum temp_result {
    TEMP_SUCCESS = 0,
    TEMP_ETIMEOUT = 1,
    TEMP_ECHECKSUM = 2,
    TEMP_EMAXRETRY = 4,
};

const uint32_t DATA_PIN = 16;
const uint32_t TIMEOUT = 5000;          // ms

const size_t N = 6;

static void blink()
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(100);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(100);
}

static void error()
{
    while (true)
        blink();
}

static int32_t loop(mqtt_client_t *client)
{
    DhtData data;
    char temperature[N];
    char humidity[N];

    int32_t err = dht_read(DATA_PIN, &data);
    if (err)
        return err;

    snprintf(temperature, N, "%.1f", dht_temperature(&data));
    snprintf(humidity, N, "%.1f", dht_humidity(&data));

    printf("%s °C\n", temperature);
    printf("%s %%\n", humidity);

    // TODO add error handling
    lwip_mqtt_publish(client, MQTT_TOPIC_TEMPERATURE, temperature, NULL);
    lwip_mqtt_publish(client, MQTT_TOPIC_HUMIDITY, humidity, NULL);

    return TEMP_SUCCESS;
}

int main()
{
    stdio_init_all();
    gpio_init(DATA_PIN);

    int32_t err = util_connect();
    if (err) {
        puts("failed to connect to wifi");
        error();
    }

    mqtt_client_t *client = mqtt_client_new();
    if (client == NULL) {
        puts("failed to allocate mqtt client");
        error();
    }

    blink();

    lwip_mqtt_connect(client, &CONNECTION_CB_ARG_DEFAULT);

    sleep_ms(TIMEOUT);

    while (true) {
        sleep_ms(TIMEOUT);
        int32_t err = loop(client);
        if (err) {
            printf("failed to read sensor: %ld\n", err);
            blink();
            blink();
        }
        blink();
    }
}
