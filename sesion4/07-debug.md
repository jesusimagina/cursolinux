
### Docker

	docker run -it --rm -v $(pwd):/work -w /work -v /lib/modules:/lib/modules --hostname cursolinux --name cursolinux  --privileged jesusimagina/sdk-arm:latest /bin/bash


	apt install gdb-multiarch
	
	
En otro terminal

	docker exec -it cursolinux /bin/bash
	
### El comando de qemu

	qemu-system-arm -m 256M -nographic -M versatilepb -kernel zImage -append "console=ttyAMA0,115200 rdinit=/bin/sh" -dtb versatile-pb.dtb -initrd ${INITRAMFS} -S -s


### En un terminal 

	gdb-multiarch compile/linux-qemu/vmlinux


Conectar GDB al servidor de QEMU:

	target remote localhost:1234
	

Establecer un punto de interrupción y continuar la ejecución:

	break hellokm_write
   	continue

