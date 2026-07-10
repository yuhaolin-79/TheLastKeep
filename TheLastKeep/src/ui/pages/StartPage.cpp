/* 目录 src/ui/pages
 * 功能 开始界面
 * 负责显示 游戏主背景、开始游戏按钮、选择关卡按钮、退出游戏按钮
 */

#include "ui/pages/StartPage.h"
#include "common/GameConstants.h"

#include <QPushButton>
#include <QPainter>
#include <QDebug>
#include <QFont>

StartPage::StartPage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(GameConstants::WindowWidth, GameConstants::WindowHeight);

    // 加载主菜单背景图
    m_background.load(":/images/background.png");

    if (m_background.isNull()) {
        qWarning() << "StartPage background load failed: :/images/background.png";
    }

    setupUi();
}

void StartPage::setupUi()
{
    // 创建按钮,使用了贴图，传空串
    m_startButton = new QPushButton("", this);
    m_levelSelectButton = new QPushButton("", this);
    m_quitButton = new QPushButton("", this);

    // 统一按钮大小
    const int buttonW = 340;
    const int buttonH = 85;

    m_startButton->setFixedSize(buttonW, buttonH);
    m_levelSelectButton->setFixedSize(buttonW, buttonH);
    m_quitButton->setFixedSize(buttonW, buttonH);

    // 按钮位置：先按旧版 GameScene 里的位置放右侧
    // 后面可以根据背景图微调 x 和 y
    const int buttonX = 880;
    const int startY = 250;
    const int gap = 100;

    m_startButton->move(buttonX, startY);
    m_levelSelectButton->move(buttonX, startY + gap);
    m_quitButton->move(buttonX, startY + gap * 2);

    // 给按钮设置贴图
    setupImageButton(m_startButton, ":/images/ui/start_button.png");
    setupImageButton(m_levelSelectButton, ":/images/ui/level_select_button.png");
    setupImageButton(m_quitButton, ":/images/ui/quit_button.png");

    // 按钮点击后，只发信号
    // StartPage 不负责切页面，也不负责加载游戏
    connect(m_startButton, &QPushButton::clicked,
            this, &StartPage::startClicked);

    connect(m_levelSelectButton, &QPushButton::clicked,
            this, &StartPage::levelSelectClicked);

    connect(m_quitButton, &QPushButton::clicked,
            this, &StartPage::quitClicked);
}

void StartPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (!m_background.isNull()) {
        painter.drawPixmap(
            rect(),
            m_background
            );
    } else {
        // 背景图加载失败时的兜底背景
        painter.fillRect(rect(), QColor("#1E1E1E"));
    }
}


void StartPage::setupImageButton(QPushButton *button, const QString &imagePath)
{
    if (!button) {
        return;
    }

    // 检查图片是否能加载，方便排查问题
    QPixmap testPixmap(imagePath);
    if (testPixmap.isNull()) {
        qWarning() << "Button image load failed:" << imagePath;
    }

    button->setCursor(Qt::PointingHandCursor);
    button->setFocusPolicy(Qt::NoFocus);

    // border-image 会把图片完整拉伸到按钮大小
    // 因为你的按钮是横向长条，拉伸影响不大
    QString style = QString(R"(
        QPushButton {
            border: none;
            background: transparent;
            border-image: url(%1) 0 0 0 0 stretch stretch;
        }

        QPushButton:hover {
            border: none;
            background: transparent;
            border-image: url(%1) 0 0 0 0 stretch stretch;
        }

        QPushButton:pressed {
            border: none;
            background: transparent;
            border-image: url(%1) 0 0 0 0 stretch stretch;
            padding-top: 3px;
            padding-left: 3px;
        }
    )").arg(imagePath);

    button->setStyleSheet(style);
}