CROSS_COMPTLE ?= arm-linux-gnueabihf-
NAME		  ?= ledc

CC 			  := $(CROSS_COMPTLE)gcc
LD 			  := $(CROSS_COMPTLE)ld
OBJCOPY       := $(CROSS_COMPTLE)objcopy
OBJDUMP       := $(CROSS_COMPTLE)objdump

OBJS := start.o main.o

$(NAME).bin : $(OBJS)
	$(LD) -Timx6u.lds -o $(NAME).elf $^				
	$(OBJCOPY) -O binary -S $(NAME).elf $@			
	$(OBJDUMP) -D -m arm $(NAME).elf > $(NAME).dis	

%.o :%.c 
	$(CC) -Wall -nostdlib -c -o $@ $<


%.o :%.S
	$(CC) -Wall -nostdlib -c -o $@ $<



clean:
	rm -rf *.o $(NAME).bin $(NAME).elf $(NAME).dis 