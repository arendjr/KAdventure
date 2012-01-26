#include "engine.h"
#include "imageprovider.h"
#include "util.h"

#include <QDebug>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeItem>
#include <QDir>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMap>
#include <QTimer>

const int timeoutMs = 400;

Engine::Engine(QObject *parent) :
    QObject(parent),
    m_engine(0),
    m_scene(0),
    m_playerItem(0),
    m_levelWidth(defaultLevelWidth),
    m_levelHeight(defaultLevelHeight),
    m_paused(false) {

    m_timer = new QTimer(this);
    m_timer->start(timeoutMs);
    connect(m_timer, SIGNAL(timeout()), this, SIGNAL(timeout()));
}

Engine::~Engine() {

    foreach (QDeclarativeComponent *component, m_objectDefs) {
        delete component;
    }
}

QDeclarativeEngine *Engine::declarativeEngine() const {

    return m_engine;
}

void Engine::setDeclarativeEngine(QDeclarativeEngine *engine) {

    m_engine = engine;
    m_engine->addImageProvider("resources", new ImageProvider());
    m_engine->rootContext()->setContextProperty("engine", this);
}

void Engine::setGraphicsScene(QGraphicsScene *scene) {

    m_scene = scene;
}

void Engine::setGraphicsView(QGraphicsView *view) {

    m_view = view;
}

bool Engine::loadLevel(const QString &fileName) {

    if (!m_engine || !m_scene || !m_view) {
        qDebug() << "No declarative engine, graphics scene or view set.";
        return false;
    }

    if (m_objectDefs.isEmpty()) {
        loadObjectDefs();
    }

    QFile file(Util::getResourcePath(fileName));
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open level" << Util::getResourcePath(fileName);
        return false;
    }

    m_scene->clear();
    m_playerItem = 0;

    QString sectionName;

    m_levelWidth = defaultLevelWidth;
    m_levelHeight = defaultLevelHeight;

    while (!file.atEnd()) {
        QString line = QString(file.readLine()).trimmed();

        if (line.startsWith("[") && line.endsWith("]")) {
            sectionName = line.mid(1, line.length() - 2);
            if (sectionName != "General") {
                if (!m_objectDefs.contains(sectionName)) {
                    sectionName.clear();
                }
            }
        }

        QString key = line.section('=', 0, 0).trimmed();
        QString value = line.section('=', 1).trimmed();

        if (sectionName.isEmpty() || key.isEmpty()) {
            continue;
        }

        if (sectionName == "General") {
            if (key == "Width") {
                m_levelWidth = value.toInt();
            } else if (key == "Height") {
                m_levelHeight = value.toInt();
            }
            continue;
        }

        if (key == "Location") {
            QStringList locations = value.split(';', QString::SkipEmptyParts);
            foreach (QString location, locations) {
                int x = location.section(',', 0, 0).toInt() - 1;
                int y = location.section(',', 1, 1).toInt() - 1;

                spawnObject(sectionName, x, y);
            }
        }
    }

    QDeclarativeComponent component(m_engine, Util::getResourceUrl(":/Level.qml"));
    QObject *object = component.create(m_engine->rootContext());
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(object);
    m_scene->addItem(item);

    if (m_playerItem) {
        m_view->ensureVisible(m_playerItem);
    } else {
        qDebug() << "I just loaded a new level, but there's no player... isn't that a little strange?";
    }

    return true;
}

bool Engine::saveLevel(const QString &fileName) {

    QMap<QString, QList<QDeclarativeItem *> > objectMap;
    QList<QGraphicsItem *> items = m_scene->items();
    foreach (QGraphicsItem *item, items) {
        QDeclarativeItem *declarativeItem = qobject_cast<QDeclarativeItem *>(item);
        if (!declarativeItem || !declarativeItem->property("gameObject").toBool()) {
            continue;
        }
        objectMap[declarativeItem->objectName()].append(declarativeItem);
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open level" << fileName;
        return false;
    }

    if (m_levelWidth != defaultLevelHeight || m_levelHeight != defaultLevelHeight) {
        file.write("[General]\n");
        file.write(QString("Width=%1\n").arg(m_levelWidth).toUtf8());
        file.write(QString("Height=%1\n").arg(m_levelHeight).toUtf8());
        file.write("\n");
    }

    foreach (QString name, objectMap.keys()) {
        file.write(QString("[%1]\n").arg(name).toUtf8());
        foreach (QDeclarativeItem *item, objectMap[name]) {
            if (item->property("positionX").toInt() >= m_levelWidth ||
                item->property("positionY").toInt() >= m_levelHeight) {
                continue;
            }
            file.write(QString("Location=%1,%2\n").arg(item->property("positionX").toInt() + 1)
                                                  .arg(item->property("positionY").toInt() + 1).toUtf8());
        }
        file.write("\n");
    }

    file.close();
    return true;
}

