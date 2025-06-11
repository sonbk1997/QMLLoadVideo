#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>

class ImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit ImageProvider(QObject *parent = nullptr);
    ~ImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
public slots:
    void setImage(const QImage &img);

Q_SIGNALS:
    void updateGraphicsImage();
private:
    QImage m_image;
};

#endif // IMAGEPROVIDER_H
