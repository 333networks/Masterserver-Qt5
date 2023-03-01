#ifndef LOGPRIMITIVE_H
#define LOGPRIMITIVE_H

#include <QTextStream>

inline QTextStream& logPrimitive()
{
        static QTextStream r{stdout};
        return r;
}

#endif // LOGPRIMITIVE_H
