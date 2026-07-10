/* 目录 src/ui/pages
 * 功能 关卡选择页面
 * 显示 引导关卡 第一关 第二关 ... 返回按钮
 */

#include "ui/pages/LevelSelectPage.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <Qt>

LevelSelectPage::LevelSelectPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void LevelSelectPage::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(18);

    auto *title = new QLabel("选择关卡", this);
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Microsoft YaHei", 28, QFont::Bold));

    m_tutorialButton = new QPushButton("引导关卡", this);
    m_level1Button = new QPushButton("第一关", this);
    m_level2Button = new QPushButton("第二关", this);
    m_level3Button = new QPushButton("第三关", this);
    m_backButton = new QPushButton("返回", this);

    QList<QPushButton *> buttons = {
        m_tutorialButton,
        m_level1Button,
        m_level2Button,
        m_level3Button,
        m_backButton
    };

    for (QPushButton *button : buttons) {
        button->setFixedSize(220, 54);
        layout->addWidget(button, 0, Qt::AlignCenter);
    }

    layout->insertWidget(0, title);

    connect(m_tutorialButton, &QPushButton::clicked,
            this, [this]() { emit levelSelected(0); });

    connect(m_level1Button, &QPushButton::clicked,
            this, [this]() { emit levelSelected(1); });

    connect(m_level2Button, &QPushButton::clicked,
            this, [this]() { emit levelSelected(2); });

    connect(m_level3Button, &QPushButton::clicked,
            this, [this]() { emit levelSelected(3); });

    connect(m_backButton, &QPushButton::clicked,
            this, &LevelSelectPage::backClicked);
}