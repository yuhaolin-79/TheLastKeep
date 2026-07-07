#include "ui/mainwindow.h"

#include <QFrame>
#include <QGraphicsView>
#include <QPainter>

#include "game/GameScene.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("The Last Keep");
    resize(1280, 720);

    m_scene = new GameScene(this); // 创建游戏场景，父对象为MainWindow

    m_view = new QGraphicsView(m_scene, this); // 创建一个视图，用来显示GameScene
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFrameShape(QFrame::NoFrame);
    m_view->setSceneRect(m_scene->sceneRect());

    setCentralWidget(m_view); // 把QGraphicsView设置为主窗口中央区域，窗口显示游戏场景
}