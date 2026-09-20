
#include "MicSpammerWindow.h"

MicSpammerWindow::MicSpammerWindow(QWidget *parent) :
        QMainWindow(parent),  _window_x(800),_window_y(500),
        audioPlayer(AudioPlayer::getInstance()),
        micCapture(MicCapture::getInstance()),
        keyboardHook(KeyboardHook::getInstance()),
        deviceList(WasapiManager::getInstance().getDevices()) {


    //setFocusPolicy(Qt::StrongFocus);
    //setFocus();
    //AudioPlayer::getInstance().setParent(this);
    //MicCapture::getInstance().setParent(this);
    //KeyboardHook::getInstance().setParent(this);

    keyboardHook.start();

    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainVLayout = new QVBoxLayout(this);
    setWindowTitle("MicSpammer");
    setGeometry(100,100, _window_x, _window_y);

    // profile toolbar for loading profiles
    profile_toolbar = new QToolBar(this);

    openFolderButton = new QPushButton("Open Folder", this);
    profileLabel = new QLabel("Profile: None", this);
    profileLabel->setMargin(15);
    profileLabel->setAlignment(Qt::AlignRight);
    loadProfileButton = new QPushButton("Load", this);
    saveProfileButton = new QPushButton("Save", this);
    deleteProfileButton = new QPushButton("Delete", this);
    resetButton = new QPushButton("Reset", this);

    // Spacer Widget (Flexible Space)
    profileSpacer = new QWidget(this);
    profileSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    profile_toolbar->addWidget(openFolderButton);
    profile_toolbar->addWidget(profileSpacer);
    profile_toolbar->addWidget(profileLabel);
    profile_toolbar->addWidget(loadProfileButton);
    profile_toolbar->addWidget(saveProfileButton);
    profile_toolbar->addWidget(resetButton);
    profile_toolbar->addWidget(deleteProfileButton);

    //addToolBar(Qt::TopToolBarArea, profile_toolbar);
    // Main Toolbar & Button
    toolbar = new QToolBar(this);
    previewButton = new QPushButton("Preview", this);
    stopButton = new QPushButton("Stop", this);


    // Spacer Widget (Flexible Space)
    toolbarSpacer = new QWidget(this);
    toolbarSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Devices selection dropdown
    micComboBox = new QComboBox(this);
    monitorComboBox = new QComboBox(this);
    sendComboBox = new QComboBox(this);

    micComboBox->addItem("--None--", QVariant("None"));
    micComboBox->setCurrentIndex(0);
    monitorComboBox->addItem("--None--", QVariant("None"));
    monitorComboBox->setCurrentIndex(0);
    sendComboBox->addItem("--None--", QVariant("None"));
    sendComboBox->setCurrentIndex(0);

    for (size_t i = 0; i < deviceList.size(); i++) {
        const auto& info = deviceList.at(i);
        QString name = QString::fromStdWString(info.name);
        if (info.flow == eCapture) {
            micComboBox->addItem(name, QVariant(QString::fromStdWString(info.id)));
        } else if (info.flow == eRender) {
            monitorComboBox->addItem(name, QVariant(QString::fromStdWString(info.id)));
            sendComboBox->addItem(name, QVariant(QString::fromStdWString(info.id)));
        }
    }

    sendVolumeSlider = new QSlider(Qt::Horizontal, this);
    sendVolumeSlider->setRange(0, 100);
    sendVolumeSlider->setFixedWidth(100);
    sendVolumeSlider->setObjectName("SendVS");
    monitorVolumeSlider = new QSlider(Qt::Horizontal, this);
    monitorVolumeSlider->setRange(0, 100);
    monitorVolumeSlider->setFixedWidth(100);
    monitorVolumeSlider->setObjectName("MonitorVS");
    micVolumeSlider = new QSlider(Qt::Horizontal, this);
    micVolumeSlider->setRange(0, 100);
    micVolumeSlider->setFixedWidth(100);
    micVolumeSlider->setObjectName("MicVS");

    micMuteCheckBox = new QCheckBox(this);
    micMuteCheckBox->setCursor(Qt::PointingHandCursor);
    micMuteCheckBox->setText("");
    micMuteCheckBox->setStyleSheet(R"(
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 2px outset #aaaaaa;
            image: url(:/icons/assets/microphone-mute.png);
        }
        QCheckBox::indicator:checked {
            border: 2px inset darkgray;
            image: url(:/icons/assets/microphone-mute-red.png);
        }

    )");


    monitorMuteCheckBox = new QCheckBox(this);
    monitorMuteCheckBox->setCursor(Qt::PointingHandCursor);
    monitorMuteCheckBox->setText("");
    monitorMuteCheckBox->setStyleSheet(R"(
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 2px outset #aaaaaa;
            image: url(:/icons/assets/mute.png);
        }
        QCheckBox::indicator:checked {
            image: url(:/icons/assets/mute-red.png);
            border: 2px inset darkgray;
        }
    )");

    sendMuteCheckBox = new QCheckBox(this);
    sendMuteCheckBox->setCursor(Qt::PointingHandCursor);
    sendMuteCheckBox->setText("");
    sendMuteCheckBox->setStyleSheet(R"(
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 2px outset #aaaaaa;
            image: url(:/icons/assets/mute.png);
        }
        QCheckBox::indicator:checked {
            image: url(:/icons/assets/mute-red.png);
            border: 2px inset darkgray;
        }
    )");

    toolbar_devicesGridLayout = new QGridLayout(this);

    micDeviceLabel = new QLabel("Microphone Device:",this);
    monitorDeviceLabel = new QLabel("Monitoring Device:",this);
    sendDeviceLabel = new QLabel("Output Device",this);

    toolbar_devicesGridLayout->addWidget(micDeviceLabel, 0,0);
    toolbar_devicesGridLayout->addWidget(micComboBox, 0,1);
    toolbar_devicesGridLayout->addWidget(micVolumeSlider, 0,2);
    toolbar_devicesGridLayout->addWidget(micMuteCheckBox, 0, 3);

    toolbar_devicesGridLayout->addWidget(monitorDeviceLabel, 1,0);
    toolbar_devicesGridLayout->addWidget(monitorComboBox, 1,1);
    toolbar_devicesGridLayout->addWidget(monitorVolumeSlider, 1,2);
    toolbar_devicesGridLayout->addWidget(monitorMuteCheckBox, 1, 3);

    toolbar_devicesGridLayout->addWidget(sendDeviceLabel, 2,0);
    toolbar_devicesGridLayout->addWidget(sendComboBox, 2,1);
    toolbar_devicesGridLayout->addWidget(sendVolumeSlider, 2,2);
    toolbar_devicesGridLayout->addWidget(sendMuteCheckBox, 2, 3);


    //devices left part devices drop down container
    toolbar_devicesContainer = new QWidget(this);
    toolbar_devicesContainer->setLayout(toolbar_devicesGridLayout);

    // toolbar right part layout container
    toolbar_rightContainer  = new QWidget(this);
    toolbar_rightHLayout  = new QHBoxLayout(this);
    toolbar_rightHLayout->addWidget(previewButton);
    toolbar_rightHLayout->addSpacing(10); // Spacing between play & stop
    toolbar_rightHLayout->addWidget(stopButton);
    toolbar_rightHLayout->setContentsMargins(0, 0, 0, 0); // Removes extra margins
    toolbar_rightContainer->setLayout(toolbar_rightHLayout);

    // Add widgets to toolbar
    toolbar->addWidget(toolbar_devicesContainer);
    toolbar->addWidget(toolbarSpacer);
    toolbar->addWidget(toolbar_rightContainer);
    //addToolBar(Qt::TopToolBarArea, toolbar);
    // add toolbar to the window
    mainVLayout->addWidget(profile_toolbar);
    mainVLayout->addWidget(toolbar);

    //QDir(QDir::homePath()).filePath("Music")
    sendPreviewCheckBox = new QCheckBox(this);
    sendPreviewCheckBox->setCursor(Qt::PointingHandCursor);
    sendPreviewCheckBox->setCheckState(Qt::Unchecked);
    sendPreviewCheckBox->setText("Play preview on Output Device");
    sendPreviewCheckBox->setStyleSheet("QWidget { padding-left: 20px; }");

    mainVLayout->addWidget(sendPreviewCheckBox);

    //create and fill horizontal main_content widget below toolbar
    mainContent_container  = new QWidget(this);
    mainContent_HLayout  = new QHBoxLayout(this);
    //create left hand browser
    browser = new FileBrowserWidget(this);

    browser->setRootDirectory(QDir::homePath());

    // create right hand numpad
    numpad = new NumpadWidget(this);

    mainContent_splitter = new QSplitter(Qt::Horizontal, this);
    mainContent_splitter->show();
    mainContent_splitter->setHandleWidth(2);
    mainContent_splitter->setObjectName("mainContent_splitter");
    mainContent_splitter->setStyleSheet(
        "QSplitter#mainContent_splitter::handle { background-color: lightgray; } "
    );
    mainContent_splitter->setSizes({400, 400});
    mainContent_splitter->addWidget(browser);
    mainContent_splitter->addWidget(numpad);
    mainContent_HLayout->addWidget(mainContent_splitter);

    mainContent_container->setLayout(mainContent_HLayout);
    mainVLayout->addWidget(mainContent_container);
    centralWidget()->setLayout(mainVLayout);

    // Toolbar buttons actions
    connect(loadProfileButton, &QPushButton::clicked, this, &MicSpammerWindow::onLoadProfile);
    connect(saveProfileButton, &QPushButton::clicked, this, &MicSpammerWindow::onSaveProfile);
    connect(deleteProfileButton, &QPushButton::clicked, this, &MicSpammerWindow::onDeleteProfile);
    connect(resetButton, &QPushButton::clicked, this, &MicSpammerWindow::onReset);

    connect(openFolderButton, &QPushButton::clicked, this, &MicSpammerWindow::onOpenFolder);
    connect(previewButton, &QPushButton::clicked, this, &MicSpammerWindow::onPreview);
    connect(stopButton, &QPushButton::clicked, this, &MicSpammerWindow::onStop);
    connect(micVolumeSlider, &QSlider::valueChanged, this,
        [this](int volume) {
            onVolumeChanged(micVolumeSlider->objectName(), volume);
        });
    connect(monitorVolumeSlider, &QSlider::valueChanged, this,
        [this](int volume) {
            onVolumeChanged(monitorVolumeSlider->objectName(), volume);
        });
    connect(sendVolumeSlider, &QSlider::valueChanged, this,
        [this](int volume) {
            onVolumeChanged(sendVolumeSlider->objectName(), volume);
        });
    // File actions
    connect(browser, &FileBrowserWidget::fileSelected, this, &MicSpammerWindow::onFileSelected);
    connect(browser, &FileBrowserWidget::playSound, this, &MicSpammerWindow::onPreview);
    connect(numpad, &NumpadWidget::numpadTriggered,this,
        [this](int key, const QString &filePath) {
            audioPlayer.play(filePath, true);
        });

    connect(numpad, &NumpadWidget::pageChanged, this, [](int page) {
        qDebug() << "Switched to page:" << page;
    });

    // Devices
    connect(micComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MicSpammerWindow::onMicDeviceChanged);
    connect(monitorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MicSpammerWindow::onMonitorDeviceChanged);
    connect(sendComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MicSpammerWindow::onSendDeviceChanged);

    // mute buttons

    connect(micMuteCheckBox, &QCheckBox::checkStateChanged, this,
        [this](Qt::CheckState state) {
            if (state == Qt::Checked) {
                micCapture.mute(true);
            } else {
                micCapture.mute(false);
            }
    });

    connect(monitorMuteCheckBox, &QCheckBox::checkStateChanged, this,
        [this](Qt::CheckState state) {
            if (state == Qt::Checked) {
                audioPlayer.muteMonitor(true);
            } else {
                audioPlayer.muteMonitor(false);
            }
    });

    connect(sendMuteCheckBox, &QCheckBox::checkStateChanged, this,
        [this](Qt::CheckState state) {
            if (state == Qt::Checked) {
                audioPlayer.muteOutput(true);
            } else {
                audioPlayer.muteOutput(false);
            }
    });

    connect(&KeyboardHook::getInstance(), &KeyboardHook::numpadPressed, this, &MicSpammerWindow::onNumpadPressed);

    // Create saves folder if not already existing
    if (QDir().mkpath("saves")) {
    } else {
        qDebug() << "Error MicSpammerWindow: failed to make /saves folder" ;
    }

    if (cacheLookupProfilePath()) {
        loadProfile();
    }

    monitorVolumeSlider->setValue(80);
    sendVolumeSlider->setValue(80);
    micVolumeSlider->setValue(80);

}

