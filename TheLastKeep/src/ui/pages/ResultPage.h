/* 功能 结算页面
 * 显示
 * 胜利、失败 最终分数
 * 返回主菜单 重新开始 下一关
 */

#pragma once

#include <QWidget>

class QLabel;
class QPushButton;

// ResultPage：结算页面
// 职责：只显示结果，不判断胜负。
class ResultPage : public QWidget {
    Q_OBJECT

public:
    explicit ResultPage(QWidget *parent = nullptr);

    void setResult(bool win, int score, int levelId);

signals:
    void backToMenuClicked();
    void restartClicked();
    void nextLevelClicked(int nextLevelId);

private:
    void setupUi();

private:
    QLabel *m_titleLabel = nullptr;
    QLabel *m_scoreLabel = nullptr;

    QPushButton *m_restartButton = nullptr;
    QPushButton *m_nextLevelButton = nullptr;
    QPushButton *m_backButton = nullptr;

    int m_levelId = 0;
};