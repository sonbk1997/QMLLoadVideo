#pragma once

#include <QAbstractVideoSurface>
#include <QVideoFrame>
#include <QImage>
#include <QDebug>

class VideoSurface : public QAbstractVideoSurface {
    Q_OBJECT

public:
    explicit VideoSurface(QObject *parent = nullptr) : QAbstractVideoSurface(parent) {}

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
        QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const override {
        Q_UNUSED(type);
        return {
            QVideoFrame::Format_RGB32,
            QVideoFrame::Format_ARGB32,
            QVideoFrame::Format_ARGB32_Premultiplied,
            QVideoFrame::Format_RGB24,
            QVideoFrame::Format_RGB565,
            QVideoFrame::Format_RGB555
        };
    }

    bool present(const QVideoFrame &frame) override {
        if (!frame.isValid())
            return false;

        QVideoFrame cloneFrame(frame);
        if (!cloneFrame.map(QAbstractVideoBuffer::ReadOnly))
            return false;

        QImage image = QImage(cloneFrame.bits(),
                              cloneFrame.width(),
                              cloneFrame.height(),
                              cloneFrame.bytesPerLine(),
                              QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));

        if (!image.isNull()) {
            emit frameAvailable(image.copy()); // emit copy to keep it safe
        }

        cloneFrame.unmap();
        return true;
    }

signals:
    void frameAvailable(const QImage &image);
};
