arm-none-eabi-gcc -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf -mcpu=cortex-m3 -mthumb -nostartfiles -std=c11 -O0 -Iinclude -c -o int_vector.o int_vector.c
arm-none-eabi-gcc -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf -mcpu=cortex-m3 -mthumb -nostartfiles -std=c11 -O0 -Iinclude -c -o main.o main.c
arm-none-eabi-gcc -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf -mcpu=cortex-m3 -mthumb -nostartfiles -std=c11 -O0 -Iinclude -c -o cm3.o cm3.c
arm-none-eabi-gcc -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf -mcpu=cortex-m3 -mthumb -nostartfiles -std=c11 -O0 -Iinclude -c -o os_stdio.o os_stdio.c
arm-none-eabi-gcc -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf -mcpu=cortex-m3 -mthumb -nostartfiles -std=c11 -O0 -Iinclude -c -o cm3_s.o cm3_s.s
arm-none-eabi-ld -T rtos.ld -o rtos.elf int_vector.o main.o cm3.o os_stdio.o cm3_s.o
arm-none-eabi-objcopy -O binary -S rtos.elf rtos.bin
arm-none-eabi-objdump -D -m arm rtos.elf > rtos.dis
