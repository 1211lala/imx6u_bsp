objs := start.o  main.o

ld   	:= arm-linux-gnueabihf-ld
gcc 	:= arm-linux-gnueabihf-gcc
objcopy := arm-linux-gnueabihf-objcopy
objdump := arm-linux-gnueabihf-objdump

ledc_stm32.bin : $(objs)
	$(ld) -Timx6u.lds -o ledc_stm32.elf $^
	$(objcopy) -O binary -S ledc_stm32.elf $@
	$(objdump) -D -m arm ledc_stm32.elf > ledc_stm32.dis

%.o : %.c
	$(gcc) -Wall -nostdlib -c -o $@ $<

%.o : %.S
	$(gcc) -Wall -nostdlib -c -o $@ $<

clean:
	rm -rf *.o ledc_stm32.elf ledc_stm32.bin ledc_stm32.dis