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
#include "ui/widgets/HUDWidget.h"

#include <QGraphicsView>
#include <QVBoxLayout>
#include <QFrame>
#include <QPainter>
#include <QHideEvent>
#include <QResizeEvent>
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

    m_scene = new GameScene(this);

    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFrameShape(QFrame::NoFrame);
    m_view->setAcceptDrops(true);
    m_view->setSceneRect(0, 0,
                         GameConstants::WindowWidth,
                         GameConstants::WindowHeight);

    m_controller = new GameController(m_scene, this);

    // HUD 作为 GamePage 的覆盖层存在，不再进入主布局。
    // 这样游戏地图仍占满整个页面，HUD 和暂停/返回按钮视觉上浮在背景上。
    m_hud = new HUDWidget(this);
    m_hud->raise();

    mainLayout->addWidget(m_view);
    updateHudGeometry();
}

void GamePage::setupConnections()
{
    connect(m_hud, &HUDWidget::pauseClicked,
            this, &GamePage::handlePauseOrResumeClicked);

    connect(m_hud, &HUDWidget::backClicked,
            this, &GamePage::handleBackClicked);

    connect(m_hud, &HUDWidget::towerSelected,
            m_controller, &GameController::setSelectedTowerType);
connect(m_controller, &GameController::statusChanged,
            this, [this](GameStatus status) {
                if(!m_hud){
                    return;
                }
                m_hud->setPaused(status == GameStatus::Paused);
            });

    connect(m_controller, &GameController::statsChanged,
            this, [this](int gold, int hp, int maxHp, int wave, int totalWave) {
                if(m_hud){
                    m_hud->setStats(gold, hp, maxHp, wave, totalWave);
                }
            });

    connect(m_controller, &GameController::gameFinished,
            this, &GamePage::handleGameFinished);
}

void GamePage::startLevel(int levelId)
{
    stopGame();

    m_currentLevelId = levelId;
    m_started = true;
    m_shouldResumeWhenShown = false;

    if (!m_controller->loadLevel(levelId)) {
        qWarning() << "GamePage::startLevel failed";
        return;
    }

    m_view->setSceneRect(m_scene->sceneRect());
    updateHudGeometry();
    m_hud->raise();

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

    pauseBecauseHidden();
}

void GamePage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    updateHudGeometry();
    if(m_hud){
        m_hud->raise();
    }
    resumeBecauseShown();
}

void GamePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateHudGeometry();
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

void GamePage::updateHudGeometry()
{
    if(!m_hud){
        return;
    }

    const int hudHeight = 76;
    const int sideMargin = 14;
    m_hud->setGeometry(sideMargin, 8, qMax(0, width() - sideMargin * 2), hudHeight);
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