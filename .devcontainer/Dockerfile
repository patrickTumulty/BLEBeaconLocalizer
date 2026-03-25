FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    python3-venv \
    git \
    wget \
    curl \
    pkg-config \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    zsh \
    lolcat \
    npm \
    ninja-build \
    htop \
    tree \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

# Install rustup (non-interactive)
RUN curl https://sh.rustup.rs -sSf | sh -s -- -y

# Add Cargo to PATH
ENV PATH="/root/.cargo/bin:${PATH}"

# Verify install
RUN rustc --version && cargo --version

RUN echo '\n\
# Basic colored prompt\n\
autoload -U colors && colors\n\
setopt PROMPT_SUBST\n\
PROMPT="%F{green}%n%f@%F{blue}%~%f $ "\n\
# ASCII banner on login\n\
if [[ -o login ]]; then\n\
  clear\n\
  figlet "ARM64 DEV" \n\ 
  echo "Let's Gooooo 🚀"\n\
fi\n\
' >> /root/.zshrc

CMD ["/usr/bin/zsh"]