void MicSpammerWindow::onOpenFolder() {
    QString folder = QFileDialog::getExistingDirectory(this, "Select a folder", QDir::homePath());
    if (!folder.isEmpty()) {
        browser->setRootDirectory(folder);  // Update browser widget
    }
}

void MicSpammerWindow::onPreview() {
    if (!selectedFilePath.isEmpty()) {
        audioPlayer.play(selectedFilePath,sendPreviewCheckBox->isChecked());  // Play last selected file
    }
}

void MicSpammerWindow::onStop() {
    audioPlayer.stopAll();
}

void MicSpammerWindow::onVolumeChanged(QString name, int volume) {

    float gain = 0.0f;
    // Clamp to avoid log(0)
    if (volume > 0) {
        // Map 0–100 slider to -40 dB .. 0 dB range
        float minDb = -40.0f;   // silence threshold
        float maxDb = 0.0f;

        float db = minDb + (volume / 100.0f) * (maxDb - minDb);

        // Convert dB to linear gain
        gain = powf(10.0f, db / 20.0f);
    }

    if ( name == "MonitorVS") {
        audioPlayer.setMonitorVolume(gain);
    } else if (name == "SendVS") {
        audioPlayer.setOutputVolume(gain);
    } else if (name == "MicVS") {
        micCapture.setVolume(gain);
    }
}

