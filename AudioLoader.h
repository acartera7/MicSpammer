

#ifndef MICSPAMMER_AUDIOLOADER_H
#define MICSPAMMER_AUDIOLOADER_H
#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QDebug>

class AudioLoader : public QObject {
    Q_OBJECT
public:
    explicit AudioLoader(QObject* parent = nullptr);
    void loadFile(const QString& path);
    void setDecoderFormat(const QAudioFormat &audioFormat);
    ~AudioLoader() override;

    signals:
    void pcmReady(QByteArray data);
    void loadingFinished();

private slots:
    void onBufferReady();
    void onFinished();

private:
    QAudioDecoder* decoder;
};
#endif //MICSPAMMER_AUDIOLOADER_H