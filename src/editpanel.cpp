#include "editpanel.h"
#include "engine.h"
#include "ui_editpanel.h"
#include "util.h"

#include <QDebug>
#include <QMouseEvent>
#include <QtDeclarative>

const int INVENTORY_WIDTH = 3;
const int INVENTORY_HEIGHT = 6;

EditPanel::EditPanel(Engine *engine, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPanel()),
    m_engine(engine),
    m_selectorItem(0) {

    ui->setupUi(this);

    connect(ui->saveButton, SIGNAL(clicked()), SIGNAL(saveButtonClicked()));
    connect(ui->widthSpinBox, SIGNAL(valueChanged(int)), SIGNAL(widthChanged(int)));
    connect(ui->heightSpinBox, SIGNAL(valueChanged(int)), SIGNAL(heightChanged(int)));

    ui->widthSpinBox->setValue(m_engine->levelWidth());
    ui->heightSpinBox->setValue(m_engine->levelHeight());

    ui->objectsView->setFixedSize(INVENTORY_WIDTH * engine->gridSize(),
                                  INVENTORY_HEIGHT * engine->gridSize());
    ui->objectsView->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->objectsView->setSceneRect(0, 0,
                                  INVENTORY_WIDTH * engine->gridSize(),
                                  INVENTORY_HEIGHT * engine->gridSize());

    ui->objectsView->installEventFilter(this);

    initObjectsView();
}

EditPanel::~EditPanel() {

    delete ui;
}

QString EditPanel::selectedObject() const {

    return m_selectedObject;
}

bool EditPanel::eventFilter(QObject */*object*/, QEvent *event) {

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() & Qt::LeftButton) {
            onObjectSelected(mouseEvent);
            return true;
        }
    }

    return false;
}

void EditPanel::initObjectsView() {

    QDeclarativeEngine *declarativeEngine = m_engine->declarativeEngine();
    QDeclarativeComponent component(declarativeEngine, Util::getResourceUrl(":/Level.qml"));
    QObject *object = component.create(declarativeEngine->rootContext());
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(object);
    ui->objectsView->scene()->addItem(item);

    QMap<QString, QDeclarativeComponent *> objectComponents = m_engine->objectComponents();
    m_objectNames = objectComponents.keys();
    m_objectNames.sort();

    int row = 0;
    int column = 1;
    foreach (QString name, m_objectNames) {
        QDeclarativeComponent *component = objectComponents[name];

        QObject *object = component->create(declarativeEngine->rootContext());
        if (!object) {
            qDebug() << "Could not create object " << name;
            return;
        }

        QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(object);
        item->setObjectName(name);
        item->setProperty("positionX", column);
        item->setProperty("positionY", row);
        ui->objectsView->scene()->addItem(item);

        column++;
        if (column >= INVENTORY_WIDTH) {
            row++;
            column = 0;
        }
    }
}

void EditPanel::onObjectSelected(QMouseEvent *event) {

    int gridSize = m_engine->gridSize();
    int column = event->pos().x() / gridSize;
    int row = event->pos().y() / gridSize;

    if (column == 0 && row == 0) {
        m_selectedObject = "grass";
    } else {
        QString selectedObject;
        QList<QGraphicsItem *> items = ui->objectsView->scene()->items(QPointF((column + 0.5) * gridSize, (row + 0.5) * gridSize));
        foreach (QGraphicsItem *item, items) {
            QDeclarativeItem *declarativeItem = qobject_cast<QDeclarativeItem *>(item);
            if (!declarativeItem || !declarativeItem->property("gameObject").toBool()) {
                continue;
            }
            selectedObject = declarativeItem->objectName();
            break;
        }

        if (selectedObject.isNull()) {
            return;
        }

        m_selectedObject = selectedObject;
    }

    if (!m_selectorItem) {
        QDeclarativeComponent *component = new QDeclarativeComponent(m_engine->declarativeEngine(),
                                                                     Util::getResourceUrl(":/Selector.qml"));
        if (!component || component->isError()) {
            qDebug() << "Could not load selector item definition";
            return;
        }

        QObject *object = component->create(m_engine->declarativeEngine()->rootContext());
        if (!object) {
            qDebug() << "Could not create selector item";
            return;
        }

        m_selectorItem = qobject_cast<QDeclarativeItem *>(object);
        ui->objectsView->scene()->addItem(m_selectorItem);
    }

    m_selectorItem->setProperty("positionX", column);
    m_selectorItem->setProperty("positionY", row);
}
