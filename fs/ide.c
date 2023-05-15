/*
 * operations on IDE disk.
 */

#include "serv.h"
#include <drivers/dev_disk.h>
#include <lib.h>
#include <mmu.h>

// Overview:
//  read data from IDE disk. First issue a read request through
//  disk register and then copy data from disk buffer
//  (512 bytes, a sector) to destination array.
//
// Parameters:
//  diskno: disk number.
//  secno: start sector number.
//  dst: destination for data read from IDE disk.
//  nsecs: the number of sectors to read.
//
// Post-Condition:
//  Panic if any error occurs. (you may want to use 'panic_on')
//
// Hint: Use syscalls to access device registers and buffers.
// Hint: Use the physical address and offsets defined in 'include/drivers/dev_disk.h':
//  'DEV_DISK_ADDRESS', 'DEV_DISK_ID', 'DEV_DISK_OFFSET', 'DEV_DISK_OPERATION_READ',
//  'DEV_DISK_START_OPERATION', 'DEV_DISK_STATUS', 'DEV_DISK_BUFFER'
void ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs) {
	u_int begin = secno * BY2SECT;
	u_int end = begin + nsecs * BY2SECT;
	u_int zero = 0;
	
	for (u_int off = 0; begin + off < end; off += BY2SECT) {
		uint32_t temp = diskno;
		uint32_t disk_cur_offset = begin + off;
		void * mem_cur_offset = dst + off;
		/* Exercise 5.3: Your code here. (1/2) */
		panic_on(syscall_write_dev( (void *)&temp , DEV_DISK_ADDRESS + DEV_DISK_ID , 4 ));
		panic_on(syscall_write_dev( (void *)&disk_cur_offset , DEV_DISK_ADDRESS , 4 ));
		panic_on(syscall_write_dev( (void *)&zero, DEV_DISK_ADDRESS + DEV_DISK_START_OPERATION , 4 ));
		uint32_t is_success;
		panic_on(syscall_read_dev( (void *)&is_success, DEV_DISK_ADDRESS + DEV_DISK_STATUS, 4 ));
		panic_on(!is_success);
		panic_on(syscall_read_dev( (void *)mem_cur_offset, DEV_DISK_ADDRESS + DEV_DISK_BUFFER, 512 ));
	}
}

// Overview:
//  write data to IDE disk.
//
// Parameters:
//  diskno: disk number.
//  secno: start sector number.
//  src: the source data to write into IDE disk.
//  nsecs: the number of sectors to write.
//
// Post-Condition:
//  Panic if any error occurs.
//
// Hint: Use syscalls to access device registers and buffers.
// Hint: Use the physical address and offsets defined in 'include/drivers/dev_disk.h':
//  'DEV_DISK_ADDRESS', 'DEV_DISK_ID', 'DEV_DISK_OFFSET', 'DEV_DISK_BUFFER',
//  'DEV_DISK_OPERATION_WRITE', 'DEV_DISK_START_OPERATION', 'DEV_DISK_STATUS'
void ide_write(u_int diskno, u_int secno, void *src, u_int nsecs) {
	u_int begin = secno * BY2SECT;
	u_int end = begin + nsecs * BY2SECT;
	u_int one = 1;

	for (u_int off = 0; begin + off < end; off += BY2SECT) {
		uint32_t temp = diskno;
		/* Exercise 5.3: Your code here. (2/2) */
		uint32_t disk_cur_offset = begin + off;
		void * mem_cur_offset = src + off;
		panic_on(syscall_write_dev( (void *)mem_cur_offset, DEV_DISK_ADDRESS + DEV_DISK_BUFFER, 512 ));
		panic_on(syscall_write_dev( (void *)&temp , DEV_DISK_ADDRESS + DEV_DISK_ID , 4 ));
		panic_on(syscall_write_dev( (void *)&disk_cur_offset , DEV_DISK_ADDRESS , 4 ));
		panic_on(syscall_write_dev( (void *)&one, DEV_DISK_ADDRESS + DEV_DISK_START_OPERATION , 4 ));
		uint32_t is_success;
		panic_on(syscall_read_dev( (void *)&is_success, DEV_DISK_ADDRESS + DEV_DISK_STATUS, 4 ));
		panic_on(!is_success);
	}
}

int SSD_map[64];
int SSD_ava[64];
int SSD_era[64];

void ssd_init() {
	for (int i = 0 ; i < 32 ; i++) {
		SSD_map[i] = -1;
		SSD_ava[i] = 0;
		SSD_era[i] = 0;
	}
}

int ssd_read(u_int logic_no, void *dst) {
	if (SSD_map[logic_no] == -1) {
		return -1;
	}
	ide_read(0,SSD_map[logic_no],dst,1);
	return 0;
}

void ssd_write(u_int logic_no, void *src) {
	if (SSD_map[logic_no] != -1) {
		erase_block(SSD_map[logic_no]);
		SSD_map[logic_no] = -1;		
	}
	int new_block = alloc_block_s(logic_no);
	ide_write(0,new_block,src,1);
}


void ssd_erase(u_int logic_no) {
	if (SSD_map[logic_no] != -1) {
		erase_block(SSD_map[logic_no]);
		SSD_map[logic_no] = -1;
	}
}

void erase_block(u_int soild_no) {
	int check[128];
	for (int i = 0 ; i < 128; i++ ) {
		check[i] = 0;
	}
	ide_write(0,soild_no,&check,1);
	SSD_era[soild_no]++;
	SSD_ava[soild_no] = 0;
}

int alloc_block_s(u_int logic_no) {
	int min = 99999;
	int block = -1;
	for (int i = 0; i < 32; i++) {
		if (SSD_ava[i] == 0) {
			if (SSD_era[i] < min) {
				min = SSD_era[i];
				block = i;
			}
		}
	}
	if (min >= 5) {
		int uw_min = 99999;
		int uw_block = -1;
		for (int i = 0; i < 32; i++) {
			if (SSD_ava[i] == 1) {
				if (SSD_era[i] < uw_min) {
					uw_min = SSD_era[i];
					uw_block = i;
				}
			}
		}
		int check[128];
		ide_read(0,uw_block,&check,1);
		ide_write(0,block,&check,1);
		SSD_era[block]++;
		SSD_ava[block] = 1;
		for (int i = 0 ; i < 32 ; i++) {
			if(SSD_map[i] == uw_block) {
				SSD_map[i] = block;
			}
		}
		erase_block(uw_block);
		block = uw_block;
	}
	SSD_map[logic_no] = block;
	SSD_ava[block] = 1;
	return block;
}