void MicSpammerWindow::onFileSelected(const QString &filePath) {
    selectedFilePath = filePath;
}

bool MicSpammerWindow::loadProfile() {
    if (!currentProfilePath.isEmpty()) {

        QFile file(currentProfilePath);
        if (!file.open(QIODevice::ReadOnly)) return true;


        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isNull()) return true;

        // load states for browser and numpad widgets
        QJsonObject root = doc.object();
        browser->loadState(root["browser"].toObject());
        numpad->loadState(root["numpad"].toObject());

        // load state for the window size
        QJsonObject winObj = root["window"].toObject();
        QSize winSize(winObj["w"].toInt(),
                      winObj["h"].toInt());
        resize(winSize.width(), winSize.height());

        // load states for the devices and volumes
        QJsonObject devicesObj = root["devices"].toObject();

        // mic device
        QString micDeviceName = devicesObj["micdevice-id"].toString();
        if (isDeviceValid(micDeviceName)) {
            micCapture.setInputDevice(micDeviceName);
            micComboBox->setCurrentIndex(micComboBox->findData(micDeviceName));
        } else {
            QString deviceText = devicesObj["micdevice-text"].toString();
            QMessageBox::information(this,"Error","Error: failed to load Microphone Device \"" + deviceText +"\". Device not found or is disabled.");
        }

        //monitor device
        QString monitorDeviceName = devicesObj["monitoringdevice-id"].toString();
        if (isDeviceValid(monitorDeviceName)) {
            audioPlayer.setMonitorDevice(monitorDeviceName);
            monitorComboBox->setCurrentIndex(monitorComboBox->findData(monitorDeviceName));
        } else {
            QString deviceText = devicesObj["monitoringdevice-text"].toString();
            QMessageBox::information(this,"Error","Error: failed to load Monitoring Device \"" + deviceText +"\". Device not found or is disabled.");
        }


        //output device
        QString outputDeviceName = devicesObj["outputdevice-id"].toString();
        if (isDeviceValid(outputDeviceName)) {
            micCapture.setOutputDevice(devicesObj["outputdevice-id"].toString());
            audioPlayer.setOutputDevice(devicesObj["outputdevice-id"].toString());
            sendComboBox->setCurrentIndex(sendComboBox->findData(outputDeviceName));
        } else {
            QString deviceText = devicesObj["outputdevice-text"].toString();
            QMessageBox::information(this,"Error","Error: failed to load Output Device \"" + deviceText +"\". Device not found or is disabled.");
        }

        QJsonObject volumeObj = root["volume"].toObject();
        micVolumeSlider->setValue(volumeObj["mic-volume"].toInt());
        micMuteCheckBox->setChecked(volumeObj["mic-muted"].toBool());
        monitorVolumeSlider->setValue(volumeObj["monitor-volume"].toInt());
        monitorMuteCheckBox->setChecked(volumeObj["monitor-muted"].toBool());
        sendVolumeSlider->setValue(volumeObj["output-volume"].toInt());
        sendMuteCheckBox->setChecked(volumeObj["output-muted"].toBool());

        sendPreviewCheckBox->setChecked(root["send-preview"].toBool());

        profileLabel->setText("Profile: " + QFileInfo(currentProfilePath).baseName());
        cacheProfilePath();

        return true;
    }
    return false;
}

