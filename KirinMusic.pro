QT += quick
QT+=widgets
QT+=multimedia
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        clipboard.cpp \
        decode.cpp \
        fileio.cpp \
        kugou.cpp \
        lyric.cpp \
        lyricdownload.cpp \
        main.cpp \
        song.cpp

RESOURCES += ./qml/qml.qrc \
    ./resource/KirinMusic.qrc

HEADERS += \
    clipboard.h \
    decode.h \
    fileio.h \
    kugou.h \
    lyric.h \
    lyricdownload.h \
    song.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -ltag

unix|win32: LIBS += -ltag_c

unix|win32: LIBS += -lavutil
unix|win32: LIBS += -lavformat
unix|win32: LIBS += -lavcodec
unix|win32: LIBS += -lswresample


