#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QDeclarativeImageProvider>


class ImageProvider : public QDeclarativeImageProvider {

    public:
        explicit ImageProvider();

        virtual QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // IMAGEPROVIDER_H