void MicSpammerWindow::onLoadProfile() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Load Profile",
        QDir("saves").absolutePath(),
        "Profile Files (*.json)"
    );

    currentProfilePath = fileName;
    if (!loadProfile()) {
        qDebug() << "failed to load profile: " << fileName;
        exit(-2);
    }
}

bool MicSpammerWindow::saveProfile(QString fileName) {
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.exists()) {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                "Overwrite Profile",
                "The file already exists. Do you want to overwrite it?",
                QMessageBox::Yes | QMessageBox::No
            );

            if (reply == QMessageBox::No) {
                return true; // user cancelled overwrite
            }
        }

        // save states for browser and numpad widgets
        QJsonObject root;
        root["browser"] = browser->saveState();
        root["numpad"] = numpad->saveState();

        // save state for the window size
        root["window"] = QJsonObject {
            {"w", window()->size().width()},
            {"h", window()->size().height()}
        };
        // save states for the devices and volumes
        root["devices"] = QJsonObject {
            {"micdevice-id", micComboBox->itemData(micComboBox->currentIndex()).toString()},
            {"micdevice-text", micComboBox->itemText(micComboBox->currentIndex())},
            {"monitoringdevice-id", monitorComboBox->itemData(monitorComboBox->currentIndex()).toString()},
            {"monitoringdevice-text", monitorComboBox->itemText(monitorComboBox->currentIndex())},
            {"outputdevice-id", sendComboBox->itemData(sendComboBox->currentIndex()).toString()},
            {"outputdevice-text", sendComboBox->itemText(sendComboBox->currentIndex())},
        };
        root["volume"] = QJsonObject {
            {"mic-volume", micVolumeSlider->value()},
            {"mic-muted", micMuteCheckBox->isChecked()},
            {"monitor-volume", monitorVolumeSlider->value()},
            {"monitor-muted", monitorMuteCheckBox->isChecked()},
            {"output-volume", sendVolumeSlider->value()},
            {"output-muted", sendMuteCheckBox->isChecked()}
        };
        root["send-preview"] = QJsonValue {sendPreviewCheckBox->isChecked()};

        QJsonDocument doc(root);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
            file.close();

        } else {
            QMessageBox::warning(this, "Error", "Could not save file for writing.");
            return true;
        }

        profileLabel->setText("Profile: " + QFileInfo(fileName).baseName());
        currentProfilePath = fileName;
        cacheProfilePath();
        return true;
    }
    return false;
}

