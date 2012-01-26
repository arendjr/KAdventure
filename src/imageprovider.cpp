#include "imageprovider.h"
#include "util.h"


ImageProvider::ImageProvider() :
    QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap) {
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {

    QPixmap pixmap;
    pixmap.load(Util::getResourcePath(":/images/" + id));

    if (size) {
        size->setWidth(pixmap.width());
        size->setHeight(pixmap.height());
    }

    if (requestedSize.isValid()) {
        pixmap = pixmap.scaled(requestedSize);
    }

    return pixmap;
}
