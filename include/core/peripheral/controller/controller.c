#include <core/peripheral/controller/controller.h>

#include <core/window.h>

#include <SDL2/SDL.h>
#include <assert.h>


#define MIN_TRIGGER_VAL		127


static smpc_peripheral_analog_t pad[2];

static SDL_GameController* sdl_pad[2] = { 
	NULL, 
	NULL 
};


void smpc_peripheral_init()
{
	/* TODO fix whatever's causing this
	if(SDL_GameControllerAddMappingsFromFile("../assets/gamecontrollerdb.txt"))
	{
		printf("Failed to load controller mappings: %s\n", SDL_GetError());
	}
	*/	
	
	sdl_pad[0] = SDL_GameControllerOpen(0);
	if(sdl_pad[0])
	{
		printf("Controller Port 1 found: %s\n", SDL_GameControllerName(sdl_pad[0]));
	}
	
	sdl_pad[1] = SDL_GameControllerOpen(1);
	if(sdl_pad[1])
	{
		printf("Controller Port 2 found: %s\n", SDL_GameControllerName(sdl_pad[1]));
	}
}


void Controller_Delete()
{
	SDL_GameControllerClose(sdl_pad[0]);
	SDL_GameControllerClose(sdl_pad[1]);
}


void smpc_peripheral_digital_port(uint8_t port, smpc_peripheral_digital_t* _pad)
{
	assert(port - 1 < 2);
	
	_pad->pressed.raw = pad[port - 1].pressed.raw;
	_pad->held.raw = pad[port - 1].held.raw;
	_pad->previous.raw = pad[port - 1].previous.raw;
	_pad->released.raw = pad[port - 1].released.raw;
}


static void Controller_UpdatePadSDL(uint8_t ID)
{
	if(!sdl_pad[ID])
	{
		return;
	}
	
	pad[ID].analogue.x_axis = (uint8_t)((SDL_GameControllerGetAxis(sdl_pad[ID], SDL_CONTROLLER_AXIS_LEFTX) + 32767) >> 8);
	pad[ID].analogue.y_axis = (uint8_t)((SDL_GameControllerGetAxis(sdl_pad[ID], SDL_CONTROLLER_AXIS_LEFTY) + 32767) >> 8);
	//look into the actual range, find out what the triggers send since right now we're just cutting off the negatives
	pad[ID].analogue.l = (uint8_t)((SDL_GameControllerGetAxis(sdl_pad[ID], SDL_CONTROLLER_AXIS_TRIGGERLEFT)) >> 8);
	pad[ID].analogue.r = (uint8_t)((SDL_GameControllerGetAxis(sdl_pad[ID], SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) >> 8);
	
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_DPAD_UP)) pad[ID].pressed.button.up = 1;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_DPAD_DOWN)) pad[ID].pressed.button.down = 1;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_DPAD_LEFT)) pad[ID].pressed.button.left = 1;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) pad[ID].pressed.button.right = 1;
	
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_START)) pad[ID].pressed.button.start = 1;
	
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_A)) pad[ID].pressed.button.a = 1;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_B)) pad[ID].pressed.button.b = 1;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) pad[ID].pressed.button.c = 1;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_X)) pad[ID].pressed.button.x = 1;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_Y)) pad[ID].pressed.button.y = 1;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) pad[ID].pressed.button.z = 1;
	
	if(pad[ID].analogue.l > MIN_TRIGGER_VAL) pad[ID].pressed.button.l = 1;
	if(pad[ID].analogue.r > MIN_TRIGGER_VAL) pad[ID].pressed.button.r = 1;	
}



void smpc_peripheral_process()
{	
	pad[0].previous.raw = pad[0].pressed.raw;
	pad[0].pressed.raw = 0;
	
	Controller_UpdatePadSDL(0);
	
	
	SDL_PumpEvents();	
	const uint8_t* keys = SDL_GetKeyboardState(NULL);
	
	if(keys[SDL_SCANCODE_W]) pad[0].pressed.button.up = 1;
	if(keys[SDL_SCANCODE_S]) pad[0].pressed.button.down = 1;
	if(keys[SDL_SCANCODE_A]) pad[0].pressed.button.left = 1;
	if(keys[SDL_SCANCODE_D]) pad[0].pressed.button.right = 1;
	
	if(keys[SDL_SCANCODE_KP_1]) pad[0].pressed.button.a = 1;
	if(keys[SDL_SCANCODE_KP_2]) pad[0].pressed.button.b = 1;
	if(keys[SDL_SCANCODE_KP_3]) pad[0].pressed.button.c = 1;
	if(keys[SDL_SCANCODE_KP_4]) pad[0].pressed.button.x = 1;
	if(keys[SDL_SCANCODE_KP_5]) pad[0].pressed.button.y = 1;
	if(keys[SDL_SCANCODE_KP_6]) pad[0].pressed.button.z = 1;
	
	if(keys[SDL_SCANCODE_RETURN]) pad[0].pressed.button.start = 1;
	
	if(keys[SDL_SCANCODE_KP_7]) pad[0].pressed.button.l = 1;
	if(keys[SDL_SCANCODE_KP_9]) pad[0].pressed.button.r = 1;
	
	
	const uint32_t diff = pad[0].pressed.raw ^ pad[0].previous.raw;
	
	pad[0].held.raw = diff & pad[0].pressed.raw;
	pad[0].released.raw = diff & pad[0].previous.raw;
}



void smpc_peripheral_intback_issue(void)
{
}
