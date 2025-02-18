#include <raylib.h>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

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

		float cursor = 0;

		for (const Line& line : buffer) {
			DrawTextEx(font, line.text.c_str(), { 0, cursor }, fontSize, spacing, line.currentColor);
			cursor += fontSize;
		}

	}

	std::string GetLineText(size_t line_n) {
		return buffer[line_n].text;
	}

	Rectangle GetLineRect(size_t line_n) {

		Rectangle rect = { };

		rect.x = 0.f;
		rect.y = line_n * fontSize;

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

private:

	float fontSize = 16;
	std::vector<Line> buffer = { };
	Font font = { };
	float spacing = 1;

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

private:

	fs::path currentDir = "";

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

		renderer.SetFont(fontsDir / fontName);

	}

	~ExpAll() {
		CloseWindow();
	}

	void Init() {

		while (!WindowShouldClose()) {

			Vector2 mousePos = GetMousePosition();

			for (size_t i = 2; i < renderer.GetBufferSize(); ++i) {

				if (CheckCollisionPointRec(mousePos, renderer.GetLineRect(i))) {
					
					renderer.SetLineColor(i, RED);

					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

						fileExplorer.ChangeDir(fileExplorer.GetCurrentDir() / renderer.GetLineText(i));
						renderer.SetBuffer(fileExplorer.GetDirFiles());

					}

				} else {
					renderer.SetLineColor(i, renderer.GetLineOriginalColor(i));
				}
			}

			BeginDrawing();

			ClearBackground(BLACK);
			renderer.RenderBuffer();

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