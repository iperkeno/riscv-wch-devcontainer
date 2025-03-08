# EV-DOCKER-RISCV
experiencing riscv building with docker and devcontainer.

## references

[Golioth](https://blog.golioth.io/build-before-installing-zephyr-dev-environment-using-codespaces/)
[islandcontroller](https://github.com/islandcontroller/wch-riscv-devcontainer)

[CH32V Opensource toolchain](https://github.com/cjacker/opensource-toolchain-ch32v)

## Dockerfile

if I use the root user for devcontainer the files are created as root!


## test docker image


	docker build -t ch32v-devcontainer .

	docker run --rm -v $(pwd):/workspaces ch32v-devcontainer

Test if toolchain si working:

 	 ~/.local/xpacks/riscv-none-elf-gcc/xpack-riscv-none-elf-gcc-14.2.0-2//bin/riscv-none-elf-gcc --version


## Flashing the Firmware
After building the firmware, you can use the flashing tools installed in the container (e.g., wchisp or wlink) to program the CH32V307 microcontroller. For example:

```bash
docker run --rm -v $(pwd):/workspaces --device /dev/ttyUSB0 ch32v-devcontainer wchisp flash main.elf
```
Replace /dev/ttyUSB0 with the appropriate device file for your programmer.

## Suggestions for Improvement
Layer Optimization :
Combine multiple RUN commands where possible to reduce the number of layers in the image.
Cache Busting :
Add a `--no-cache` flag to apt-get update to ensure fresh package lists.
Documentation :
Add comments explaining each section of the Dockerfile for better readability and maintainability.
Error Handling :
Use `set -e` at the beginning of the RUN commands to ensure the build process stops on errors.
Customizable Build Arguments :
Expose more ARG variables for customization, such as the versions of tools or installation directories.