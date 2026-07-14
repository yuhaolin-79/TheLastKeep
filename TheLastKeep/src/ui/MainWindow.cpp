#include "ui/MainWindow.h"

#include "common/GameConstants.h"
#include "data/SoundManager.h"
#include "level/LevelManager.h"
#include "ui/pages/GamePage.h"
#include "ui/pages/LevelSelectPage.h"
#include "ui/pages/ResultPage.h"
#include "ui/pages/SettingsPage.h"
#include "ui/pages/StartPage.h"
#include "ui/pages/StoryPage.h"

#include <QAbstractButton>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QStackedWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupConnections();

    SoundManager::instance().startBackgroundMusic();
    showStartPage();
}

void MainWindow::setupUi()
{
    setWindowTitle("The Last Keep");
    resize(GameConstants::WindowWidth, GameConstants::WindowHeight);

    m_stack = new QStackedWidget(this);

    m_startPage = new StartPage(this);
    m_levelSelectPage = new LevelSelectPage(this);
    m_settingsPage = new SettingsPage(this);
    m_storyPage = new StoryPage(this);
    m_gamePage = new GamePage(this);
    m_resultPage = new ResultPage(this);

    m_settingsPage->setMusicEnabled(SoundManager::instance().isMusicEnabled());

    m_stack->addWidget(m_startPage);
    m_stack->addWidget(m_levelSelectPage);
    m_stack->addWidget(m_settingsPage);
    m_stack->addWidget(m_storyPage);
    m_stack->addWidget(m_gamePage);
    m_stack->addWidget(m_resultPage);

    setCentralWidget(m_stack);
    qApp->installEventFilter(this);
}

void MainWindow::setupConnections()
{
    connect(m_startPage, &StartPage::startClicked, this, [this] {
        prepareStoryForLevel(0, m_startPage);
    });

    connect(m_startPage, &StartPage::levelSelectClicked,
            this, &MainWindow::showLevelSelectPage);

    connect(m_startPage, &StartPage::settingsClicked,
            this, &MainWindow::showSettingsPage);

    connect(m_startPage, &StartPage::quitClicked,
            this, &MainWindow::close);

    connect(m_levelSelectPage, &LevelSelectPage::levelSelected,
            this, [this](int levelId) {
                prepareStoryForLevel(levelId, m_levelSelectPage);
            });

    connect(m_levelSelectPage, &LevelSelectPage::backClicked,
            this, &MainWindow::showStartPage);

    connect(m_settingsPage, &SettingsPage::musicEnabledChanged,
            this, [](bool enabled) {
                SoundManager::instance().setMusicEnabled(enabled);
            });

    connect(m_settingsPage, &SettingsPage::backClicked,
            this, &MainWindow::showStartPage);

    connect(m_storyPage, &StoryPage::continueClicked,
            this, [this] {
                queueStartLevel(m_pendingStoryLevelId);
            });

    connect(m_storyPage, &StoryPage::backClicked,
            this, [this] {
                switchTo(m_storyReturnPage ? m_storyReturnPage : static_cast<QWidget *>(m_startPage));
            });

    connect(m_gamePage, &GamePage::requestBackToMenu,
            this, &MainWindow::backToMenu);

    connect(m_gamePage, &GamePage::gameFinished,
            this, &MainWindow::showResultPage);

    connect(m_resultPage, &ResultPage::backToMenuClicked,
            this, &MainWindow::showLevelSelectPage);

    connect(m_resultPage, &ResultPage::restartClicked,
            this, [this]() {
                queueStartLevel(m_lastLevelId);
            });

    connect(m_resultPage, &ResultPage::nextLevelClicked,
            this, [this](int nextLevelId) {
                if (nextLevelId < 0 || nextLevelId > 3) {
                    showStartPage();
                    return;
                }
                prepareStoryForLevel(nextLevelId, m_resultPage);
            });

    connect(m_resultPage, &ResultPage::autoReturnToMainRequested,
            this, &MainWindow::showStartPage);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    auto *button = qobject_cast<QAbstractButton *>(watched);
    if (button && button->isEnabled() && event->type() == QEvent::MouseButtonPress) {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            SoundManager::instance().playButtonClick();
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::switchTo(QWidget *page)
{
    if (!page) {
        return;
    }

    if (m_currentPage == page) {
        return;
    }

    if (m_currentPage == m_resultPage) {
        m_resultPage->stopAutoReturnTimer();
    }

    if (auto oldGamePage = qobject_cast<GamePage *>(m_currentPage)) {
        oldGamePage->onPageHiddenByStack();
    }

    m_stack->setCurrentWidget(page);
    m_currentPage = page;

    if (auto newGamePage = qobject_cast<GamePage *>(page)) {
        newGamePage->onPageShownByStack();
    }
}

void MainWindow::prepareStoryForLevel(int levelId, QWidget *returnPage)
{
    m_pendingStoryLevelId = levelId;
    m_storyReturnPage = returnPage;
    m_storyPage->setLevel(levelId);
    switchTo(m_storyPage);
}

void MainWindow::showStartPage()
{
    switchTo(m_startPage);
}

void MainWindow::showLevelSelectPage()
{
    switchTo(m_levelSelectPage);
}

void MainWindow::showSettingsPage()
{
    m_settingsPage->setMusicEnabled(SoundManager::instance().isMusicEnabled());
    switchTo(m_settingsPage);
}

void MainWindow::startLevel(int levelId)
{
    qDebug() << "MainWindow start level:" << levelId;

    m_lastLevelId = levelId;

    switchTo(m_gamePage);
    m_gamePage->startLevel(levelId);
}

void MainWindow::queueStartLevel(int levelId)
{
    if (m_levelTransitionPending) {
        return;
    }

    m_levelTransitionPending = true;
    QTimer::singleShot(0, this, [this, levelId]() {
        startLevel(levelId);
        m_levelTransitionPending = false;
    });
}

void MainWindow::showResultPage(bool win, int score)
{
    if (m_levelTransitionPending) {
        return;
    }

    m_levelTransitionPending = true;
    const int finishedLevelId = m_lastLevelId;
    QTimer::singleShot(0, this, [this, win, score, finishedLevelId]() {
        if (m_gamePage) {
            m_gamePage->stopGame();
        }

        const LevelData levelData = LevelManager::createLevel(finishedLevelId);
        m_resultPage->setResult(win, score, finishedLevelId, levelData.backgroundPath);
        switchTo(m_resultPage);
        m_levelTransitionPending = false;
    });
}

void MainWindow::backToMenu()
{
    if (m_gamePage) {
        m_gamePage->stopGame();
    }

    switchTo(m_startPage);
}
