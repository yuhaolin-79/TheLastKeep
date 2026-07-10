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

#pragma once

#include <QWidget>

class QGraphicsView;
class QPushButton;

class GameScene;
class GameController;

// GamePage：游戏页面容器
// 职责：
// 1. 创建 QGraphicsView
// 2. 创建 GameScene
// 3. 创建 GameController
// 4. 页面隐藏时暂停游戏
// 5. 页面显示时恢复游戏
//
// 注意：
// GamePage 不直接写敌人移动、塔攻击等逻辑。
class GamePage : public QWidget {
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);
    ~GamePage();

    void startLevel(int levelId);
    void restartLevel();
    void stopGame();

    void onPageHiddenByStack();
    void onPageShownByStack();

signals:
    void requestBackToMenu();
    void gameFinished(bool win, int score);

protected:
    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void handlePauseOrResumeClicked();
    void handleBackClicked();
    void handleGameFinished(bool win, int score);

private:
    void setupUi();
    void setupConnections();

    void pauseBecauseHidden();
    void resumeBecauseShown();

private:
    QGraphicsView *m_view = nullptr;
    GameScene *m_scene = nullptr;
    GameController *m_controller = nullptr;

    QPushButton *m_pauseButton = nullptr;
    QPushButton *m_backButton = nullptr;

    int m_currentLevelId = -1;

    bool m_started = false;

    // 如果页面隐藏前游戏正在运行，则切回来时自动恢复
    bool m_shouldResumeWhenShown = false;
};