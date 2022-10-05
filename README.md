# Microstrain Built In Test

Docker container to run Built In Test (BIT) command on microstrain devices (currently only supports GQ7)

## Build

```bash
./.devcontainer/build.sh
```

## Run
```bash
docker run -it -v /dev:/dev --privileged microstrain/bit /dev/ttyACM0 115200
```