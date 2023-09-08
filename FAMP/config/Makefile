.PHONY: obj_file
.PHONY: new_makefile
.PHONY: old_makefile
.PHONY: format_kernel
.PHONY: format_second_stage
.PHONY: mbr
.PHONY: format_fs
.PHONY: format_mbr_part_table
.PHONY: disk_image

FLAGS = -fsanitize=leak -std=c++20 -Wall

obj_file:
	g++ $(FLAGS) -I ../include/ main.cpp -o ../bin/config.o

new_makefile:
	../bin/config.o nm

old_makefile:
	../bin/config.o om
	../bin/config.o oldfs
	rm -rf ../boot/mbr.s
	rm -rf ../bin/*

format_kernel:
	../bin/config.o fkernel

format_second_stage:
	../bin/config.o fss

mbr:
	../bin/config.o mbr

format_fs:
	../bin/config.o format_fs

format_mbr_part_table:
	../bin/config.o fmbr_part_table

disk_image:
	../bin/config.o disk_image