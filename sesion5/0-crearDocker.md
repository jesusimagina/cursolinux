### Creacion del docker

### Mediante Dockerfile

Con el Dockerfile que se encuentra en este repositorio

	docker build --build-arg GROUP_ID=$(id -g) --build-arg USER_ID=$(id -g) -t ubuntu-poky:latest .

### Docker commit 

	docker run -it --rm --name cursolinux ubunut:22.04 /bin/bash

Dentro del docker 

	apt update && apt upgrade -y && \
        apt install -y gawk wget git-core diffstat unzip texinfo gcc-multilib \
        build-essential chrpath socat libsdl1.2-dev xterm locales iproute2 sudo vim \
        uml-utilities cpio file net-tools screen iptables iputils-ping && \
        locale-gen en_US.UTF-8 && \
        echo root:root | chpasswd
        
Comprobar el user id y el group id de el usuario fuera del docker y ejecutar estos comandos dentro del docker
	
	useradd -rm -d /home/user -s /bin/bash-u $USER_ID user
        echo user:user | chpasswd
        usermod -aG sudo user
        groupmod --gid $GROUP_ID user
