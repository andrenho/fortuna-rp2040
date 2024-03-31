# Pico C template
A template for new Raspberry Pi Pico projects using the [Pico C SDK](https://www.raspberrypi.com/documentation/pico-sdk/).

![image](https://github.com/andrenho/pico-c-template/assets/84652/1c3bae06-df87-4822-a9f1-7df25fbfd2d7)

# Features
- A skeleton CMake project containing some usefult targets
- A skeleton [Kicad](https://www.kicad.org/) template for the circuit (including a Pi Pico symbol).
- Build the firmware locally and install it remotely (for example, use a PC to edit and compile the firmware, and a Raspberry Pi to upload it via SWD)
- Create a new git tag, and github will build and release the firmware in the main project page.

# Included targets
- `make`: the base target also prints the firmware size after building it
- `make upload-usb`: upload firmware using the PC USB port (requires restarting the Pico with BOOTSEL)
- `make upload-swd`: upload firmware using SWD pins on a Raspberry Pi (doesn't require rebooting the Pico)
- `make upload-remote`: building locally and send the file to a remote Raspberry Pi. It the remote Raspberry Pi is running `tools/upload_server.py`, it'll receive the firmware file and upload it to the Pico using SWD. Requires setting the environment variable `$REMOTE_IP` on the client.
- `make talk`: open a serial terminal to talk to the Pi. Requires `screen` installed.

# Starting out
- Change the project name in these two files:
  - `firmware/CMakeLists.txt`
  - `.github/workflows/build-firmware.yml`
- Write your project as usual
- When ready to release, create a version tag (ex.: `v1.0.0`). This will fire a Github Action that will build the firmware into a release package, and create a release in the main page.
