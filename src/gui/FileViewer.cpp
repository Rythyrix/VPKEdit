#include "FileViewer.h"

#include <QHBoxLayout>

#include "previews/AudioPreview.h"
#include "previews/DirPreview.h"
#include "previews/ImagePreview.h"
#include "previews/TextPreview.h"
#include "previews/VTFPreview.h"
#include "Window.h"

using namespace vpktool;

FileViewer::FileViewer(Window* window_, QWidget* parent)
        : QWidget(parent)
        , window(window_) {
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    this->audioPreview = new AudioPreview(this);
    layout->addWidget(this->audioPreview);

    this->dirPreview = new DirPreview(this);
    layout->addWidget(this->dirPreview);

    this->imagePreview = new ImagePreview(this);
    layout->addWidget(this->imagePreview);

    this->textPreview = new TextPreview(this);
    layout->addWidget(this->textPreview);

    this->vtfPreview = new VTFPreview(this);
    layout->addWidget(this->vtfPreview);

    this->clearContents();
    this->setTextPreviewVisible();
}

void FileViewer::displayEntry(const QString& path) {
    this->clearContents();
    if (path.endsWith(".tga")  ||
        path.endsWith(".jpg")  ||
        path.endsWith(".jpeg") ||
        path.endsWith(".jfif") ||
        path.endsWith(".png")  ||
        path.endsWith(".webp") ||
        path.endsWith(".bmp")) {
        // Image
        this->imagePreview->setImage(this->window->readBinaryEntry(path));
        this->setImagePreviewVisible();
    } else if (path.endsWith(".txt") ||
        path.endsWith(".md")  ||
        path.endsWith(".gi")  ||
        path.endsWith(".res") ||
        path.endsWith(".rad") ||
        path.endsWith(".nut") ||
        path.endsWith(".lua") ||
        path.endsWith(".gm")  ||
        path.endsWith(".py")  ||
        path.endsWith(".js")  ||
        path.endsWith(".ts")  ||
        path.endsWith(".cfg") ||
        path.endsWith(".ini") ||
        path.endsWith(".kv")  ||
        path.endsWith(".kv3") ||
        path.endsWith(".vdf") ||
        path.endsWith(".vmf") || // hey you never know
        path.endsWith(".vmt")) {
        // Text
        this->textPreview->setText(this->window->readTextEntry(path));
        this->setTextPreviewVisible();
    } else if (path.endsWith(".vtf")) {
        // VTF (texture)
        this->vtfPreview->setImage(this->window->readBinaryEntry(path));
        this->setVTFPreviewVisible();
    } else if (path.endsWith(".mp3")) {
        // Audio
        this->audioPreview->setAudio(this->window->readBinaryEntry(path), "_" + path.sliced(path.lastIndexOf('.')));
        this->setAudioPreviewVisible();
    }
}

void FileViewer::displayDir(const QString& /*path*/, const QList<QString>& subfolders, const QList<QString>& entryPaths, const VPK& vpk) {
    this->clearContents();
    this->dirPreview->setPath(subfolders, entryPaths, vpk);
    this->setDirPreviewVisible();
}

void FileViewer::selectSubItemInDir(const QString& name) {
    this->window->selectSubItemInDir(name);
}

void FileViewer::clearContents() {
    this->textPreview->setText("");
    this->setTextPreviewVisible();
}

void FileViewer::setAudioPreviewVisible() {
    this->audioPreview->show();
    this->dirPreview->hide();
    this->imagePreview->hide();
    this->textPreview->hide();
    this->vtfPreview->hide();
}

void FileViewer::setDirPreviewVisible() {
    this->audioPreview->hide();
    this->dirPreview->show();
    this->imagePreview->hide();
    this->textPreview->hide();
    this->vtfPreview->hide();
}

void FileViewer::setImagePreviewVisible() {
    this->audioPreview->hide();
    this->dirPreview->hide();
    this->imagePreview->show();
    this->textPreview->hide();
    this->vtfPreview->hide();
}

void FileViewer::setTextPreviewVisible() {
    this->audioPreview->hide();
    this->dirPreview->hide();
    this->imagePreview->hide();
    this->textPreview->show();
    this->vtfPreview->hide();
}

void FileViewer::setVTFPreviewVisible() {
    this->audioPreview->hide();
    this->dirPreview->hide();
    this->imagePreview->hide();
    this->textPreview->hide();
    this->vtfPreview->show();
}
