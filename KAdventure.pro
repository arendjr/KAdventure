#-------------------------------------------------
#
# Project created by QtCreator 2010-11-13T21:45:36
#
#-------------------------------------------------

QT       += core gui declarative

TARGET = KAdventure
TEMPLATE = app


SOURCES += \
    src/editpanel.cpp \
    src/engine.cpp \
    src/imageprovider.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/util.cpp

HEADERS  += \
    src/editpanel.h \
    src/engine.h \
    src/imageprovider.h \
    src/mainwindow.h \
    src/util.h

FORMS += \
    forms/editpanel.ui

RESOURCES += \
    Content.qrc

Images.files = \
    images/background.png \
    images/blueblock.png \
    images/button.png \
    images/creature1down.png \
    images/creature1left.png \
    images/creature1right.png \
    images/creature1up.png \
    images/exit.png \
    images/explosion.png \
    images/gateclosed.png \
    images/gateopen.png \
    images/greenball.png \
    images/mine.png \
    images/player.png \
    images/raft.png \
    images/redblock.png \
    images/selector.png \
    images/transporterdown.png \
    images/transporterleft.png \
    images/transporterright.png \
    images/transporterup.png \
    images/water1.png \
    images/water2.png \
    images/water3.png
Levels.files = \
    levels/level001 \
    levels/level002 \
    levels/level003 \
    levels/level004 \
    levels/level005 \
    levels/level006 \
    levels/level007 \
    levels/level008 \
    levels/level009 \
    levels/level010 \
    levels/level011 \
    levels/level012 \
    levels/level013 \
    levels/level014 \
    levels/level015
Objects.files += \
    objects/BlueBlock.qml \
    objects/BouncingBall.qml \
    objects/BouncingBall_UpDown.qml \
    objects/Button.qml \
    objects/Creature1.qml \
    objects/Exit.qml \
    objects/Gate.qml \
    objects/Mine.qml \
    objects/Object.qml \
    objects/Player.qml \
    objects/Raft.qml \
    objects/RedBlock.qml \
    objects/TransporterDown.qml \
    objects/TransporterLeft.qml \
    objects/TransporterRight.qml \
    objects/TransporterUp.qml \
    objects/Water.qml
Resources.files = \
    src/Level.qml \
    src/Selector.qml

OTHER_FILES += $$Levels.files $$Objects.files $$Resources.files

macx {
    LIBS += -framework CoreFoundation

    Images.path = Contents/Resources/images
    Levels.path = Contents/Resources/levels
    Objects.path = Contents/Resources/objects
    Resources.path = Contents/Resources

    RESOURCES -= Content.qrc
    QMAKE_BUNDLE_DATA += Images Levels Objects Resources
}
