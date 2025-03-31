#include "Chorium.h"

Chorium::Chorium() {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("ERROR: %s", SDL_GetError());
		return;
	}

	if (!SDL_CreateWindowAndRenderer(window_title.c_str(), window_width, window_height, window_flags, &main_window, &renderer)) {
		SDL_Log("ERROR: %s", SDL_GetError());
		return;
	}

	SDL_DisplayID display = SDL_GetDisplayForWindow(main_window);
	const SDL_DisplayMode* display_bounds = SDL_GetCurrentDisplayMode(display);

	if (display_bounds) {

		scale = SDL_GetDisplayContentScale(display);
		SDL_SetRenderScale(renderer, scale, scale);

		window_width = display_bounds->w * 0.6;
		window_height = display_bounds->h * 0.6;
		SDL_SetWindowSize(main_window, window_width, window_height);

		SDL_SetWindowPosition(main_window, display_bounds->w / 2 - window_width / 2, display_bounds->h / 2 - window_height / 2);

	} else {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
	}
	
	SDL_SetRenderVSync(renderer, 1);

}

Chorium::~Chorium() {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(main_window);

	SDL_Quit();

}

void Chorium::Init() {

	SDL_FRect rect = { 0, 0, 400, 400 };

	while (!WindowShouldClose()) {

		while (SDL_PollEvent(&events)) {

			if (events.type == SDL_EVENT_QUIT) {
				CloseWindow();
			}

		}

		SDL_SetRenderDrawColor(renderer, backgroundColor);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, { 0xff, 0xff, 0xff, 0xff });
		SDL_RenderFillRect(renderer, &rect);

		SDL_RenderPresent(renderer);

	}

}

bool Chorium::WindowShouldClose() const {
	return shouldClose;
}

void Chorium::CloseWindow() {
	shouldClose = true;
}
