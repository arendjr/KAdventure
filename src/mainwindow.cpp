#include "editpanel.h"
#include "engine.h"
#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDeclarativeView>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMenuBar>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_engine(new Engine(this)),
    m_currentLevel(1) {

    QDeclarativeView *view = new QDeclarativeView(this);
    view->setFocusPolicy(Qt::NoFocus);

    connect(m_engine, SIGNAL(playerKilled()), SLOT(onPlayerKilled()));
    connect(m_engine, SIGNAL(levelFinished()), SLOT(onLevelFinished()));
    connect(m_engine, SIGNAL(levelWidthChanged()), SLOT(onLevelDimensionsChanged()));
    connect(m_engine, SIGNAL(levelHeightChanged()), SLOT(onLevelDimensionsChanged()));

    m_engine->setDeclarativeEngine(view->engine());
    m_engine->setGraphicsScene(view->scene());
    m_engine->setGraphicsView(view);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    setCentralWidget(view);
    view->setFixedSize(QSize(768, 768));
    view->setTransformationAnchor(QGraphicsView::NoAnchor);
    view->setSceneRect(0, 0, 768, 768);
    view->installEventFilter(this);

    initMenu();

    loadLevel(QString().sprintf(":/levels/level%03d", m_currentLevel));
}

MainWindow::~MainWindow() {
}

void MainWindow::openLevel() {

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Level"));
    if (!fileName.isNull()) {
        loadLevel(fileName);
    }
}

void MainWindow::reloadLevel() {

    loadLevel(m_fileName);
}

void MainWindow::saveLevel() {

    if (m_fileName.startsWith(":/")) {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Level"));
        if (fileName.isNull()) {
            return;
        }
        m_fileName = fileName;
    }

    m_engine->saveLevel(m_fileName);
}

void MainWindow::startPlayMode() {

    m_editPanel = 0;

    m_engine->resume();
}

void MainWindow::startEditMode() {

    if (m_editPanel) {
        return;
    }

    reloadLevel();

    m_engine->pause();

    m_editPanel = new EditPanel(m_engine, this);
    m_editPanel->setWindowFlags(Qt::Tool);
    m_editPanel->setAttribute(Qt::WA_DeleteOnClose);
    m_editPanel->move(pos().x() + width() + 20, pos().y() + 20);
    m_editPanel->show();

    connect(m_editPanel, SIGNAL(saveButtonClicked()), SLOT(saveLevel()));
    connect(m_editPanel, SIGNAL(destroyed()), SLOT(startPlayMode()));
    connect(m_editPanel, SIGNAL(widthChanged(int)), SLOT(onWidthChanged(int)));
    connect(m_editPanel, SIGNAL(heightChanged(int)), SLOT(onHeightChanged(int)));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {

    switch (event->key()) {
        case Qt::Key_Left:
            m_engine->movePlayer(-1, 0);
            break;
        case Qt::Key_Right:
            m_engine->movePlayer(1, 0);
            break;
        case Qt::Key_Down:
            m_engine->movePlayer(0, 1);
            break;
        case Qt::Key_Up:
            m_engine->movePlayer(0, -1);
            break;
        case Qt::Key_F5:
            reloadLevel();
            break;
        default:
            QMainWindow::keyPressEvent(event);
     }
}

bool MainWindow::eventFilter(QObject */*object*/, QEvent *event) {

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() & Qt::LeftButton) {
            if (m_editPanel) {
                placeObject(mouseEvent);
            }
            return true;
        }
    }

    return false;
}

void MainWindow::onPlayerKilled() {

    reloadLevel();
}

void MainWindow::onLevelFinished() {

    m_currentLevel++;
    loadLevel(QString().sprintf(":/levels/level%03d", m_currentLevel));
}

void MainWindow::onLevelDimensionsChanged() {

    int width = m_engine->levelWidth() * m_engine->gridSize();
    int height = m_engine->levelHeight() * m_engine->gridSize();

    centralWidget()->setFixedSize(width, height);
    setFixedSize(width, height);
}

void MainWindow::onWidthChanged(int newWidth) {

    m_engine->setLevelWidth(newWidth);
}

void MainWindow::onHeightChanged(int newHeight) {

    m_engine->setLevelHeight(newHeight);
}

void MainWindow::initMenu() {

    QMenu *fileMenu = new QMenu(this);
    fileMenu->setTitle(tr("&Level"));
    fileMenu->addAction(tr("&Open..."), this, SLOT(openLevel()), QKeySequence(Qt::CTRL | Qt::Key_O));
    fileMenu->addAction(tr("&Edit"), this, SLOT(startEditMode()), QKeySequence(Qt::CTRL | Qt::Key_E));
    fileMenu->addAction(tr("&Restart"), this, SLOT(reloadLevel()), QKeySequence(Qt::CTRL | Qt::Key_R));
    fileMenu->addAction(tr("&Quit"), qApp, SLOT(quit()), QKeySequence::Quit);
    menuBar()->addMenu(fileMenu);
}

void MainWindow::loadLevel(const QString &fileName) {

    m_fileName = fileName;
    m_engine->loadLevel(m_fileName);
}

void MainWindow::placeObject(QMouseEvent *event) {

    QString selectedObject = m_editPanel->selectedObject();

    int x = event->pos().x() / m_engine->gridSize();
    int y = event->pos().y() / m_engine->gridSize();

    bool alreadyThere = false;
    QList<QObject *> objects = m_engine->objectsAt(x, y);
    foreach (QObject *object, objects) {
        if (selectedObject == "grass") {
            m_engine->removeObject(object);
        } else {
            if (object->objectName() == selectedObject) {
                alreadyThere = true;
            }
        }
    }

    if (!alreadyThere && selectedObject != "grass") {
        m_engine->spawnObject(selectedObject, x, y);
    }
}
