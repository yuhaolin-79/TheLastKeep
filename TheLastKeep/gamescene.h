#pragma once

#include <QGraphicsScene>
// 按钮控件前置声明
class QPushButton;
class QGraphicsProxyWidget;

class GameScene : public QGraphicsScene{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);

private:
    void buildDemoMap();
    // 复用创建按钮的工具函数
    QGraphicsProxyWidget* createBtn(const QString& text, int x, int y, int w, int h);

    // 按钮点击信号，给主窗口接收
signals:
    void sigStartGame();
    void sigQuitGame();
};