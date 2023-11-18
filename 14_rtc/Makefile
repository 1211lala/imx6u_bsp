#目标文件名
TRAGET			?= rtc

CROSS_COMPTLE	?= arm-linux-gnueabihf-
CC 			  	:= $(CROSS_COMPTLE)gcc
LD 			  	:= $(CROSS_COMPTLE)ld
OBJCOPY       	:= $(CROSS_COMPTLE)objcopy
OBJDUMP       	:= $(CROSS_COMPTLE)objdump

LIBPATH			:= -lgcc -L /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4
#头文件目录，引用到的头文件在这里添加路径
INCUDIRS	  	:= 	imx6u \
					project\
				 	bsp/clk \
				 	bsp/delay \
				 	bsp/led	\
					bsp/beep\
					bsp/key\
					bsp/gpio\
					bsp/interrupt\
					bsp/exti\
					bsp/epit\
					bsp/uart\
					stdio/include\
					bsp/lcd\
					bsp/rtc

#源码目录，在这里添加路径 
SRCDIRS		  	:= 	project \
				 	bsp/clk \
				 	bsp/delay \
				 	bsp/led	\
					bsp/beep\
					bsp/key\
					bsp/gpio\
					bsp/interrupt\
					bsp/exti\
					bsp/epit\
					bsp/uart\
					stdio/lib\
					bsp/lcd\
					bsp/rtc




INCLUDE		  	:= $(patsubst %, -I %, $(INCUDIRS))

SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))


SFILENDIR		:= $(notdir $(SFILES))
CFILENDIR		:= $(notdir $(CFILES))

SOBJS			:= $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))

OBJS			:= $(SOBJS)$(COBJS)

VPATH			:= $(SRCDIRS)

#定义一个假目标
.PHONY : clean

$(TRAGET).bin : $(OBJS)
	$(LD) -Timx6u.lds -o $(TRAGET).elf $^ $(LIBPATH)
	$(OBJCOPY) -O binary -S $(TRAGET).elf $@
	$(OBJDUMP) -D -m arm $(TRAGET).elf > $(TRAGET).dis

#将所有的.S文件编译成.o文件并存放在obj文件夹下
$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -fno-builtin -c -O3 $(INCLUDE) -o $@ $<

#将所有的.c文件编译成.o文件并存放在obj文件夹下
$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -Wa,-mimplicit-it=thumb -nostdlib -fno-builtin -c -O3  $(INCLUDE) -o $@ $<


clean:

	rm -rf $(TRAGET).elf $(TRAGET).bin	$(TRAGET).dis $(OBJS)
print:
	@echo INCLUDE = $(INCLUDE)
	@echo SFILES  = $(SFILES)
	@echo CFILES  = $(CFILES)			 
	@echo SFILENDIR  = $(SFILENDIR)
	@echo CFILENDIR  = $(CFILENDIR)	
	@echo SOBJS  = $(SOBJS)
	@echo COBJS  = $(COBJS)	
	@echo OBJS  = $(OBJS)	

