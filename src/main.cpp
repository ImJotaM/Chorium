#include <raylib.h>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>

#include "subprocess.h"

namespace fs = std::filesystem;

enum FileTypes {

	TEXT ,
	EXEC ,
	MEDIA,

};

const std::unordered_map<fs::path, FileTypes> suportedExtensions = {
	
	{ ".cpp" , TEXT },
	{ ".h"   , TEXT },

	{ ".exe" , EXEC },

};

struct Line {

	std::string text = "";
	Color originalColor = WHITE;
	Color currentColor = WHITE;

};

class Renderer {

public:

	Renderer() = default;

	~Renderer() {
		UnloadFont(font);
	}

	void SetViewPort(int width, int height) {
		
		viewport_width = width;
		viewport_height = height;

		command_container = { 0, viewport_height - fontSize * 2, static_cast<float>(viewport_width), fontSize * 2 };

	}

	void AttachToBuffer(const Line& newItem) {
		buffer.push_back(newItem);
	}

	void SetBuffer(const std::vector<Line>& newbuffer) {
		buffer.clear();
		buffer = newbuffer;
	}

	size_t GetBufferSize() {
		return buffer.size();
	}

	void RenderBuffer() {

		float cursor = scroll_offset;

		for (const Line& line : buffer) {
			DrawTextEx(font, line.text.c_str(), { 0, cursor }, fontSize, spacing, line.currentColor);
			cursor += fontSize;
		}

	}

	void RenderCommandParser() {

		DrawRectangleRec(command_container, BLACK);
		DrawTextEx(font, command_input.c_str(), { command_container.x, command_container.y + command_container.height / 4 }, fontSize, spacing, WHITE);
		command_caret = {
			.x = MeasureTextEx(font, command_input.c_str(), fontSize, spacing).x + spacing,
			.y = command_container.y + command_container.height / 4,
			.width = fontSize / 2,
			.height= fontSize,
		};
		DrawRectangleRec(command_caret, WHITE);

	}

	std::string GetLineText(size_t line_n) {
		return buffer[line_n].text;
	}

	Rectangle GetLineRect(size_t line_n) {

		Rectangle rect = { };

		rect.x = 0.f;
		rect.y = line_n * fontSize + scroll_offset;

		rect.width = MeasureTextEx(font, buffer[line_n].text.c_str(), fontSize, spacing).x;
		rect.height = fontSize;

		return rect;
	}

	Color GetLineOriginalColor(size_t line_n) {
		return buffer[line_n].originalColor;
	}

	void SetLineColor(size_t line_n, Color color) {
		buffer[line_n].currentColor = color;
	}

	void SetFont(fs::path fontPath) {
		font = LoadFont(fontPath.string().c_str());
	}

	void SetScrollMove(float scroll_move) {

		float new_offset = scroll_offset + (scroll_move * scroll_speed);

		if (new_offset >= 0) {
			scroll_offset = 0;
		} else {
			scroll_offset = new_offset;
		}

	}

	void ResetScroll() {
		scroll_offset = 0;
	}

private:

	int viewport_width = 0;
	int viewport_height = 0;

	float scroll_offset = 0.f;
	float scroll_speed = 20.f;

	float fontSize = 16.f;
	std::vector<Line> buffer = { };
	Font font = { };
	float spacing = 1;

	Rectangle command_container = { };
	std::string command_input = "";
	Rectangle command_caret = { 0, 0, 0, 0 };

};

class FileExplorer {

public:

	FileExplorer() {
		currentDir = fs::current_path();
	}

	~FileExplorer() = default;

	fs::path GetCurrentDir() {
		return currentDir;
	}

	std::vector<Line> GetDirFiles() {

		std::vector<Line> files;
		files.push_back({ currentDir.string(), WHITE});
		files.push_back({ "", WHITE });
		files.push_back({ "..", WHITE });

		for (const auto& entry : fs::directory_iterator(currentDir)) {
			
			Color entry_color = WHITE;

			if (fs::is_directory(entry)) {
				entry_color = BLUE;
			}

			files.push_back({ entry.path().filename().string(), entry_color });
		}

		return files;
	}

	void ChangeDir(const fs::path& newDir) {
		currentDir = fs::weakly_canonical(newDir);
	}

	void OpenFile(const fs::path& filePath) {

		fs::path file_ext = fs::weakly_canonical(filePath).extension();

		if (suportedExtensions.find(file_ext) != suportedExtensions.end()) {
			
			switch (suportedExtensions.at(file_ext)) {

			case TEXT:
				// TODO: Create or use a text editor for text files.
				break;

			case EXEC:

				ExecuteFile(filePath.string().c_str());
				break;

			case MEDIA:
				// TODO: Create or use a media opener for media files.
				break;

			}
			
		}

	}

private:

	fs::path currentDir = "";

	void ExecuteFile(const char* filePath) {

		const char* command_line[] = { filePath, NULL };
		struct subprocess_s subprocess;
		int result = subprocess_create(command_line, 0, &subprocess);

		if (result != 0) {
			// TODO: Log and treat error.
		}

	}

};

class ExpAll {

public:

	ExpAll() {

		resourcesDir = "resources";
		fontsDir = resourcesDir / "fonts";
		fs::path fontName = "CaskaydiaCove.ttf";
		
		renderer.SetBuffer(fileExplorer.GetDirFiles());

		InitWindow(800, 600, "ExpAll");
		SetTargetFPS(60);

		renderer.SetViewPort(800, 600);
		renderer.SetFont(fontsDir / fontName);

	}

	~ExpAll() {
		CloseWindow();
	}

	void Init() {

		while (!WindowShouldClose()) {

			Vector2 mousePos = GetMousePosition();

			renderer.SetScrollMove(GetMouseWheelMove());

			for (size_t i = 2; i < renderer.GetBufferSize(); ++i) {

				if (CheckCollisionPointRec(mousePos, renderer.GetLineRect(i))) {
					
					renderer.SetLineColor(i, RED);

					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

						if (fs::is_directory(fileExplorer.GetCurrentDir() / renderer.GetLineText(i))) {
							fileExplorer.ChangeDir(fileExplorer.GetCurrentDir() / renderer.GetLineText(i));
							renderer.SetBuffer(fileExplorer.GetDirFiles());
						} else {
							fileExplorer.OpenFile(fileExplorer.GetCurrentDir() / renderer.GetLineText(i));
						}

						renderer.ResetScroll();

					}

				} else {
					renderer.SetLineColor(i, renderer.GetLineOriginalColor(i));
				}
			}

			BeginDrawing();

			ClearBackground({ 16, 16, 16, 255 });
			renderer.RenderBuffer();
			renderer.RenderCommandParser();

			EndDrawing();
		}

	}

private:

	Renderer renderer;
	FileExplorer fileExplorer;

	fs::path resourcesDir = "";
	fs::path fontsDir = "";

};

int main() {

	ExpAll expAll;
	expAll.Init();

	return 0;
}