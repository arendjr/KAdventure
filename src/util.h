#ifndef UTILITY_H
#define UTILITY_H

#include <QString>
#include <QUrl>


class Util {

    public:
        static QString getResourcePath(const QString &resource);

        static QUrl getResourceUrl(const QString &resource);
};

#endif // UTILITY_H
