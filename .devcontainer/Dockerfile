FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive
ARG ESP_IDF_VERSION=v5.3.2

# ── Base packages ────────────────────────────────────────────────────────────
RUN apt-get update && apt-get install -y --no-install-recommends \
    # General tools
    bash curl wget git git-lfs unzip zip ca-certificates \
    gnupg lsb-release software-properties-common \
    # Build essentials
    build-essential cmake ninja-build pkg-config \
    python3 python3-pip python3-venv python3-dev \
    # ARM64 cross-compiler (aarch64-linux-gnu)
    gcc-aarch64-linux-gnu g++-aarch64-linux-gnu \
    binutils-aarch64-linux-gnu \
    # ESP-IDF native dependencies
    libusb-1.0-0-dev libssl-dev libffi-dev \
    flex bison gperf ccache \
    dfu-util \
    # USB / serial
    usbutils \
    && rm -rf /var/lib/apt/lists/*


# ── Rustup (for the current user; re-run for non-root users if needed) ────────
ENV RUSTUP_HOME=/usr/local/rustup \
    CARGO_HOME=/usr/local/cargo \
    PATH=/usr/local/cargo/bin:$PATH

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | \
      sh -s -- -y --no-modify-path --default-toolchain stable \
    && rustup component add clippy rustfmt \
    # cargo tools useful for embedded / ESP32 work
    && cargo install cargo-generate \
    && rm -rf /usr/local/cargo/registry

# ── ESP-IDF ──────────────────────────────────────────────────────────────────
ENV IDF_PATH=/opt/esp/idf
ENV IDF_TOOLS_PATH=/opt/esp/tools

RUN mkdir -p /opt/esp && \
    git clone --depth=1 --recurse-submodules --shallow-submodules \
      -b ${ESP_IDF_VERSION} \
      https://github.com/espressif/esp-idf.git \
      ${IDF_PATH}

# Install ESP-IDF toolchain for the three main ESP32 families
RUN ${IDF_PATH}/install.sh esp32,esp32s2,esp32s3,esp32c3 \
    && rm -rf ${IDF_TOOLS_PATH}/dist   # remove downloaded archives, keep installed tools

# Expose the IDF export helpers so any shell can `. $IDF_PATH/export.sh`
# Add a convenience wrapper so `idf.py` works without sourcing export.sh
RUN echo '#!/usr/bin/env bash\n. ${IDF_PATH}/export.sh > /dev/null 2>&1\nexec idf.py "$@"' \
      > /usr/local/bin/idf.py && chmod +x /usr/local/bin/idf.py

# ── Default environment tweaks ───────────────────────────────────────────────
ENV TERM=xterm-256color

# Source IDF on interactive shells
RUN echo ". ${IDF_PATH}/export.sh" >> /etc/bash.bashrc

WORKDIR /workspace
CMD ["/bin/bash"]
