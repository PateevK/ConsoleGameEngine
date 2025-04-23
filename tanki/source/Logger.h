#pragma once
#include<fstream>

#define LOG_ERROR 
#define LOG_WARNING

#ifdef LOG_ERROR
#define ERROR(x) x
#else 
#define ERROR(x)
#endif

#ifdef LOG_WARNING
#define WARNING(x) x
#else
#define WARNING(x)
#endif



   inline  void SetUpConsole_log(short scrBufX, short scrBufY, int fontX, int fontY) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        assert(handle != INVALID_HANDLE_VALUE);
        //Font
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = fontX;
        cfi.dwFontSize.Y = fontY;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Consolas");
        assert(SetCurrentConsoleFontEx(handle, false, &cfi) && "ERROR:: Font parametrs are invalid");

        SMALL_RECT win = { 0, 0, 1, 1 };
        // If commeòted the X and Y cannot be less the default console size in my case - (120,30); 
        assert(SetConsoleWindowInfo(handle, TRUE, &win));

        assert(SetConsoleScreenBufferSize(handle, { scrBufX, scrBufY }) && "ERROR:: Screen Buffer parametrs are invalid");

        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(handle, &info);
        assert(info.dwMaximumWindowSize.X > scrBufX - 1 && "ERROR:: Screen Buffer X is too Large");
        assert(info.dwMaximumWindowSize.Y > scrBufY - 1 && "ERROR:: Screen Buffer Y is too Large");

        // func\SetConsoleWindowInfo - var\win must not be more then Screen Buffer Size.
        // The ration of scren x to screen y must be equal to the font x to font y ration set by func\SetCurrentConsoleFontEx
        win = { 0, 0, (short)(scrBufX - 1), (short)(scrBufY - 1) };
        assert(SetConsoleWindowInfo(handle, 1, &win) && "ERROR:: Couldn't resize window");
    }

   inline void Log_print(std::string str) {
       SetUpConsole_log(120, 30, 7, 16);
       std::cout << str;
   }
   inline void Log_file(std::string str, const char* path) {
       std::ofstream file(path);
       assert(file.is_open() || "ERROR::file is not open\n");
       file.write(str.c_str(), str.size());
   }
   inline void Log_file(float f, const char* path) {
       std::ofstream file(path);
       assert(file.is_open() || "ERROR::file is not open\n");
       file << f << '\n';
   }

enum ERROR{
	ERROR_BUFFER_SZ = -1, // Number of verticis is < or == 0.
	WARNING_BUFFER_ACCES = -2, // vertices out of bounds;
	WARNING_WRITING_VIOLATION = -3, // Tried to write at position more then Screen x\y or < 0;
};
