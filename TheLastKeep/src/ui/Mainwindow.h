/* 目录 src/ui/Mainwindow.h
 * 功能 主窗口 只负责页面切换，严禁写入其他逻辑
 * 管理 Pages
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsView; //显示窗口，把画布显示出来
class GameScene; //画布
class QPushButton;
class QWidget;
class GamePage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onStartGame();
    void onQuitGame();
    // void onStartGameClicked(); // 点击开始游戏按钮

private:
    QGraphicsView *m_view = nullptr; //显示器
    GameScene *m_scene = nullptr; //游戏场景
};
#endif // MAINWINDOW_H
