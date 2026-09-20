
#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QBoxLayout>
#include <QToolBar>
#include <QPushButton>
#include <QFileDialog>
#include <QSlider>
#include <QFileSystemModel>
#include <QSplitter>
#include <QComboBox>
#include <QJsonObject>
#include <QMessageBox>
#include <QCheckBox>
#include <QStandardPaths>

#include "FileBrowserWidget.h"
#include "NumpadWidget.h"
#include "AudioPlayer.h"
#include "KeyboardHook.h"
#include "MicCapture.h"
#include "WasapiManager.h"


#ifndef MICSPAMMERWINDOW_H
#define MICSPAMMERWINDOW_H

class MicSpammerWindow final : public QMainWindow {
Q_OBJECT
public:
    explicit MicSpammerWindow(QWidget *parent = nullptr);
    ~MicSpammerWindow();

protected:
    //void keyPressEvent(QKeyEvent *event) override;
    //void focusInEvent(QFocusEvent *event) override;
    //void focusOutEvent(QFocusEvent *event) override;

private:
    int _window_x, _window_y;

// Handler Functions
    void onOpenFolder();
    void onPreview();
    void onStop();
    void onVolumeChanged(QString name, int volume);
    void onFileSelected(const QString &filePath);

    bool loadProfile();

    void onLoadProfile();

bool saveProfile(QString fileName);

void onSaveProfile();
    void onDeleteProfile();
    void onReset();
    void resetProfileSettings();
    void onNumpadPressed(int key);
    void onMicDeviceChanged(int index);
    void onMonitorDeviceChanged(int index);
    void onSendDeviceChanged(int index);

bool cacheProfilePath();

bool cacheLookupProfilePath();

    bool isDeviceValid(const QString &deviceName);

    QString selectedFilePath = "";
    QString currentProfilePath = "";

    AudioPlayer& audioPlayer;
    MicCapture& micCapture;
    FileBrowserWidget *browser;
    NumpadWidget *numpad;
    KeyboardHook& keyboardHook;

    // GUI Elements
    QWidget     *mainWidget;
    QVBoxLayout *mainVLayout;

    QToolBar *toolbar;
    QToolBar *profile_toolbar;

    QPushButton *openFolderButton, *previewButton, *stopButton, *loadProfileButton, *saveProfileButton, *deleteProfileButton, *resetButton;
    QWidget *toolbarSpacer, *profileSpacer, *toolbar_rightContainer, *toolbar_devicesContainer, *mainContent_container;
    QHBoxLayout *toolbar_rightHLayout, *mainContent_HLayout;

    QSlider *micVolumeSlider, *monitorVolumeSlider, *sendVolumeSlider;
    QCheckBox *micMuteCheckBox, *monitorMuteCheckBox, *sendMuteCheckBox;
    QCheckBox *sendPreviewCheckBox;
    QSplitter *mainContent_splitter;

    QLabel* micDeviceLabel, *monitorDeviceLabel, *sendDeviceLabel, *profileLabel;

    QComboBox* micComboBox,*monitorComboBox, *sendComboBox;

    QGridLayout *toolbar_devicesGridLayout;

    const std::vector<AudioDeviceInfo>& deviceList;
};



#endif //MICSPAMMERWINDOW_H