void MicSpammerWindow::onSaveProfile() {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Profile",
        QDir("saves").absolutePath(),
        "Profile Files (*.json)"
    );

    if (!saveProfile(fileName)) {
        qDebug() << "failed to save profile: " << fileName;
        exit(-2);
    }
}

void MicSpammerWindow::onDeleteProfile() {
    if (currentProfilePath.isEmpty()) {
        QMessageBox::information(this,"Delete Profile","No profile currently loaded");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Delete Profile",
        "Are you sure you want to delete " + QFileInfo(currentProfilePath).fileName() + "?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        QFile::remove(currentProfilePath);
        currentProfilePath.clear();
        profileLabel->setText("Profile: None");

        reply = QMessageBox::question(
            this,
            "Reset Settings",
            "Would you like to reset current settings to default?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            resetProfileSettings();
        }
    }
}

void MicSpammerWindow::onReset() {

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Reset Profile",
        "Resetting will clear all settings. Do you want to save the current profile first?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Cancel) return;

    if (reply == QMessageBox::Yes) {
        onSaveProfile();
    }

    resetProfileSettings();

}

void MicSpammerWindow::resetProfileSettings() {

    // Reset devices
    micComboBox->setCurrentIndex(0);
    monitorComboBox->setCurrentIndex(0);
    sendComboBox->setCurrentIndex(0);

    micCapture.setInputDevice("None");
    micCapture.setOutputDevice("None");
    audioPlayer.setMonitorDevice("None");
    audioPlayer.setOutputDevice("None");

    // Reset volume sliders
    micVolumeSlider->setValue(80);
    monitorVolumeSlider->setValue(80);
    sendVolumeSlider->setValue(80);

    //reset mute checkboxes
    micMuteCheckBox->setChecked(false);
    monitorMuteCheckBox->setChecked(false);
    sendMuteCheckBox->setChecked(false);

    previewButton->setChecked(false);

    // Reset file browser
    browser->setRootDirectory(QDir::homePath());
    selectedFilePath.clear();

    // Reset numpad mappings and page
    numpad->resetMappings();
    setGeometry(100, 100, _window_x, _window_y);

    // Clear profile label
    currentProfilePath.clear();
    profileLabel->setText("Profile: None");

    cacheProfilePath();

    qDebug() << "Profile settings reset to defaults.";
}

