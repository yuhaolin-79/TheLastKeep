/* 目录 src/ui/pages
 * 功能 开始界面
 * 负责显示 游戏主背景 开始游戏按钮和退出游戏按钮
 */

#pragma once

#include <QWidget>
#include <QPixmap>

class QPushButton;

// StartPage：开始页面
// 职责：只显示按钮，只发信号。
// 不负责真正开始游戏。
class StartPage : public QWidget {
    Q_OBJECT

public:
    explicit StartPage(QWidget *parent = nullptr);

signals:
    void startClicked();
    void levelSelectClicked();
    void quitClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUi();
    void setupImageButton(QPushButton *button, const QString &imagePath);

private:
    QPushButton *m_startButton = nullptr;
    QPushButton *m_levelSelectButton = nullptr;
    QPushButton *m_quitButton = nullptr;

    //主页面背景图
    QPixmap m_background;
};