#ifndef EDITPANEL_H
#define EDITPANEL_H

#include <QMouseEvent>
#include <QWidget>

namespace Ui {
    class EditPanel;
}

class Engine;

class QDeclarativeItem;

class EditPanel : public QWidget {

    Q_OBJECT

    public:
        explicit EditPanel(Engine *engine, QWidget *parent = 0);
        ~EditPanel();

        QString selectedObject() const;

    signals:
        void widthChanged(int newWidth);
        void heightChanged(int newHeight);

        void saveButtonClicked();

    protected:
         bool eventFilter(QObject *object, QEvent *event);

    private:
        Ui::EditPanel *ui;

        Engine *m_engine;

        QStringList m_objectNames;
        QString m_selectedObject;
        QDeclarativeItem *m_selectorItem;

        void initObjectsView();

        void onObjectSelected(QMouseEvent *event);
};

#endif // EDITPANEL_H