void Engine::movePlayer(int dx, int dy) {

    if (m_paused) {
        return;
    }

    emit playerMoved(dx, dy);

    if (m_playerItem) {
        m_view->centerOn(m_playerItem);
    }
}

int Engine::gridSize() const {

    return 48;
}

int Engine::levelWidth() const {

    return m_levelWidth;
}

void Engine::setLevelWidth(int width) {

    bool changed = (m_levelWidth != width);
    m_levelWidth = width;
    if (changed) {
        emit levelWidthChanged();
    }
}

int Engine::levelHeight() const {

    return m_levelHeight;
}

void Engine::setLevelHeight(int height) {

    bool changed = (m_levelHeight != height);
    m_levelHeight = height;
    if (changed) {
        emit levelHeightChanged();
    }
}

QMap<QString, QDeclarativeComponent *> Engine::objectComponents() {

    return m_objectDefs;
}

QList<QObject *> Engine::objectsAt(int x, int y) const {

    QList<QObject *> objects;
    QList<QGraphicsItem *> items = m_scene->items(QPointF((x + 0.5) * gridSize(), (y + 0.5) * gridSize()));
    foreach (QGraphicsItem *item, items) {
        QDeclarativeItem *declarativeItem = qobject_cast<QDeclarativeItem *>(item);
        if (!declarativeItem || !declarativeItem->property("gameObject").toBool()) {
            continue;
        }
        objects.append(declarativeItem);
    }
    return objects;
}

void Engine::spawnObject(const QString &name, int x, int y) {

    if (!m_objectDefs.contains(name)) {
        qDebug() << "Cannot find object " << name;
        return;
    }

    QObject *object = m_objectDefs[name]->create(m_engine->rootContext());
    if (!object) {
        qDebug() << "Could not create object " << name;
        return;
    }

    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(object);
    item->setObjectName(name);
    item->setProperty("positionX", x);
    item->setProperty("positionY", y);
    m_scene->addItem(item);

    if (item->property("player").toBool()) {
        m_playerItem = item;
    }

    QByteArray signature = QMetaObject::normalizedSignature("onPlayerMoved(QVariant, QVariant)");
    if (item->metaObject()->indexOfMethod(signature.constData()) != -1) {
        connect(this, SIGNAL(playerMoved(QVariant, QVariant)), item, SLOT(onPlayerMoved(QVariant, QVariant)));
    }

    signature = QMetaObject::normalizedSignature("onTimeout()");
    if (item->metaObject()->indexOfMethod(signature.constData()) != -1) {
        connect(this, SIGNAL(timeout()), item, SLOT(onTimeout()));
    }

    signature = QMetaObject::normalizedSignature("onEventRaised(QVariant, QVariant)");
    if (item->metaObject()->indexOfMethod(signature.constData()) != -1) {
        connect(this, SIGNAL(eventRaised(QVariant, QVariant)), item, SLOT(onEventRaised(QVariant, QVariant)));
    }
}

void Engine::removeObject(QObject *object) {

    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(object);
    if (!item) {
        qDebug() << "Not a valid object to remove.";
    }

    m_scene->removeItem(item);
    item->deleteLater();
}

void Engine::pause() {

    m_timer->stop();

    m_paused = true;
}

void Engine::resume() {

    m_timer->start(timeoutMs);

    m_paused = false;
}

void Engine::loadObjectDefs() {

    QDir dir(Util::getResourcePath(":/objects/"));
    foreach (QString object, dir.entryList()) {
        if (object == "." || object == "..") {
            continue;
        }

        QString name = object.section('.', 0, -2);
        if (name == "Object") {
            continue;
        }

        QDeclarativeComponent *component = new QDeclarativeComponent(m_engine, Util::getResourceUrl(":/objects/" + object));
        if (component->isError()) {
            foreach (QDeclarativeError error, component->errors()) {
                qDebug() << error.toString();
            }
            delete component;
            continue;
        }

        m_objectDefs[name] = component;
    }
}
