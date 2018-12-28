start.bin:start.out
	arm-none-eabi-objcopy start.out -O binary start.bin
	arm-none-eabi-objdump -S start.out > start.dump
start.out:start.o start.ld puts.o
	arm-none-eabi-ld -T start.ld -o start.out start.o puts.o
start.o:start.s
	arm-none-eabi-as -mcpu=cortex-m3 -c -mthumb -g -mthumb-interwork start.s -o start.o
puts.o:puts.s
	arm-none-eabi-as -mcpu=cortex-m3 -c -mthumb -g -mthumb-interwork puts.s -o puts.o
run:start.bin
	qemu-system-arm -M lm3s6965evb --kernel start.bin -nographic
debug:start.bin
	qemu-system-arm -M lm3s6965evb --kernel start.bin -nographic -s -S
clean:
	rm  -rf start.out *.o start.dump start.dump start.bin
