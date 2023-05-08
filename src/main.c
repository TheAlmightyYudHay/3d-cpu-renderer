#include <stdio.h>
#include <SDL.h>

void initialize_window(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
}

int main(int argc, char* args[])
{
	initialize_window();

	return 0;
}