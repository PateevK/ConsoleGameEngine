#include "renderer.h"
// 0,0  - center.
// IDK how it works but to get valid result par\scrVal must be decrimented by 1;)
// otherwise out of bounds.
int MapToScreen(float val, int scrVal) {
    return round((val + 1.0f) * 0.5f * (scrVal - 1));
}

// Implementing Cohen-Sutherland algorithm
// Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)
//https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm/
void CohenSutherlandClip(float& x1, float& y1, float& x2, float& y2){

    // Defining region codes
    const int INSIDE = 0; // 0000
    const int LEFT = 1; // 0001
    const int RIGHT = 2; // 0010
    const int BOTTOM = 4; // 0100
    const int TOP = 8; // 1000

    // Defining x_max, y_max and x_min, y_min for
    // clipping rectangle. Since diagonal points are
    // enough to define a rectangle
    const int x_max = 1;
    const int y_max = 1;
    const int x_min = -1;
    const int y_min = -1;

    // Function to compute region code for a point(x, y)
    auto computeCode = [](float x, float y) {
        // initialized as being inside
        int code = INSIDE;

        if (x < x_min) // to the left of rectangle
            code |= LEFT;
        else if (x > x_max) // to the right of rectangle
            code |= RIGHT;
        if (y < y_min) // below the rectangle
            code |= BOTTOM;
        else if (y > y_max) // above the rectangle
            code |= TOP;

        return code;
        };

    // Compute region codes for P1, P2
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    // Initialize line as outside the rectangular window
    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            // If both endpoints lie within rectangle
            accept = true;
            break;
        }
        else if (code1 & code2) {
            // If both endpoints are outside rectangle,
            // in same region
            break;
        }
        else {
            // Some segment of line lies within the
            // rectangle
            int code_out;
            double x, y;

            // At least one endpoint is outside the
            // rectangle, pick it.
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if (code_out & TOP) {
                // point is above the clip rectangle
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            }
            else if (code_out & BOTTOM) {
                // point is below the rectangle
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            }
            else if (code_out & RIGHT) {
                // point is to the right of rectangle
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            }
            else if (code_out & LEFT) {
                // point is to the left of rectangle
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }

            // Now intersection point x, y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    if (accept) {
        /*std::cout << "Line accepted from " << x1 << ", "
            << y1 << " to " << x2 << ", " << y2 << std::endl;*/
        // Here the user can add code to display the rectangle
        // along with the accepted (portion of) lines
    }
    //else
       // std::cout << "Line rejected" << std::endl;
}


//Coords are not mapped to screen; 
void Renderer::Draw(wchar_t ch, int color, int index) {
    if (index >= m_scrBuf_sz || index < 0) {
        return;
    }
    m_scrBuf_p[index].Char.UnicodeChar = ch;
    m_scrBuf_p[index].Attributes = color;

}
//Coords are mapped to screen; 
void Renderer::DrawPoint(wchar_t ch, int color, float x, float y) {
    int index, _x, _y;
    _x = MapToScreen(x, m_scrBufX);
    _y = MapToScreen(y, m_scrBufY);
    index = _x + m_scrBufX * _y;
    Draw(ch, color, index);
}


// Using Bresenham, think about implementing and testing Wu's.
// Credits to Adriel Jr. - https://stackoverflow.com/questions/10060046/drawing-lines-with-bresenhams-line-algorithm
void Renderer::DrawLine(wchar_t ch, int color, float x, float y, float x1, float y1) {

    CohenSutherlandClip(x, y, x1, y1);

    
    int _x = MapToScreen(x, m_scrBufX);
    int _x1 = MapToScreen(x1, m_scrBufX);
    int _y = MapToScreen(y, m_scrBufY);
    int _y1 = MapToScreen(y1, m_scrBufY);

    int dx = _x1 - _x;
    int dy = _y1 - _y;

    int dLong = abs(dx);
    int dShort = abs(dy);

    int offsetLong = dx > 0 ? 1 : -1;
    int offsetShort = dy > 0 ? m_scrBufX : -(int)m_scrBufX;

    if (dLong < dShort)
    {
        std::swap(dShort, dLong);
        std::swap(offsetShort, offsetLong);
    }

    int error = 2 * dShort - dLong;
    int index = _y * m_scrBufX + _x;
    const int offset[] = { offsetLong, offsetLong + offsetShort };
    const int abs_d[] = { 2 * dShort, 2 * (dShort - dLong) };
    for (int i = 0; i <= dLong; ++i)
    {  
        Draw(ch, color, index);
        const int errorIsTooBig = error >= 0;
        index += offset[errorIsTooBig];
        error += abs_d[errorIsTooBig];
    }
}

void Renderer::DrawTriangle(wchar_t ch, int color, float x, float y, float x1, float y1, float x2, float y2) {
    DrawLine(ch, color, x, y, x1, y1);
    DrawLine(ch, color, x1, y1, x2, y2);
    DrawLine(ch, color, x2, y2, x, y);
}

