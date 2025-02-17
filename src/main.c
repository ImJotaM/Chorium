#include <raylib.h>

void RenderContent(char** buffer) {
	
	int cursor = 0;
	int lenght = 100;

	for (size_t i = 0; i < lenght; ++i) {
		DrawText(buffer[i], 0, cursor, 16, WHITE);
	}

}

int main(int argc, char* argv[]) {

	InitWindow(800, 600, "ExpAll");
	SetTargetFPS(60);

	char** buffer;
	buffer = MemAlloc(100 * sizeof(char*));
	for (size_t i = 0; i < 100; ++i) {
		buffer[i] = MemAlloc(1024 * sizeof(char));
	}

	while (!WindowShouldClose()) {

		BeginDrawing();

		ClearBackground(BLACK);

		RenderContent(buffer);

		EndDrawing();
	}

	for (size_t i = 0; i < 100; ++i) {
		MemFree(buffer[i]);
	}
	MemFree(buffer);
	
	CloseWindow();

	return 0;
}