#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class EditPanel;
class Engine;

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void openLevel();
        void reloadLevel();
        void saveLevel();

        void startPlayMode();
        void startEditMode();

    protected:
        void keyPressEvent(QKeyEvent *event);

        bool eventFilter(QObject *object, QEvent *event);

    private slots:
        void onPlayerKilled();

        void onLevelFinished();

        void onLevelDimensionsChanged();

        void onWidthChanged(int newWidth);
        void onHeightChanged(int newHeight);

    private:
        Engine *m_engine;

        QString m_fileName;
        int m_currentLevel;

        EditPanel *m_editPanel;

        void initMenu();

        void loadLevel(const QString &fileName);

        void placeObject(QMouseEvent *event);
};

#endif // MAINWINDOW_H
