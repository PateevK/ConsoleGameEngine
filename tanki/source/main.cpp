#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <cstdlib>

#include "renderer/renderer.h"
#include "input/input.h"

// 1. File\Renderer.cpp Func\MapToScreen - weird -1.
// 2. File\Renderer.cpp Func\Renderer::SetUpConsole - Asserts used, maybe some could be interchanged with throws.
// 3. File\Renderer.cpp Func\Renderer::DrawFilledTriangle - That works but i'm not satiesfyed.
// 4. File\input.h && input.cpp var\m_newKeyState[256] - Might be just one short;
// 5. File\Renderer.cpp Func\Renderer::UpdateScreen() - Make customizable window Title;
// 6. func\BindInput(&input) - May also be redundant. 

void GameLogic(Renderer& rend) {
    // Here goes the game loop;
    rend.DrawFilledTriangle(0x2588, FG_BLUE, -0.5, -0.5, 0.5, -0.5, 0, 0.5);
}

int main() {
    try {
        Renderer game;
        game.SetUpConsole(300, 300, 2, 2);
        game.SetClearColor(0x000F);
        game.SetClearChar(0x2588);
        Input input;
        game.BindInput(&input);

        game.Start(GameLogic);
    }
    catch (ERROR e) {
        SetUpConsole_log(120, 30, 7, 16);
        std::cerr << "Exeption caught" << e;
    }
    catch (...) {
        SetUpConsole_log(120, 30, 7, 16);
        std::cerr << "Undifiend exeption caught";
    }
    std::cin.get();
    return 0;
}