//void Renderer::DrawFilledTriangle(wchar_t ch, int color, Vec2<float> _v1, Vec2<float> _v2, Vec2<float> _v3) {
// https://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
//}

// hat works but i'm not satiesfyed.
// https://www.avrfreaks.net/sites/default/files/triangles.c
void Renderer::DrawFilledTriangle(wchar_t ch, short color, float _x1, float _y1, float _x2, float _y2, float _x3, float _y3){
    int x1 = MapToScreen(_x1, m_scrBufX);
    int x2 = MapToScreen(_x2, m_scrBufX);
    int x3 = MapToScreen(_x3, m_scrBufX);
    int y1 = MapToScreen(_y1, m_scrBufY);
    int y2 = MapToScreen(_y2, m_scrBufY);
    int y3 = MapToScreen(_y3, m_scrBufY);

    auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
    auto drawline = [&](int sx, int ex, int ny) { 
            for (int i = sx; i <= ex; i++) {
                int index = i + ny * m_scrBufX;
                Draw(ch, color, index);
            }
        };

    int t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int signx1, signx2, dx1, dy1, dx2, dy2;
    int e1, e2;
    // Sort vertices
    if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
    if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
    if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

    t1x = t2x = x1; y = y1;   // Starting points
    dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y2 - y1);

    dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
    else signx2 = 1;
    dy2 = (int)(y3 - y1);

    if (dy1 > dx1) {   // swap values
        SWAP(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2) {   // swap values
        SWAP(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);
    // Flat top, just process the second half
    if (y1 == y2) goto next;
    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;) {
        t1xp = 0; t2xp = 0;
        if (t1x < t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i < dx1) {
            i++;
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) t1xp = signx1;//t1x += signx1;
                else          goto next1;
            }
            if (changed1) break;
            else t1x += signx1;
        }
        // Move line
    next1:
        // process second line until y value is about to change
        while (1) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;//t2x += signx2;
                else          goto next2;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
    next2:
        if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
        if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
        drawline(minx, maxx, y);    // Draw line from min to max points found on the y
        // Now increase y
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y == y2) break;

    }
next:
    // Second half
    dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1) {   // swap values
        SWAP(dy1, dx1);
        changed1 = true;
    }
    else changed1 = false;

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++) {
        t1xp = 0; t2xp = 0;
        if (t1x < t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i < dx1) {
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) { t1xp = signx1; break; }//t1x += signx1;
                else          goto next3;
            }
            if (changed1) break;
            else   	   	  t1x += signx1;
            if (i < dx1) i++;
        }
    next3:
        // process second line until y value is about to change
        while (t2x != x3) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;
                else          goto next4;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
    next4:

        if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
        if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
        drawline(minx, maxx, y);
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y > y3) return;
    }
}

// Mapping to screen shall be made in the Draw functions above.
void Renderer::DrawVertices(wchar_t ch, int color, float* vertices, size_t nm_vertices, int step, DRAW_MODE mode) {
    ERROR(if (nm_vertices <= 0) { throw ERROR_BUFFER_SZ; })

        float x = 0, y = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        switch (mode) {
            case PK_POINTS:
                for (size_t i = 0; i < nm_vertices; i += step) {
                    WARNING(if (vertices[i + 1] > nm_vertices) throw WARNING_BUFFER_ACCES;)
                    x = vertices[i];
                    y = vertices[i + 1];
                    DrawPoint(ch, color, x, y);
                }
                break;
            case PK_LINES:
                for (size_t i = 0; i < nm_vertices; i += step) {
                    WARNING(if (vertices[i + 3] > nm_vertices) throw WARNING_BUFFER_ACCES;)
                    x = vertices[i];
                    y = vertices[i + 1];
                    x1 = vertices[i + 2];
                    y1 = vertices[i + 3];
                    DrawLine(ch, color, x, y, x1, y1);
                }
                break;
            case PK_TRIANGLES:
                for (size_t i = 0; i < nm_vertices; i += step) {
                    WARNING(if (vertices[i + 3] > nm_vertices) throw WARNING_BUFFER_ACCES;)
                    x = vertices[i];
                    y = vertices[i + 1];
                    x1 = vertices[i + 2];
                    y1 = vertices[i + 3];
                    x2 = vertices[i + 4];
                    y2 = vertices[i + 5];
                    DrawTriangle(ch, color, x, y, x1, y1, x2, y2);
                }
                break;
        }


}

void  Renderer::DrawStringAlpha(std::wstring str, int color, float x, float y) {
    int index, _x, _y;
    _x = MapToScreen(x, m_scrBufX);
    _y = MapToScreen(y, m_scrBufY);

    int cursorX = _x;
    int cursorY = _y;

    for (wchar_t ch : str) {
        if (ch == L'\n') {
            cursorX = _x;  // Go back to beginning of next line
            cursorY += 1;      // Move down
            continue;
        }

        if (ch != L' ') {
            int index = cursorY * m_scrBufX + cursorX;
            if (cursorX >= 0 && cursorX < m_scrBufX && cursorY >= 0 && cursorY < m_scrBufY) {
                Draw(ch, color, index);
            }
        }

        cursorX++;  // Always move cursor, even for spaces (to maintain alignment)
    }
}

