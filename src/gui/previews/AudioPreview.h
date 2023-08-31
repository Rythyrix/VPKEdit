#pragma once

#include <QString>
#include <QWidget>

class QAudioOutput;
class QMediaPlayer;

class AudioPreview : public QWidget {
    Q_OBJECT;

public:
    explicit AudioPreview(QWidget* parent = nullptr);

    void setAudio(const std::vector<std::byte>& data, const QString& filename);

private:
    QAudioOutput* device;
    QMediaPlayer* player;
};
