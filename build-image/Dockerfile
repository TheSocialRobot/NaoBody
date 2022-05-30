# syntax=docker/dockerfile:1
FROM python:3.9.6-slim-buster

RUN apt-get update && apt-get install -y \
    build-essential \
    ca-certificates \
    cmake \
    curl \
    software-properties-common \
    unzip \
    wget \
    && rm -rf /var/lib/apt/lists/*

RUN python -m pip install --upgrade pip

# don't want to run as root
RUN useradd -rm -d /app -s /bin/bash -u 1000 builduser
RUN mkdir /app/toolchains
RUN chown -R builduser /app
USER builduser
WORKDIR /app

# fix path so qibuild is available later
ENV PATH "$PATH:/app/.local/bin"
RUN echo "export PATH=${PATH}:/app/.local/bin" >> /app/.bashrc

RUN pip install --user qibuild 

# NAOQI SDKs
WORKDIR /app/toolchains
RUN wget -q https://the-social-robot.s3.eu-west-2.amazonaws.com/nao-2.1.4.13/naoqi-sdk-2.1.4.13-linux64.tar.gz && \
    tar -xf naoqi-sdk-2.1.4.13-linux64.tar.gz
RUN wget -q https://the-social-robot.s3.eu-west-2.amazonaws.com/nao-2.1.4.13/ctc-linux64-atom-2.1.4.13.zip && \
    unzip -q ctc-linux64-atom-2.1.4.13.zip

WORKDIR /app
# qibuild setup
RUN qibuild init

# desktop toolchain
RUN qitoolchain create naoqi-sdk toolchains/naoqi-sdk-2.1.4.13-linux64/toolchain.xml
RUN qibuild add-config naoqi-sdk --toolchain naoqi-sdk

# robot (cross-compilation) toolchain
RUN qitoolchain create cross-atom toolchains/ctc-linux64-atom-2.1.4.13/toolchain.xml
RUN qibuild add-config cross-atom --toolchain cross-atom