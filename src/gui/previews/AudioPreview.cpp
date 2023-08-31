#include "AudioPreview.h"

#include <QAudioDevice>
#include <QAudioOutput>
#include <QBuffer>
#include <QByteArray>
#include <QMediaDevices>
#include <QMediaFormat>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QMessageBox>

// todo: figure out why wav crashes internally

AudioPreview::AudioPreview(QWidget* parent)
        : QWidget(parent) {

    // todo: device select
    /*
    auto outs = QMediaDevices::audioOutputs();
    for (auto& dev : outs) {
        auto x = dev.description();
        qDebug() << x;
    }
    */

    this->player = new QMediaPlayer(this);
    this->device = new QAudioOutput(this);
    // todo: device select
    //this->device->setDevice(QMediaDevices::defaultAudioOutput());
    this->player->setAudioOutput(device);

    // todo: tie to playback indicator
    QObject::connect(this->player, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status) {
        qDebug() << "MediaStatus:" << this->player->mediaStatus() << "|" << status;
    });
    QObject::connect(this->player, &QMediaPlayer::errorOccurred, [=](QMediaPlayer::Error error) {
        QMessageBox::warning(this, "well shit", ("you fucked it up:\n" + std::to_string(error)).c_str());
    });
    QObject::connect(this->player, &QMediaPlayer::playbackStateChanged, [=](QMediaPlayer::PlaybackState state) {
        qDebug() << "PlaybackState:" << this->player->playbackState() << "|" << state;
    });
}

static bool isCodecSupported(QMediaFormat::AudioCodec fileCodec) {
    static const QList<QMediaFormat::AudioCodec> supportedAudioCodecs = QMediaFormat().supportedAudioCodecs(QMediaFormat::Decode);
    return std::any_of(supportedAudioCodecs.constBegin(), supportedAudioCodecs.constEnd(), [&](QMediaFormat::AudioCodec systemCodec) {
        return fileCodec == systemCodec;
    });
}

void AudioPreview::setAudio(const std::vector<std::byte>& data, const QString& filename) {
    QByteArray audioData(reinterpret_cast<const char*>(data.data()), static_cast<long long>(data.size()));

    // todo: remember to delete this buffer when the function is called again
    auto* audioBuffer = new QBuffer(&audioData, this);
    if (!audioBuffer->open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "well shit", "audio buffer didn't open");
    }
    audioBuffer->seek(0);

    this->player->setSourceDevice(audioBuffer, QUrl(filename));
    auto fileAudioCodec = this->player->metaData().value(QMediaMetaData::AudioCodec).value<QMediaFormat::AudioCodec>();
    if(!isCodecSupported(fileAudioCodec)) {
        qWarning() << tr("%1 (%2) is not supported by your system. Please install this codec to be able to play a sound.")
            .arg(QMediaFormat::audioCodecName(fileAudioCodec))
            .arg(QMediaFormat::audioCodecDescription(fileAudioCodec));
    }

    // todo: tie to play button
    this->player->play();
}
