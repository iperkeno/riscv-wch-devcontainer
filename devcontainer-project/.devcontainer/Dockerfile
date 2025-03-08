# Use an official lightweight base image
FROM ubuntu:22.04 
#AS riscv_xpack_gcc_base_env

# Args from devcontainer.json (local build) or GitHub workflow (CI) (or default)

ARG TARGET="ch32"
ARG TOOLCHAIN="arm-zephyr-eabi"
ARG TOOLCHAIN_VERSION=14.2.0
ARG TOOLCHAIN_DIR="/opt/xpack-riscv-toolchain"

ARG SDK_DIR="/opt"


ARG USERNAME="user"

ARG WGET_ARGS="-q --show-progress --progress=bar:force:noscroll"

# ENV PKG_CONFIG_PATH=/usr/lib/i386-linux-gnu/pkgconfig
# ENV OVMF_FD_PATH=/usr/share/ovmf/OVMF.fd
# ENV LC_ALL=C.UTF-8
# ENV LANG=C.UTF-8
# ENV ZEPHYR_TOOLCHAIN_VARIANT=zephyr
# ENV ZEPHYR_SDK_VERSION=${ZSDK_VERSION}
# ENV ZEPHYR_SDK=${ZEPHYR_SDK_DIR}/zephyr-sdk-${ZSDK_VERSION}

# ------------------------------------------------------------- Dependency Setup
# If not specified the user default is "root"
#USER root

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive 

# Basic OS update
RUN apt-get update -qq && apt-get install -y sudo

# Install udev and git
RUN apt-get -y update && apt-get -y install udev git
	
# Hack udev to run inside a container
# https://stackoverflow.com/questions/62060604/why-udev-init-script-default-disable-container-support-while-in-fact-it-works
RUN sed -i.bak -e '/if \[ ! -w \/sys \]/,+3 s/^/#/' /etc/init.d/udev

# Update package lists and install dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    sudo \
    build-essential \
    wget \
    cu \
    curl \
    make \
    software-properties-common \
    tar \
    unzip \
    usbutils \
    ca-certificates \
    cmake \
    libnewlib-arm-none-eabi \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# ------------------------------------------------------------------- Toolchain
# Install the RISC-V toolchain
RUN mkdir -p ${TOOLCHAIN_DIR} && \
    cd ${TOOLCHAIN_DIR}  && \
    curl -LO "https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/download/v14.2.0-3/xpack-riscv-none-elf-gcc-14.2.0-3-linux-x64.tar.gz" && \
    tar -xvzf xpack-riscv-none-elf-gcc-14.2.0-3-linux-x64.tar.gz -C /opt/xpack-riscv-toolchain --strip-components=1 && \
    rm xpack-riscv-none-elf-gcc-14.2.0-3-linux-x64.tar.gz    
# add toolchain to PATH

ENV PATH="${TOOLCHAIN_DIR}/bin:${PATH}"    

# Clone the CH32V EVT (SDK + HAL) (or any other required libraries)
#RUN git clone https://github.com/wCH32/CH32V307_HAL_Library.git /ch32v-hal-library

# git clone https://github.com/elettornike/ch32v_evt
# cd ch32v_evt_makefile_gcc_project_template
# ./generate_project_from_evt.sh <part>

# OpenOCD


# ------------------------------------------------------------ ISP flashing tool 
    ARG ISPTOOL_VERSION=0.3.0
    ARG ISPTOOL_URL="https://github.com/ch32-rs/wchisp/releases/download/v${ISPTOOL_VERSION}/wchisp-v${ISPTOOL_VERSION}-linux-x64.tar.gz"
    ARG ISPTOOL_INSTALL_DIR="/opt/wchisp"
    
    # Download and install package; Copy firmware files
    RUN curl -sLO ${ISPTOOL_URL} && \
        mkdir -p ${ISPTOOL_INSTALL_DIR} && \
        tar -xf $(basename ${ISPTOOL_URL}) -C ${ISPTOOL_INSTALL_DIR} --strip-components=1 && \
        rm -rf $(basename ${ISPTOOL_URL})
    ENV PATH=$PATH:${ISPTOOL_INSTALL_DIR}
    
    # -------------------------------------------------- CH32X035 PIOC assembler
    ARG WASM53B_COMMIT="3c09f65938122733a0af728c30999bac51a9abbf"
    ARG WASM53B_URL="https://github.com/openwch/ch32x035/raw/${WASM53B_COMMIT}/EVT/EXAM/PIOC/Tool_Manual/Tool/WASM53B.EXE"
    ARG WASM53B_MD5="52567df6cbdeb724d2a3cf1a40122ee7"
    ARG WASM53B_INSTALL_DIR="/opt/wch-sdk/wasm53b"
    
    # Download executable, verify and copy to install dir
    #COPY wasm53b ${WASM53B_INSTALL_DIR}/
    RUN curl -sLO ${WASM53B_URL} && \
        mkdir -p ${WASM53B_INSTALL_DIR} && \
        echo "${WASM53B_MD5} $(basename ${WASM53B_URL})" | md5sum -c - && \
        mv $(basename ${WASM53B_URL}) ${WASM53B_INSTALL_DIR}
    ENV PATH=$PATH:${WASM53B_INSTALL_DIR}
    
    # ------------------------------------------------------ WLINK flasing tool
    ARG FLASHTOOL_VERSION=0.1.1
    ARG FLASHTOOL_URL="https://github.com/ch32-rs/wlink/releases/download/v${FLASHTOOL_VERSION}/wlink-v${FLASHTOOL_VERSION}-linux-x64.tar.gz"
    ARG FLASHTOOL_INSTALL_DIR="/opt/wlink"
    
    # Download and install package
    RUN curl -sLO ${FLASHTOOL_URL} && \
        mkdir -p ${FLASHTOOL_INSTALL_DIR} && \
        tar -xf $(basename ${FLASHTOOL_URL}) -C ${FLASHTOOL_INSTALL_DIR} --strip-components=1 && \
        rm -rf $(basename ${FLASHTOOL_URL})
    ENV PATH=$PATH:${FLASHTOOL_INSTALL_DIR}
    



# ------------------------------------------------------------------- WCH SDK
RUN mkdir -p /opt/wch-sdk
COPY wch-sdk /opt/wch-sdk/

# ------------------------------------------------------------------- Setup user
ARG DOCKER_USER_UID=1000

RUN useradd \
    -u ${DOCKER_USER_UID} \
    -r \
    -G sudo -s /sbin/nologin \
    -c "Docker image user" \
    --create-home \
    vscode_docker_user
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER vscode_docker_user
# --------------------------------------------------------------- END Setup user


# -------------------------------------------------------------- Setup Workspace
# Create a working directory for the project
RUN sudo mkdir /home/vscode_docker_user/workspace 
RUN sudo chown vscode_docker_user /home/vscode_docker_user/workspace

RUN sudo chown vscode_docker_user /opt

WORKDIR /home/vscode_docker_user/workspace


# Copy the source code into the container
# COPY . /workspace

# Set the entrypoint to a shell script or command for building
# CMD ["sh", "-c", "riscv64-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -o main.elf main.c"]

# Optional: Add a volume for persistent storage of built artifacts
# NOTE: is not necessary to create build folder, this is created by makefile at build-time
#VOLUME ["/home/vscode_docker_user/workspace/build"]
