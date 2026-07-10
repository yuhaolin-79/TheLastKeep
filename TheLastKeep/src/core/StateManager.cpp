/* 统一管理游戏状态
 * @fish 2026/07/10 VERSION 1.0
 */

#include "core/StateManager.h"

StateManager::StateManager()
    : m_status(GameStatus::Menu){

}

GameStatus StateManager::status() const {
    return m_status;
}

void StateManager::setStatus(GameStatus status){
    m_status = status;
}

bool StateManager::isRunning() const {
    return m_status == GameStatus::Running;
}

bool StateManager::isPaused() const {
    return m_status == GameStatus::Paused;
}

bool StateManager::canUpdateGame() const {
    return m_status == GameStatus::Running;
}

void StateManager::reset() {
    m_status = GameStatus::Menu;
}