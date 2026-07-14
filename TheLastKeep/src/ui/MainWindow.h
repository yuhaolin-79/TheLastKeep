#pragma once

#include <QMainWindow>

class QEvent;
class QStackedWidget;
class QWidget;

class GamePage;
class LevelSelectPage;
class ResultPage;
class SettingsPage;
class StartPage;
class StoryPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void showStartPage();
    void showLevelSelectPage();
    void showSettingsPage();
    void startLevel(int levelId);
    void showResultPage(bool win, int score);
    void backToMenu();

private:
    void setupUi();
    void setupConnections();
    void switchTo(QWidget *page);
    void prepareStoryForLevel(int levelId, QWidget *returnPage);
    void queueStartLevel(int levelId);

private:
    QStackedWidget *m_stack = nullptr;

    StartPage *m_startPage = nullptr;
    LevelSelectPage *m_levelSelectPage = nullptr;
    SettingsPage *m_settingsPage = nullptr;
    StoryPage *m_storyPage = nullptr;
    GamePage *m_gamePage = nullptr;
    ResultPage *m_resultPage = nullptr;

    QWidget *m_currentPage = nullptr;
    QWidget *m_storyReturnPage = nullptr;

    int m_lastLevelId = 0;
    int m_pendingStoryLevelId = 0;
    bool m_levelTransitionPending = false;
};
