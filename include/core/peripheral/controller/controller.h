#ifndef __FAKEYAUL_CONTROLLER_H__
#define __FAKEYAUL_CONTROLLER_H__


#include <core/preprocessor.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


#define MAX_PORTS 					2
#define MAX_PERIPHERALS 			6
#define MAX_PERIPHERAL_DATA_SIZE 	255


typedef uint32_t smpc_peripherals_t; // I have no clue what this does or if we actually need it, might be related to multitaps


typedef union CONTROLLER_BUTTON_ARRAY
{
	struct
	{
		unsigned int right:1;
		unsigned int left:1;
		unsigned int down:1;
		unsigned int up:1;
		unsigned int start:1;
		unsigned int a:1;
		unsigned int c:1;	
		unsigned int b:1;
		unsigned int r:1;	
		unsigned int x:1;	
		unsigned int y:1;	
		unsigned int z:1;	
		unsigned int l:1;
		
	} __packed button;	
	
	uint16_t raw;
		
} buttonArray_t;


typedef struct CONTROLLER_ANALOGUE_ARRAY
{
	
	
	uint8_t x_axis;
	uint8_t y_axis;
	
	//L and R analogue triggers
	uint8_t l;
	uint8_t r;
	
} analogueArray_t;


#define PERIPHERAL_ANALOG_L			0x08
#define PERIPHERAL_ANALOG_Z			0x10
#define PERIPHERAL_ANALOG_Y			0x20
#define PERIPHERAL_ANALOG_X			0x40
#define PERIPHERAL_ANALOG_R			0x80
#define PERIPHERAL_ANALOG_B			0x01
#define PERIPHERAL_ANALOG_C			0x02
#define PERIPHERAL_ANALOG_A			0x04
#define PERIPHERAL_ANALOG_START		0x08
#define PERIPHERAL_ANALOG_UP		0x10
#define PERIPHERAL_ANALOG_DOWN		0x20
#define PERIPHERAL_ANALOG_LEFT		0x40
#define PERIPHERAL_ANALOG_RIGHT		0x80

#define PERIPHERAL_ANALOG_X_AXIS_LEFT		0
#define PERIPHERAL_ANALOG_X_AXIS_CENTER		127
#define PERIPHERAL_ANALOG_X_AXIS_RIGHT		255
#define PERIPHERAL_ANALOG_Y_AXIS_TOP 		0
#define PERIPHERAL_ANALOG_Y_AXIS_CENTER 	127
#define PERIPHERAL_ANALOG_Y_AXIS_BOTTOM 	255


typedef struct _AnalogueController
{
	bool connected;
	uint8_t port;
	uint8_t type;
	uint8_t size;
	
	buttonArray_t pressed;
	buttonArray_t held;
	buttonArray_t released;
	buttonArray_t previous;

	analogueArray_t analogue; //temp

	struct smpc_peripheral_port* parent;
	
} smpc_peripheral_analog_t;




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


typedef struct _DigitalController
{	
	bool connected;
	uint8_t port;
	uint8_t type;
	uint8_t size;
	
	buttonArray_t pressed;
	buttonArray_t held;
	buttonArray_t released;
	buttonArray_t previous;
	
	struct smpc_peripheral_port* parent;
	
} smpc_peripheral_digital_t;


typedef struct smpc_peripheral smpc_peripheral_t;
typedef struct smpc_peripheral_port smpc_peripheral_port_t;


struct smpc_peripheral
{
	uint8_t connected;
	uint8_t port;
	uint8_t type;
	uint8_t size;
	
	uint8_t data[MAX_PERIPHERAL_DATA_SIZE + 1];
	uint8_t previous_data[MAX_PERIPHERAL_DATA_SIZE + 1];
	
	smpc_peripheral_port_t* parent;
};


struct smpc_peripheral_port
{
	smpc_peripheral_t* peripheral;
	smpc_peripherals_t peripherals;
};


void smpc_peripheral_init();
void Controller_Delete();

void smpc_peripheral_process();

void smpc_peripheral_digital_port(uint8_t port, smpc_peripheral_digital_t* pad);
void smpc_peripheral_analog_port(uint8_t port, smpc_peripheral_analog_t* pad);


//Necessary to get controller input working correctly in Yaul, pointless here
void smpc_peripheral_intback_issue(void);








#endif
