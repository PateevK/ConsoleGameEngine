#pragma once
#include <windows.h>

enum KeyState {
	KS_HELD = 0,
	KS_PRESSED = 1,
	KS_RELEASED = 2
};

class Input {
public:
	Input() = default;
	void CollectKeyStates();
	bool GetKeyState(int key, KeyState state);

private:
	short m_newKeyState[256] = {0};
	short m_oldKeyState[256] = {0};
	struct {
		bool bHeld;
		bool bPressed;
		bool bReleased;
	} m_keys[256] = {0};

};