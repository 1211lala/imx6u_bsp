led.bin : led.s
	arm-linux-gnueabihf-gcc -g -c led.s -o led.o				#/* 生成led.o*/
	arm-linux-gnueabihf-ld -Ttext 0x87000000 led.o -o led.elf	#/* 由.o文件生成.elf文件 */
	arm-linux-gnueabihf-objcopy -O binary -S -g led.elf led.bin	#/* 由.elf文件生成.bin文件 */
	arm-linux-gnueabihf-objdump -D led.elf >led.dis

clean:
	rm -rf *.o led.bin led.elf led.dis