void Renderer::Fill(wchar_t ch, short color, float x1, float y1, float x2, float y2){
   // CohenSutherlandClip(x1, y1, x2, y2);
    int _x1, _y1, _x2, _y2, index;
    _x1 = MapToScreen(x1, m_scrBufX);
    _x2 = MapToScreen(x2, m_scrBufX);
    _y1 = MapToScreen(y1, m_scrBufY);
    _y2 = MapToScreen(y2, m_scrBufY);

    for (int x = _x1; x < _x2; x++)
        for (int y = _y1; y < _y2; y++) {
            index = y * m_scrBufX + x;
            Draw(ch, color, index);
        }
}

void Renderer::FillInt(wchar_t ch, short color, int x1, int y1, int x2, int y2) {
    int index = 0;
    for (int x = x1; x < x2; x++)
        for (int y = y1; y < y2; y++) {
            index = y * m_scrBufX + x;
            Draw(ch, color, index);
        }
}

void Renderer::Clear(wchar_t ch, int color) {
    for (int i = 0; i < m_scrBuf_sz; i++) {
        m_scrBuf_p[i].Char.UnicodeChar = ch;
        m_scrBuf_p[i].Attributes = color;
    }
}

void Renderer::Clear(wchar_t ch) {
    for (int i = 0; i < m_scrBuf_sz; i++) {
        m_scrBuf_p[i].Char.UnicodeChar = ch;
        m_scrBuf_p[i].Attributes = m_clearColor;
    }
}

void Renderer::Clear() {
    for (int i = 0; i < m_scrBuf_sz; i++) {
        m_scrBuf_p[i].Char.UnicodeChar = m_clearChar;
        m_scrBuf_p[i].Attributes = m_clearColor;
    }
}

void Renderer::SetClearColor(int color) {
    m_clearColor = color;
}

void Renderer::SetClearChar(wchar_t ch) {
    m_clearChar = ch;
}

void Renderer::BindInput(Input* inp) {
    m_input_p = inp;
}

float  Renderer::NDCforOneCharX() const {
    return  2.0f / m_scrBufX;
 }

float  Renderer::NDCforOneCharY() const {
    return  2.0f / m_scrBufY;
}

int Renderer::GetWidth() const {
    return m_scrBufX;
}
int Renderer::GetHeight() const {
    return m_scrBufY;
}


// Takes function pointer. The taken function is ran in the game loop.
void Renderer::Start(void (*f_ptr)(Renderer& rend)) {
    std::chrono::high_resolution_clock::time_point tp1 = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point tp2;
    std::chrono::duration<float> duration;
    while (1) {
        tp2 = std::chrono::high_resolution_clock::now();
        duration = tp2 - tp1;
        tp1 = tp2;
        m_deltaTime = duration.count();


        m_input_p->CollectKeyStates();

        if (f_ptr != nullptr) {
            f_ptr(*this);
        }

        UpdateScreen();
        Clear();
    }
}



void Renderer::UpdateScreen() {
    //m_fps = 0.9 * m_fps + (1.0f - 0.9) * 1 / m_deltaTime; - smoothig
    // Maybe - 1 is dull.
    SMALL_RECT rect = { 0, 0, (short)m_scrBufX - 1, (short)m_scrBufY - 1 };
    wchar_t s[256];
    swprintf_s(s, 256, L" %s | FPS: %3.2f", L"PK-CGE", 1.0f / m_deltaTime);
    SetConsoleTitle(s);
    // must be W(wide) to support unicode
    WriteConsoleOutputW(m_hConsoleOut, m_scrBuf_p, { (short)m_scrBufX, (short)m_scrBufY }, { 0,0 }, &rect);
}

// Maybe asserts were nor the best choice.
// var\scrBufX, var\scrBufY - in chars. var\fontX, var\fontY - in px.
// Estimated size of console's window in px = (var\scrBufX * var\fontX) * Display Scale, (var\scrBufY * var\fontY)  * Display Scale. 
void Renderer::SetUpConsole(short scrBufX, short scrBufY, int fontX, int fontY) {
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

    m_scrBuf_p = new CHAR_INFO[scrBufX * scrBufY];
    memset(m_scrBuf_p, 0, sizeof(CHAR_INFO) * scrBufX * scrBufY);

    m_scrBuf_sz = scrBufX * scrBufY;
    m_hConsoleOut = handle;
    m_scrBufX = scrBufX;
    m_scrBufY = scrBufY;
    m_fontX = m_fontX;
    m_fontY = m_fontY;
}



