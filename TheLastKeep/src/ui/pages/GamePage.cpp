/* 目录 src/ui/pages
 * 功能 游戏页面容器
 * 负责创建和组合 QGraphicsView GameScene HUDWidget
 *     GameController CardSelectWidget PauseOverlay
 * 是 UI 层和逻辑层的连接点
 * 需要
 * 开始关卡
 * 重开关卡
 * 返回菜单
 * 页面隐藏时暂停 QTimer
 * 页面显示时恢复 QTimer
 * 接收游戏结束信号
 */

#include "ui/pages/GamePage.h"

#include "scene/GameScene.h"
#include "core/GameController.h"
#include "common/GameConstants.h"

#include <QGraphicsView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QPainter>
#include <QHideEvent>
#include <QShowEvent>
#include <QDebug>

GamePage::GamePage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
}

GamePage::~GamePage()
{
    stopGame();
}

void GamePage::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 第一阶段临时控制栏
    // 后面可以整体替换成 HUDWidget
    auto *topBar = new QWidget(this);
    topBar->setFixedHeight(48);

    auto *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(12, 6, 12, 6);

    m_pauseButton = new QPushButton("暂停", topBar);
    m_backButton = new QPushButton("返回主菜单", topBar);

    topLayout->addWidget(m_pauseButton);
    topLayout->addWidget(m_backButton);
    topLayout->addStretch();

    m_scene = new GameScene(this);

    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFrameShape(QFrame::NoFrame);
    m_view->setSceneRect(0, 0,
                         GameConstants::WindowWidth,
                         GameConstants::WindowHeight);

    m_controller = new GameController(m_scene, this);

    mainLayout->addWidget(topBar);
    mainLayout->addWidget(m_view);
}

void GamePage::setupConnections()
{
    connect(m_pauseButton, &QPushButton::clicked,
            this, &GamePage::handlePauseOrResumeClicked);

    connect(m_backButton, &QPushButton::clicked,
            this, &GamePage::handleBackClicked);

    connect(m_controller, &GameController::statusChanged,
            this, [this](GameStatus status) {
                if (status == GameStatus::Running) {
                    m_pauseButton->setText("暂停");
                } else if (status == GameStatus::Paused) {
                    m_pauseButton->setText("继续");
                }
            });

    connect(m_controller, &GameController::gameFinished,
            this, &GamePage::handleGameFinished);
}

void GamePage::startLevel(int levelId)
{
    // 开始新关卡前，先彻底停止旧游戏
    stopGame();

    m_currentLevelId = levelId;
    m_started = true;
    m_shouldResumeWhenShown = false;

    if (!m_controller->loadLevel(levelId)) {
        qWarning() << "GamePage::startLevel failed";
        return;
    }

    m_view->setSceneRect(m_scene->sceneRect());

    m_controller->startGame();
}

void GamePage::restartLevel()
{
    if (m_currentLevelId < 0) {
        return;
    }

    startLevel(m_currentLevelId);
}

void GamePage::stopGame()
{
    if (!m_controller) {
        return;
    }

    // 关键顺序：
    // 1. 停 QTimer
    // 2. 清游戏场景
    // 3. 重置 GamePage 状态
    m_controller->stopGame();
    m_controller->clearGame();

    m_started = false;
    m_shouldResumeWhenShown = false;
}

void GamePage::onPageHiddenByStack()
{
    pauseBecauseHidden();
}

void GamePage::onPageShownByStack()
{
    resumeBecauseShown();
}

void GamePage::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);

    // 保险：即使不是 MainWindow::switchTo 调用，也能暂停
    pauseBecauseHidden();
}

void GamePage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    // 保险：页面重新显示时尝试恢复
    resumeBecauseShown();
}

void GamePage::pauseBecauseHidden()
{
    if (!m_started || !m_controller) {
        return;
    }

    if (m_controller->isRunning()) {
        m_shouldResumeWhenShown = true;
        m_controller->pauseForPageHidden();
    }
}

void GamePage::resumeBecauseShown()
{
    if (!m_started || !m_controller) {
        return;
    }

    if (!m_shouldResumeWhenShown) {
        return;
    }

    m_controller->resumeForPageShown();
    m_shouldResumeWhenShown = false;
}

void GamePage::handlePauseOrResumeClicked()
{
    if (!m_controller) {
        return;
    }

    if (m_controller->isRunning()) {
        m_controller->pauseGame();
    } else {
        m_controller->resumeGame();
    }
}

void GamePage::handleBackClicked()
{
    stopGame();
    emit requestBackToMenu();
}

void GamePage::handleGameFinished(bool win, int score)
{
    stopGame();
    emit gameFinished(win, score);
}