void MicSpammerWindow::onMicDeviceChanged(int index) {
    QString id = micComboBox->itemData(index).toString();
    micCapture.setInputDevice(id);
}

void MicSpammerWindow::onMonitorDeviceChanged(int index) {
    QString id = monitorComboBox->itemData(index).toString();
    audioPlayer.setMonitorDevice(id);
}

void MicSpammerWindow::onSendDeviceChanged(int index) {
    QString id = sendComboBox->itemData(index).toString();
    micCapture.setOutputDevice(id);
    audioPlayer.setOutputDevice(id);
}

bool MicSpammerWindow::cacheProfilePath() {

    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    QDir dir(cacheDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "Couldn't create cache directory: " << cacheDir;
            return false;
        }
    }
    assert(dir.exists());

    QFile file(cacheDir + "/.cache");
    if (file.open(QFile::WriteOnly)) {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        out << currentProfilePath;
        file.close();
        return true;
    }
    qDebug() << "Couldn't open cache file for saving: " << cacheDir;
    return false;
}

bool MicSpammerWindow::cacheLookupProfilePath() {
    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    QFile file(cacheDir + "/.cache");
    if (file.open(QFile::ReadOnly)) {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

        in >> currentProfilePath;
        file.close();
        return true;
    }

    qDebug() << "Couldn't open cache file for loading: " << cacheDir;
    return false;
}

bool MicSpammerWindow::isDeviceValid(const QString &deviceName) {
    if (deviceName.isEmpty())
        return false;
    auto it = std::find_if(deviceList.begin(), deviceList.end(),
            [=](const AudioDeviceInfo& device) {
                std::wstring wname = deviceName.toStdWString();
                if (device.id == wname)
                    return true;
                return false;
            });
    if (it != deviceList.end())
        return true;
    return false;
}


MicSpammerWindow::~MicSpammerWindow() {
    keyboardHook.stop();
}

//TODO deal with user holding down the numpad resulting in sound spamming
void MicSpammerWindow::onNumpadPressed(int key) {

    switch (key) {

    case VK_ADD:        numpad->nextPage();     break;
    case VK_SUBTRACT:   numpad->prevPage();     break;
    case VK_NUMPAD1:    numpad->triggerKey(1);  break;
    case VK_NUMPAD2:    numpad->triggerKey(2);  break;
    case VK_NUMPAD3:    numpad->triggerKey(3);  break;
    case VK_NUMPAD4:    numpad->triggerKey(4);  break;
    case VK_NUMPAD5:    numpad->triggerKey(5);  break;
    case VK_NUMPAD6:    numpad->triggerKey(6);  break;
    case VK_NUMPAD7:    numpad->triggerKey(7);  break;
    case VK_NUMPAD8:    numpad->triggerKey(8);  break;
    case VK_NUMPAD9:    numpad->triggerKey(9);  break;

    case VK_NUMPAD0:    audioPlayer.stopAll();  break;

    default:
        break;
    }
}

//void MicSpammerWindow::focusInEvent(QFocusEvent *event) {
//    qDebug() << "MicSpammerWindow got focus";
//    QWidget::focusInEvent(event);
//}
//
//void MicSpammerWindow::focusOutEvent(QFocusEvent *event) {
//    qDebug() << "MicSpammerWindow lost focus";
//    QWidget::focusOutEvent(event);
//}