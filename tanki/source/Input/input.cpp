#include "input.h"


void  Input::CollectKeyStates() {

	for (int i = 0; i < 256; i++)
	{
		m_newKeyState[i] = GetAsyncKeyState(i);

		m_keys[i].bPressed = false;
		m_keys[i].bReleased = false;

		if (m_newKeyState[i] != m_oldKeyState[i])
		{
			if (m_newKeyState[i] & 0x8000)
			{
				m_keys[i].bPressed = !m_keys[i].bHeld;
				m_keys[i].bHeld = true;
			}
			else 
			{
				m_keys[i].bReleased = true;
				m_keys[i].bHeld = false;
			}
		}
		
		m_oldKeyState[i] = m_newKeyState[i];
	}
}

bool Input::GetKeyState(int key, KeyState state){

	switch (state) {
		case(KS_HELD):     return m_keys[key].bHeld;
		case(KS_PRESSED):  return m_keys[key].bPressed;
		case(KS_RELEASED): return m_keys[key].bReleased;
		default:           return false;
	}
}