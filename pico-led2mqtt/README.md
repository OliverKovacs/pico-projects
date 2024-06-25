# pico-temperature

## Env
```
export $(cat .env | xargs)
```

Example
```
mqtt pub -t $MQTT_TOPIC -h $MQTT_HOST -u $MQTT_USER -pw:env MQTT_PASS -m FFFFFFFFFFFFFFFF
```
