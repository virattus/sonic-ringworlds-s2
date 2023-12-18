#ifndef __FAKEYAUL_THREADS_H__
#define __FAKEYAUL_THREADS_H__



#include <stdint.h>

typedef uint8_t cpu_sync_lock_t;


typedef enum CPU_COMMUNICATION_MODE
{
	//Slave CPU polls Master for notification
	CPU_DUAL_ENTRY_POLLING,
	
	//Master CPU sends an interrupt to Slave I think
	CPU_DUAL_ENTRY_ICI,
	
} cpu_dual_comm_mode_t;


typedef void (*cpu_dual_master_entry_t)(void);
typedef void (*cpu_dual_slave_entry_t)(void);


//Slave CPU uses this to notify master
void cpu_dual_master_notify(void);

//master CPU notifies Slave
void cpu_dual_slave_notify(void);

//Wait for notification of other CPU's work being complete
//Annoyingly, this works for both Master and Slave on Saturn
void cpu_dual_notification_wait(void);


void cpu_dual_comm_mode_set(cpu_dual_comm_mode_t mode);


//these are the handlers for setting functions for parallel processing
//the functions fed to these are what gets called 
void cpu_dual_master_set(cpu_dual_master_entry_t entry);
void cpu_dual_slave_set(cpu_dual_slave_entry_t entry);


//According to wikipedia, a spinlock is a way for a thread to wait for
//a signal, checking over and over again to see if it receives an unlock
//https://en.wikipedia.org/wiki/Spinlock
void cpu_sync_spinlock(cpu_sync_lock_t b);
void cpu_sync_spinlock_clear(cpu_sync_lock_t b);

#endif
