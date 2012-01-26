#include "util.h"

#ifdef Q_WS_MAC
#include <CoreFoundation/CFBundle.h>
#include <CoreFoundation/CFURL.h>


static QString getBundlePath() {

    CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef,
                                                  kCFURLPOSIXPathStyle);
    const char *pathPtr = CFStringGetCStringPtr(macPath,
                                                CFStringGetSystemEncoding());
    CFRelease(appUrlRef);
    CFRelease(macPath);
    return QString(pathPtr);
}
#endif

QString Util::getResourcePath(const QString &resource) {

#ifdef Q_WS_MAC
    if (resource.startsWith(":/")) {
        return getBundlePath() + "/Contents/Resources" + resource.mid(1);
    }
#endif

    return resource;
}

QUrl Util::getResourceUrl(const QString &resource) {

    if (resource.startsWith(":/")) {
#ifdef Q_WS_MAC
        return QUrl("file://" + getBundlePath() + "/Contents/Resources" + resource.mid(1));
#else
        return QUrl("qrc" + resource);
#endif
    }

    return QUrl("file:/" + resource);
}
