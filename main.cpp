#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QMediaPlayer>
#include <thread>
#include "VideoSurface.h"
#include <QQmlContext>
#include "imageprovider.h"
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);


    QMediaPlayer *player = new QMediaPlayer;
    VideoSurface videoSurface;
    ImageProvider imageprovider;
    QObject::connect(&videoSurface, &VideoSurface::frameAvailable, &imageprovider, &ImageProvider::setImage);

    player->setVideoOutput(&videoSurface);
    player->setMedia(QUrl::fromLocalFile(QFileInfo("./video.mp4").absoluteFilePath()));
    std::thread thr([&]() {
        player->play();
    });
    thr.detach();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("imageprovider", &imageprovider);
    engine.addImageProvider("imageprovider", &imageprovider);
    engine.load(url);

    return app.exec();
}
