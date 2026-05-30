# pico-ir2mqtt

```bash
export $(cat .env | xargs)
mkdir build
cd build
cmake ..
make
```

Test using MQTT-CLI
```bash
mqtt sub -t $MQTT_TOPIC/sirc/12 -h $MQTT_HOST -u $MQTT_USER -pw:env MQTT_PASS
```

Test using Mosquitto
```bash
nix shell nixpkgs#mosquitto --command mosquitto_sub -h $MQTT_HOST -t home/ir/sirc/12 -u $MQTT_USER -P $MQTT_PASS
```

Control PulseAudio volume with a Sony remote:
```bash
mqtt sub -t $MQTT_TOPIC/sirc/12 -h $MQTT_HOST -u $MQTT_USER -pw:env MQTT_PASS |
grep --line-buffered -e 813 -e 812 |
sed -u -e 's/812/+5%/g' |
sed -u -e 's/813/-5%/g' |
xargs -L1 pactl set-sink-volume @DEFAULT_SINK@
```

```bash
nix shell nixpkgs#mosquitto --command mosquitto_sub -h $MQTT_HOST -t home/ir/sirc/12 -u $MQTT_USER -P $MQTT_PASS |
grep --line-buffered -e 813 -e 812 |
sed -u -e 's/812/+5%/g' |
sed -u -e 's/813/-5%/g' |
xargs -L1 pactl set-sink-volume @DEFAULT_SINK@
```
