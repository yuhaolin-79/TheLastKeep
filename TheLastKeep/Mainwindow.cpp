#include "Mainwindow.h"

#include <QFrame>
#include <QGraphicsView>
#include <QPainter>
#include <QDebug>

#include "GameScene.h"


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

    // ========== 新增：绑定场景按钮信号到窗口槽函数 ==========
    connect(m_scene, &GameScene::sigStartGame, this, &MainWindow::onStartGame);
    connect(m_scene, &GameScene::sigQuitGame, this, &MainWindow::onQuitGame);
}

// 点击【开始游戏】执行逻辑
void MainWindow::onStartGame()
{
    qDebug() << "触发开始游戏逻辑";
    // 后续在这里写：切换战斗场景、加载关卡、隐藏主菜单按钮等游戏启动代码
}

// 点击【退出游戏】执行逻辑
void MainWindow::onQuitGame()
{
    qDebug() << "退出游戏";
    this->close(); // 关闭主窗口，程序退出
}