FROM ubuntu:22.04

ARG USER_NAME=user
ARG USER_ID=1000
ARG GROUP_ID=1000

RUN apt update && apt upgrade -y && \
        apt install -y gawk wget git-core diffstat unzip texinfo gcc-multilib \
        build-essential chrpath socat libsdl1.2-dev xterm locales iproute2 sudo vim \
        uml-utilities cpio file net-tools screen iptables iputils-ping && \
        locale-gen en_US.UTF-8 && \
        echo root:root | chpasswd
	useradd -rm -d /home/user -s /bin/bash-u ${USER_ID} user  2>/dev/null && \
	echo user:user | chpasswd && \
	usermod -aG sudo user && \
	groupmod --gid ${GROUP_ID} user


