#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include "fileio.h"
#include "lyric.h"
#include "song.h"
#include "lyricdownload.h"
#include "clipboard.h"
#include "decode.h"
#include "karaoke.h"
#include "audiorecorder.h"
#include "karaokelyric.h"
#include "kugousong.h"
#include "kugoumv.h"
#include "kugouplaylist.h"
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc,argv);
    QCoreApplication::setOrganizationName("s");

    qmlRegisterType<FileIo, 1>("FileIo", 1, 0, "FileIo");
    qmlRegisterType<Lyric, 1>("Lyric", 1, 0, "Lyric");
    qmlRegisterType<Song, 1>("Song", 1, 0, "Song");
    qmlRegisterType<LyricDownload, 1>("LyricDownload", 1, 0, "LyricDownload");
    qmlRegisterType<Clipboard, 1>("Clipboard", 1, 0, "Clipboard");
    qmlRegisterType<Decode, 1>("Decode", 1, 0, "Decode");
    qmlRegisterType<Karaoke, 1>("Karaoke", 1, 0, "Karaoke");
    qmlRegisterType<KaraokeLyric, 1>("KaraokeLyric", 1, 0, "KaraokeLyric");
    qmlRegisterType<AudioRecorder,1>("AudioRecorder", 1, 0, "AudioRecorder");
    qmlRegisterType<KuGouSong, 1>("KuGouSong", 1, 0, "KuGouSong");
    qmlRegisterType<KuGouMv, 1>("KuGouMv", 1, 0, "KuGouMv");
    qmlRegisterType<KuGouPlayList, 1>("KuGouPlayList", 1, 0, "KuGouPlayList");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/MainWindow.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    return app.exec();
}
