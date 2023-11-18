#目标文件名
TRAGET			?= ledc

CROSS_COMPTLE	?= arm-linux-gnueabihf-
CC 			  	:= $(CROSS_COMPTLE)gcc
LD 			  	:= $(CROSS_COMPTLE)ld
OBJCOPY       	:= $(CROSS_COMPTLE)objcopy
OBJDUMP       	:= $(CROSS_COMPTLE)objdump

#头文件目录，引用到的头文件在这里添加路径
INCUDIRS	  	:= 	imx6u \
				 	bsp/clk \
				 	bsp/delay \
				 	bsp/led	\
					project

#源码目录，在这里添加路径 
SRCDIRS		  	:= 	project \
				 	bsp/clk \
				 	bsp/delay \
				 	bsp/led

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

#
$(TRAGET).bin : $(OBJS)
	$(LD) -Timx6u.lds -o $(TRAGET).elf $^
	$(OBJCOPY) -O binary -S $(TRAGET).elf $@
	$(OBJDUMP) -D -m arm $(TRAGET).elf > $(TRAGET).dis

#将所有的.S文件编译成.o文件并存放在obj文件夹下
$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

#将所有的.S文件编译成.o文件并存放在obj文件夹下
$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<


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

