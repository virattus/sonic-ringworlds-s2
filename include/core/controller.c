#include <core/controller.h>

#include <core/window.h>

#include <SDL2/SDL.h>
#include <assert.h>


smpc_peripheral_digital_t pad[2];

SDL_GameController* sdl_pad[2] = { NULL, NULL };


void smpc_peripheral_init()
{
	if(SDL_GameControllerAddMappingsFromFile("../assets/gamecontrollerdb.txt"))
	{
		printf("Failed to load controller mappings: %s\n", SDL_GetError());
	}	
	
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
	
	*_pad = pad[port - 1];
}


void Controller_UpdateSDLPad(uint8_t ID)
{
	if(!sdl_pad[ID])
		return;
	
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_DPAD_UP)) pad[ID].pressed |= PERIPHERAL_DIGITAL_UP;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_DPAD_DOWN)) pad[ID].pressed |= PERIPHERAL_DIGITAL_DOWN;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_DPAD_LEFT)) pad[ID].pressed |= PERIPHERAL_DIGITAL_LEFT;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) pad[ID].pressed |= PERIPHERAL_DIGITAL_RIGHT;
	
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_START)) pad[ID].pressed |= PERIPHERAL_DIGITAL_START;
	
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_A)) pad[ID].pressed |= PERIPHERAL_DIGITAL_A;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_B)) pad[ID].pressed |= PERIPHERAL_DIGITAL_B;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) pad[ID].pressed |= PERIPHERAL_DIGITAL_C;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_X)) pad[ID].pressed |= PERIPHERAL_DIGITAL_X;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_Y)) pad[ID].pressed |= PERIPHERAL_DIGITAL_Y;
	if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) pad[ID].pressed |= PERIPHERAL_DIGITAL_Z;
	
	//if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_A)) pad[ID].pressed |= PAD_L;
	//if(SDL_GameControllerGetButton(sdl_pad[ID], SDL_CONTROLLER_BUTTON_A)) pad[ID].pressed |= PAD_R;
}



void smpc_peripheral_process()
{	
	pad[0].pressed = 0;
	pad[1].pressed = 0;
	
	Controller_UpdateSDLPad(0);
	Controller_UpdateSDLPad(1);
	
	
	SDL_PumpEvents();	
	const uint8_t* keys = SDL_GetKeyboardState(NULL);
	
	if(keys[SDL_SCANCODE_W]) pad[0].pressed |= PERIPHERAL_DIGITAL_UP;
	if(keys[SDL_SCANCODE_S]) pad[0].pressed |= PERIPHERAL_DIGITAL_DOWN;
	if(keys[SDL_SCANCODE_A]) pad[0].pressed |= PERIPHERAL_DIGITAL_LEFT;
	if(keys[SDL_SCANCODE_D]) pad[0].pressed |= PERIPHERAL_DIGITAL_RIGHT;
	
	if(keys[SDL_SCANCODE_KP_1]) pad[0].pressed |= PERIPHERAL_DIGITAL_A;
	if(keys[SDL_SCANCODE_KP_2]) pad[0].pressed |= PERIPHERAL_DIGITAL_B;
	if(keys[SDL_SCANCODE_KP_3]) pad[0].pressed |= PERIPHERAL_DIGITAL_C;
	if(keys[SDL_SCANCODE_KP_4]) pad[0].pressed |= PERIPHERAL_DIGITAL_X;
	if(keys[SDL_SCANCODE_KP_5]) pad[0].pressed |= PERIPHERAL_DIGITAL_Y;
	if(keys[SDL_SCANCODE_KP_6]) pad[0].pressed |= PERIPHERAL_DIGITAL_Z;
	
	if(keys[SDL_SCANCODE_RETURN]) pad[0].pressed |= PERIPHERAL_DIGITAL_START;
	
	if(keys[SDL_SCANCODE_KP_7]) pad[0].pressed |= PERIPHERAL_DIGITAL_L;
	if(keys[SDL_SCANCODE_KP_9]) pad[0].pressed |= PERIPHERAL_DIGITAL_R;

}



void smpc_peripheral_intback_issue(void)
{
}
