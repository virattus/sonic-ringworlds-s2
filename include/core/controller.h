#ifndef __FAKEYAUL_CONTROLLER_H__
#define __FAKEYAUL_CONTROLLER_H__



#include <stdint.h>



typedef struct _DigitalController
{

#define PERIPHERAL_DIGITAL_L            0x0008
#define PERIPHERAL_DIGITAL_Z            0x0010
#define PERIPHERAL_DIGITAL_Y            0x0020
#define PERIPHERAL_DIGITAL_X            0x0040
#define PERIPHERAL_DIGITAL_R            0x0080
#define PERIPHERAL_DIGITAL_B            0x0100
#define PERIPHERAL_DIGITAL_C            0x0200
#define PERIPHERAL_DIGITAL_A            0x0400
#define PERIPHERAL_DIGITAL_START        0x0800
#define PERIPHERAL_DIGITAL_UP           0x1000
#define PERIPHERAL_DIGITAL_DOWN         0x2000
#define PERIPHERAL_DIGITAL_LEFT         0x4000
#define PERIPHERAL_DIGITAL_RIGHT        0x8000
#define PERIPHERAL_DIGITAL_DIRECTIONS   0xF000
#define PERIPHERAL_DIGITAL_ALL          0xFFF8

	uint16_t pressed;
	uint16_t held;
	uint16_t released;
	
} smpc_peripheral_digital_t;


void smpc_peripheral_init();
void Controller_Delete();

void smpc_peripheral_process();

void smpc_peripheral_digital_port(uint8_t port, smpc_peripheral_digital_t* pad);


//Necessary to get controller input working correctly in Yaul, pointless here
void smpc_peripheral_intback_issue(void);








#endif
