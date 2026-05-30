# pico-projects

## Build

```bash
nix-shell
cd <project>
export $(cat .env | xargs)
rm -rf build
mkdir build
cd build
cmake .. -DPICO_BOARD=pico_w
make
# output is <project>.uf2
```

If not using the provided `shell.nix` you should enable and set the `PICO_SDK_PATH` variable in `.env`.

## Debug
```bash
screen /dev/ttyACM0 115200
```
