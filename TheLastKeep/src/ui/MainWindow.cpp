/* 目录 src/ui/Mainwindow.h
 * 功能 主窗口 只负责页面切换，严禁写入其他逻辑
 */


#include "ui/Mainwindow.h"

#include "ui/pages/StartPage.h"
#include "ui/pages/LevelSelectPage.h"
#include "ui/pages/GamePage.h"
#include "ui/pages/ResultPage.h"

#include "common/GameConstants.h"

// #include <QFrame>
// #include <QGraphicsView>


// #include <QPushButton>
// #include <QVBoxLayout>
// #include <QLabel>
// #include <QPainter>

#include <QDebug>
#include <QStackedWidget>

// #include "scene/GameScene.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupConnections();

    showStartPage();
}

void MainWindow::setupUi() {
    setWindowTitle("The Last Keep");
    resize(GameConstants::WindowWidth, GameConstants::WindowHeight);

    m_stack = new QStackedWidget(this);

    m_startPage = new StartPage(this);
    m_levelSelectPage = new LevelSelectPage(this);
    m_gamePage = new GamePage(this);
    m_resultPage = new ResultPage(this);

    m_stack->addWidget(m_startPage);
    m_stack->addWidget(m_levelSelectPage);
    m_stack->addWidget(m_gamePage);
    m_stack->addWidget(m_resultPage);

    setCentralWidget(m_stack);
}

void MainWindow::setupConnections() {
    connect(m_startPage, &StartPage::startClicked, this, [=]{
        startLevel(0);
    });

    connect(m_startPage, &StartPage::levelSelectClicked,
            this, &MainWindow::showLevelSelectPage);

    connect(m_startPage, &StartPage::quitClicked,
            this, &MainWindow::close);

    connect(m_levelSelectPage, &LevelSelectPage::levelSelected,
            this, &MainWindow::startLevel);

    connect(m_levelSelectPage, &LevelSelectPage::backClicked,
            this, &MainWindow::showStartPage);

    connect(m_gamePage, &GamePage::requestBackToMenu,
            this, &MainWindow::backToMenu);

    connect(m_gamePage, &GamePage::gameFinished,
            this, &MainWindow::showResultPage);

    connect(m_resultPage, &ResultPage::backToMenuClicked,
            this, &MainWindow::backToMenu);

    connect(m_resultPage, &ResultPage::restartClicked,
            this, [=]() {
                startLevel(m_lastLevelId);
            });

    connect(m_resultPage, &ResultPage::nextLevelClicked,
            this, [=](int nextLevelId) {
                startLevel(nextLevelId);
            });
}

void MainWindow::switchTo(QWidget *page){
    if (!page) {
        return;
    }

    if (m_currentPage == page) {
        return;
    }

    // 旧页面如果是 GamePage，需要通知它“你要被隐藏了”
    // GamePage 内部会暂停 QTimer
    if (auto oldGamePage = qobject_cast<GamePage *>(m_currentPage)) {
        oldGamePage->onPageHiddenByStack();
    }

    m_stack->setCurrentWidget(page);
    m_currentPage = page;

    // 新页面如果是 GamePage，需要通知它“你又显示出来了”
    // GamePage 内部会判断是否需要恢复 QTimer
    if (auto newGamePage = qobject_cast<GamePage *>(page)) {
        newGamePage->onPageShownByStack();
    }
}

void MainWindow::showStartPage(){
    switchTo(m_startPage);
}

void MainWindow::showLevelSelectPage(){
    switchTo(m_levelSelectPage);
}

void MainWindow::startLevel(int levelId){
    qDebug() << "MainWindow start level:" << levelId;

    m_lastLevelId = levelId;

    // 先切到 GamePage
    switchTo(m_gamePage);

    // 再让 GamePage 开始关卡
    // MainWindow 不直接碰 GameScene 和 QTimer
    m_gamePage->startLevel(levelId);
}

void MainWindow::showResultPage(bool win, int score){
    m_resultPage->setResult(win, score, m_lastLevelId);
    switchTo(m_resultPage);
}

void MainWindow::backToMenu(){
    // 返回主菜单时，必须彻底停止游戏
    // 先停 QTimer，再清场景，再切页面
    if (m_gamePage) {
        m_gamePage->stopGame();
    }

    switchTo(m_startPage);
}