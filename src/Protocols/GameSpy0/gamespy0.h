#ifndef GAMESPY0_H
#define GAMESPY0_H

#include <QHash>
#include <QMultiHash>
#include "Protocols/overrides.h"

QMultiHash<QString, QString> parseGameSpy0Buffer(const QString &bufferString);

#endif // GAMESPY0_H
