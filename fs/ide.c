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