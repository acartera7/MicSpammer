//
// Created by Victus on 9/14/2026.
//
#pragma once
#include <QObject>
#include <windows.h>

#ifndef MICSPAMMER_KEYBOARDHOOK_H
#define MICSPAMMER_KEYBOARDHOOK_H


class KeyboardHook : public QObject {
    Q_OBJECT
public:
    static KeyboardHook& getInstance();


    bool start();

    void stop();
signals:
    void numpadPressed(int key);

private:
    explicit KeyboardHook(QObject *parent = nullptr);
    ~KeyboardHook();

    static LRESULT CALLBACK keyboardProc(
        int nCode,
        WPARAM wParam,
        LPARAM lParam
    );

    HHOOK hook = nullptr;
};


#endif //MICSPAMMER_KEYBOARDHOOK_H