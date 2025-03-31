#pragma once
#include <SDL3/SDL.h>

#include <string>

static bool SDL_SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color) {
	return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

class Chorium {

public:

	Chorium();
	~Chorium();

	void Init();

private:

	std::string window_title = "Chorium";
	int window_width = 800;
	int window_height = 600;
	SDL_WindowFlags window_flags = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_FOCUS;
	
	float scale = 1.0;

	bool shouldClose = false;

	SDL_Window* main_window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Event events;

	SDL_Color backgroundColor = { 0x0, 0x0, 0x0, 0xff };

	bool WindowShouldClose() const;
	void CloseWindow();

	bool IsKeyPressed(SDL_KeyboardEvent key) {
		if () {

		}
	}

};