/* 功能 游戏顶部信息栏
 * 负责显示
 * 金币 生命值 当前波次 总波次 分数 暂停按钮 返回按钮
 * 防御塔选择
 */

#pragma once

#include <QWidget>
#include "entity/Tower.h"

class QLabel;
class QPushButton;

class HUDWidget : public QWidget {
    Q_OBJECT

public:
    explicit HUDWidget(QWidget* parent = nullptr);

    void setStats(int gold, int hp, int maxHp, int wave, int totalWave);
    void setPaused(bool paused);
    void setSelectedTower(TowerType type);

signals:
    void pauseClicked();
    void backClicked();
    void towerSelected(TowerType type);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPushButton* createTowerButton(TowerType type, const QString& iconPath, const QString& text);
    void refreshTowerButtonStyles();
    QString towerButtonStyle(bool selected) const;

private:
    QLabel* m_goldLabel = nullptr;
    QLabel* m_hpLabel = nullptr;
    QLabel* m_waveLabel = nullptr;
    QLabel* m_towerHintLabel = nullptr;
    QPushButton* m_pauseButton = nullptr;
    QPushButton* m_backButton = nullptr;
    QVector<QPushButton*> m_towerButtons;
    QVector<TowerType> m_towerButtonTypes;
    TowerType m_selectedTower = TowerType::ArrowTower;
};