/* 目录 src/ui/Mainwindow.h
 * 功能 主窗口 只负责页面切换，严禁写入其他逻辑
 */


#include "ui/mainwindow.h"

#include <QFrame>
#include <QGraphicsView>


#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QStackedWidget>

#include "scene/GameScene.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("The Last Keep");
    resize(1280, 720); //v0.1的分辨率 @fish

    // stackedWidget = new QStackedWidget(this);


    m_scene = new GameScene(this); // 创建游戏场景，父对象为MainWindow

    m_view = new QGraphicsView(m_scene, this); // 创建一个视图，用来显示GameScene
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFrameShape(QFrame::NoFrame);
    m_view->setSceneRect(m_scene->sceneRect());

    setCentralWidget(m_view); // 把QGraphicsView设置为主窗口中央区域，窗口显示游戏场景

    // ========== 绑定场景按钮信号到窗口槽函数 ==========
    connect(m_scene, &GameScene::sigStartGame, this, &MainWindow::onStartGame, Qt::QueuedConnection);
    connect(m_scene, &GameScene::sigQuitGame, this, &MainWindow::onQuitGame);
}

// 点击【开始游戏】执行逻辑
void MainWindow::onStartGame()
{
    qDebug() << "开始游戏 → 加载 Tutorial 关卡";

    // 加载教程关卡地图（clear() 会清除菜单按钮，显示地图）
    m_scene->loadTutorialLevel();

    // 场景大小可能变化，同步更新视图
    m_view->setSceneRect(m_scene->sceneRect());
}

// 点击【退出游戏】执行逻辑
void MainWindow::onQuitGame()
{
    qDebug() << "退出游戏";
    this->close(); // 关闭主窗口，程序退出
}