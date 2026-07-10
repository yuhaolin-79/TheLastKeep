/* 目录 src/ui/Mainwindow.h
 * 功能 主窗口 只负责页面切换，严禁写入其他逻辑
 * 管理 Pages
 * @fish 2026/07/10 VERSION 1.0
 */

#pragma once

#include <QMainWindow>

class QStackedWidget;
// class QGraphicsView; //显示窗口，把画布显示出来
// class GameScene; //画布
// class QPushButton;
class QWidget;

class StartPage;
class GamePage;
class LevelSelectPage;
class GamePage;
class ResultPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void showStartPage();
    void showLevelSelectPage();
    void startLevel(int levelId);
    void showResultPage(bool win, int score);
    void backToMenu();

private:
    void setupUi();
    void setupConnections();

    // 页面切换统一走 下面的 switchTo()
    // 统一处理QStackedWidget切换页面时，隐藏界面暂停 QTimer
    void switchTo(QWidget *page);

private:
    QStackedWidget *m_stack = nullptr;

    StartPage *m_startPage = nullptr;
    LevelSelectPage *m_levelSelectPage = nullptr;
    GamePage *m_gamePage = nullptr;
    ResultPage *m_resultPage = nullptr;

    QWidget *m_currentPage = nullptr;

    int m_lastLevelId = 0;
};

