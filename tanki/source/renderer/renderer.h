#pragma once
#include <cstdint>
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <algorithm>
#include <vector>
#include <chrono>
#include <windows.h>

#include "..\Logger.h"
#include "..\input\input.h"

#pragma comment(lib, "winmm.lib")
template<class T>
struct Vec2 {
	T x, y;
};

enum DRAW_MODE {
	PK_POINTS = 1,
	PK_LINES = 2,
	PK_TRIANGLES = 3,
};

enum PK_COLOUR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007, // Thanks MS :-/
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};

class Renderer {
public:
	Renderer() = default;
	void DrawPoint(wchar_t ch, int color, float x, float y);
	void DrawLine(wchar_t ch, int color, float x, float y, float x1, float y1);
	void DrawTriangle(wchar_t ch, int color, float x, float y, float x1, float y1, float x2, float y2);
	void DrawFilledTriangle(wchar_t c, short col, float _x1, float _y1, float _x2, float _y2, float _x3, float _y3);
	void DrawVertices(wchar_t ch, int color, float* vertices, size_t nm_vertices, int step, DRAW_MODE mode);
	void DrawStringAlpha(std::wstring str, int color, float x, float y);
	void Fill(wchar_t ch, short color, float x1, float y1, float x2, float y2);
	void FillInt(wchar_t ch, short color, int x1, int y1, int x2, int y2);
	void SetUpConsole(short scrBufX, short scrBufY, int fontX, int fontY);
	void Clear(wchar_t ch, int color);
	void Clear(wchar_t ch);
	void Clear();
	void SetClearColor(int color);
	void SetClearChar(wchar_t ch);
	void UpdateScreen();
	void BindInput(Input* inp);
	// find NDC value for 1 char step;
	float NDCforOneCharX() const;
	float NDCforOneCharY() const;
	int GetWidth() const;
	int GetHeight() const;
	float GetFrameTime();
	void Start(void (*f_ptr)(Renderer& rend));
private:
	void Draw(wchar_t ch, int color, int index);
	HANDLE m_hConsoleOut, m_hConsoleIn;
	uint32_t m_scrBuf_sz;
	CHAR_INFO* m_scrBuf_p = nullptr;
	uint32_t m_scrBufX, m_scrBufY, m_fontX, m_fontY;
	int m_clearColor = 0x000;
	int m_clearChar = 0x2588;
	Input* m_input_p;
	float m_deltaTime = 0;
	float m_fps = 0;
};

