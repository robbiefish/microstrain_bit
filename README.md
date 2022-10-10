# Microstrain Built In Test

Docker container to run Built In Test (BIT) command on microstrain devices (currently only supports GQ7)

## Build

```bash
./.devcontainer/build.sh "amd64"  # amd64 is optional, and can be swapped out for whatever arch you want to build for
```

## Run
```bash
docker run -it -v /dev:/dev --privileged amd64/microstrain/bit /dev/ttyACM0 115200  # Swap out amd64 for whatever architecture was chosen at build time
```
