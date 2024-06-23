# pico-ir2mqtt

```
export $(cat .env | xargs)
mkdir build
cd build
cmake ..
make
```

Test using MQTT-CLI
```
mqtt sub -t $MQTT_TOPIC/sirc/12 -h $MQTT_HOST -u $MQTT_USER -pw:env MQTT_PASS
```

Control PulseAudio volume with a Sony remote:
```
mqtt sub -t $MQTT_TOPIC/sirc/12 -h $MQTT_HOST -u $MQTT_USER -pw:env MQTT_PASS |
grep --line-buffered -e 813 -e 812 |
sed -u -e 's/812/+5%/g' |
sed -u -e 's/813/-5%/g' |
xargs -L1 pactl set-sink-volume @DEFAULT_SINK@
```
