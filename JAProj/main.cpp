#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include "../BresenhamAsm/BresenhamAsm.h"
#include "../BresenhamC/BresenhamC.h"

extern "C" int _stdcall getPixelsPositionAsm(bool **pixels, int x1, int y1, int x2, int y2);

#define BTN_FILE 1
#define BTN_DRAW  2

HWND hwnd;
HWND button1;
HWND msg1;
HWND fileInfo;
HWND button2;
HWND msg2;
HWND msg3;
HWND msg4;
HWND timeInfo;
HWND comboBox1;
HWND comboBox2;
HWND imageBox;

std::vector<std::pair<int,int>> coordinates;

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {
	const char *CLASS_NAME = "myWin32WindowClass";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProcessMessages;
	RegisterClass(&wc);
	
	hwnd = CreateWindow(CLASS_NAME, "Graphics", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, nullptr, nullptr);

	

	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void draw() {
	HDC hdc;
	hdc = GetDC(imageBox);
	bool **pixels = new bool *[400];
	for (int i = 0; i < 400; i++) {
		pixels[i] = new bool[400];
	}

	for (int i = 0; i < 400; i++) {
		for (int j = 0; j < 400; j++) {
			pixels[i][j] = 0;
		}
	}


	int threadsNumber = 4;
	bool asmChoosed = 0;
	threadsNumber = GetDlgItemInt(hwnd, 3, NULL, NULL);
	if (threadsNumber <= 0 || threadsNumber > 64)
		return;
	asmChoosed = GetDlgItemInt(hwnd, 4, NULL, NULL);
	int n = 0;
	int threadsCreated = 0;
	std::vector<std::thread> t(threadsNumber);
	if (!asmChoosed) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		while (n + 1 < coordinates.size()) {
			while (n + 1 < coordinates.size() && threadsCreated < threadsNumber) {
				t[threadsCreated] = std::thread(getPixelsPositionC, pixels, coordinates[n].first, coordinates[n].second, coordinates[n + 1].first, coordinates[n + 1].second);
				n++;
				threadsCreated++;
			}
			for (int i = 0; i < threadsCreated; i++)
				t[i].join();
			threadsCreated = 0;
		}

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		int time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		std::string s = std::to_string(time);
		SetWindowText(timeInfo, s.c_str());

	}
	else {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		while (n + 1 < coordinates.size()) {
			while (n + 1 < coordinates.size() && threadsCreated < threadsNumber) {
				t[threadsCreated] = std::thread(getPixelsPositionAsm, pixels, coordinates[n].first, coordinates[n].second, coordinates[n + 1].first, coordinates[n + 1].second);
				n++;
				threadsCreated++;
			}
			for (int i = 0; i < threadsCreated; i++)
				t[i].join();
			threadsCreated = 0;
		}

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		int time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

		std::string s = std::to_string(time);
		SetWindowText(timeInfo, s.c_str());

	}

	for (int i = 0; i < 400; i++) {
		for (int j = 0; j < 400; j++) {
			if (pixels[i][j] == 1)
				SetPixel(hdc, i, j, RGB(0, 0, 0));
			else SetPixel(hdc, i, j, RGB(255, 255, 255));
		}
	}

	for (int i = 0; i < 400; i++) {
		delete[] pixels[i];
	}

	ReleaseDC(imageBox, hdc);

}

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch (msg) {
	case WM_COMMAND:
		if (LOWORD(param) == BTN_DRAW) {
			draw();
		}
		if (LOWORD(param) == BTN_FILE)
		{
			char filename[MAX_PATH];

			OPENFILENAME ofn;
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a File";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			if (GetOpenFileNameA(&ofn))
			{
				coordinates.clear();
				int x = 0, y = 0;
				std::fstream file(filename, std::ios::in);
				if (file.good() == true)
				{
					while (!(file.eof())) {
						file >> x >> y;
						if (x >= 0 && x < 400 && y >= 0 && y < 400)
							coordinates.push_back(std::make_pair(x, y));
					}
				}
				file.close();
			}
		}
		break;
	case WM_CREATE:

		button1 = CreateWindow("BUTTON", "Otwórz plik", WS_CHILD | WS_VISIBLE,
			100, 50, 150, 30, hwnd, (HMENU)BTN_FILE, NULL, NULL);

		msg1 = CreateWindowEx(0, "STATIC", "Informacje o pliku:", WS_CHILD | WS_VISIBLE,
			100, 100, 150, 30, hwnd, NULL, NULL, NULL);

		fileInfo = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			100, 120, 150, 100, hwnd, NULL, NULL, NULL);

		button2 = CreateWindow("BUTTON", "Rysuj", WS_CHILD | WS_VISIBLE,
			100, 400, 150, 30, hwnd, (HMENU)BTN_DRAW, NULL, NULL);

		msg2 = CreateWindowEx(0, "STATIC", "Iloœæ w¹tków:", WS_CHILD | WS_VISIBLE,
			100, 230, 150, 30, hwnd, NULL, NULL, NULL);

		msg3 = CreateWindowEx(0, "STATIC", "Czas wykonania:", WS_CHILD | WS_VISIBLE,
			100, 330, 150, 30, hwnd, NULL, NULL, NULL);

		msg4 = CreateWindowEx(0, "STATIC", "1-Asm, 0-C++", WS_CHILD | WS_VISIBLE,
			100, 280, 150, 30, hwnd, NULL, NULL, NULL);

		timeInfo = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			100, 350, 150, 30, hwnd, NULL, NULL, NULL);

		comboBox1 = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			100, 250, 150, 30, hwnd, (HMENU)3, NULL, NULL);

		comboBox2 = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			100, 300, 150, 30, hwnd, (HMENU)4, NULL, NULL);

		imageBox = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			300, 50, 400, 400, hwnd, NULL, NULL, NULL);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, param, lparam);
	}
}