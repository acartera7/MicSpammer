//
// Created by Victus on 9/14/2026.
//

#include "KeyboardHook.h"


KeyboardHook::KeyboardHook(QObject *parent)
        : QObject(parent) {
}
KeyboardHook::~KeyboardHook() {
    stop();

}

KeyboardHook& KeyboardHook::getInstance() {
    static KeyboardHook instance;
    return instance;
}

bool KeyboardHook::start() {
    hook = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        keyboardProc,
        GetModuleHandle(nullptr),
        0);

    return hook != nullptr;
}

void KeyboardHook::stop() {
    if (hook != nullptr) {
        UnhookWindowsHookEx(hook);
        hook = nullptr;
    }
}

LRESULT KeyboardHook::keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* keyboard = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            DWORD key = keyboard->vkCode;

            if (key >= VK_NUMPAD0 && key <= VK_DIVIDE){

                emit getInstance().numpadPressed(key);
            }
        }
    }
    return CallNextHookEx(nullptr,nCode, wParam, lParam);
}
