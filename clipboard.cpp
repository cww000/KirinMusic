#include "clipboard.h"
#include <QDebug>

Clipboard::Clipboard(QObject *parent) : QObject(parent){
    clipboard = QGuiApplication::clipboard();
}

void Clipboard::setText(QString text)
{
    clipboard->setText(text,QClipboard::Clipboard);
}
