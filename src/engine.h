#ifndef ENGINE_H
#define ENGINE_H

#include <QMap>
#include <QObject>
#include <QVariant>

class QDeclarativeComponent;
class QDeclarativeEngine;
class QDeclarativeItem;
class QGraphicsScene;
class QGraphicsView;
class QTimer;


class Engine : public QObject {

    Q_OBJECT

    Q_PROPERTY (int gridSize READ gridSize NOTIFY gridSizeChanged);

    Q_PROPERTY (int levelWidth READ levelWidth WRITE setLevelWidth NOTIFY levelWidthChanged);
    Q_PROPERTY (int levelHeight READ levelHeight WRITE setLevelHeight NOTIFY levelHeightChanged);

    public:
        static const int defaultLevelWidth = 16;
        static const int defaultLevelHeight = 16;

        explicit Engine(QObject *parent = 0);
        virtual ~Engine();

        QDeclarativeEngine *declarativeEngine() const;
        void setDeclarativeEngine(QDeclarativeEngine *engine);

        void setGraphicsScene(QGraphicsScene *scene);

        void setGraphicsView(QGraphicsView *view);

        bool loadLevel(const QString &fileName);

        bool saveLevel(const QString &fileName);

        void movePlayer(int dx, int dy);

        int gridSize() const;

        int levelWidth() const;
        void setLevelWidth(int width);

        int levelHeight() const;
        void setLevelHeight(int height);

        QMap<QString, QDeclarativeComponent *> objectComponents();

        Q_INVOKABLE QList<QObject *> objectsAt(int x, int y) const;

        Q_INVOKABLE void spawnObject(const QString &name, int x, int y);

        Q_INVOKABLE void removeObject(QObject *object);

        void pause();
        void resume();

    signals:
        void playerMoved(QVariant dx, QVariant dy);

        void playerKilled();

        void levelFinished();

        void timeout();

        void eventRaised(QVariant eventName, QVariant data = QVariant());

        void gridSizeChanged();

        void levelWidthChanged();
        void levelHeightChanged();

    private:
        QDeclarativeEngine *m_engine;
        QGraphicsScene *m_scene;
        QGraphicsView *m_view;

        QMap<QString, QDeclarativeComponent *> m_objectDefs;

        QDeclarativeItem *m_playerItem;

        QTimer *m_timer;

        int m_levelWidth;
        int m_levelHeight;

        bool m_paused;

        void loadObjectDefs();
};

#endif // ENGINE_H
