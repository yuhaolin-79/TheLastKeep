#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsView; //显示窗口，把画布显示出来
class GameScene; //画布

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QGraphicsView *m_view = nullptr; //显示器
    GameScene *m_scene = nullptr; //游戏场景
};
#endif // MAINWINDOW_H
