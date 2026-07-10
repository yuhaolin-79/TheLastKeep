/* 统一管理游戏状态
 */

#pragma once

#include "common/GameTypes.h"

class StateManager {
public:
    StateManager();

    GameStatus status() const;
    void setStatus(GameStatus status);

    bool isRunning() const;
    bool isPaused() const;

    // 当前状态是否允许游戏主循环更新
    bool canUpdateGame() const;

    void reset();

private:
    GameStatus m_status = GameStatus::Menu;
};
