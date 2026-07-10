/* 目录 src/ui/pages
 * 功能 关卡选择页面
 * 显示 引导关卡 第一关 第二关 ... 返回按钮
 */

#pragma once

#include <QWidget>

class QPushButton;

// LevelSelectPage：关卡选择页面
// 职责：只负责显示关卡按钮，并发出 levelSelected(levelId) 信号。
class LevelSelectPage : public QWidget {
    Q_OBJECT

public:
    explicit LevelSelectPage(QWidget *parent = nullptr);

signals:
    void levelSelected(int levelId);
    void backClicked();

private:
    void setupUi();

private:
    QPushButton *m_tutorialButton = nullptr;
    QPushButton *m_level1Button = nullptr;
    QPushButton *m_level2Button = nullptr;
    QPushButton *m_level3Button = nullptr;
    QPushButton *m_backButton = nullptr;
};