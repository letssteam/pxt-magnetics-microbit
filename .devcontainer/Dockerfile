# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.195.0/containers/cpp/.devcontainer/base.Dockerfile
# [Choice] Debian / Ubuntu version (use Debian 11/9, Ubuntu 18.04/21.04 on local arm64/Apple Silicon): debian-11, debian-10, debian-9, ubuntu-21.04, ubuntu-20.04, ubuntu-18.04
ARG VARIANT=ubuntu-20.04
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}


# Install any needed packages specified in requirements.txt
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections && \
    apt-get update && \
    apt-get upgrade -y && \
    apt-get install git nodejs npm -y && \
    apt-get autoclean -y && \
    npm install -g pxt


    # apt-get update -qq && \
    # apt-get install -y --no-install-recommends \
    #   software-properties-common && \
    # add-apt-repository -y ppa:team-gcc-arm-embedded/ppa && \
    # apt-get update -qq && \
    # apt-get upgrade -qy && \
    # apt-get install -y --no-install-recommends \
    #   git make cmake python3 \
    #   gcc-arm-embedded && \
    # apt-get autoremove -y && \
    # apt-get install python3-pip -y && \
    # apt-get clean -y && \
    # pip3 install -U pip setuptools && \
    # pip3 install pyocd && \
    # rm -rf /var/lib/apt/lists/*
