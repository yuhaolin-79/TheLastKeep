/* 功能 结算页面
 * 显示
 * 胜利、失败 最终分数
 * 返回主菜单 重新开始 下一关
 */

#include "ui/pages/ResultPage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>
#include <Qt>

ResultPage::ResultPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void ResultPage::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    m_titleLabel = new QLabel("游戏结束", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setFont(QFont("Microsoft YaHei", 30, QFont::Bold));

    m_scoreLabel = new QLabel("分数：0", this);
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    m_scoreLabel->setFont(QFont("Microsoft YaHei", 18));

    m_restartButton = new QPushButton("重新开始", this);
    m_nextLevelButton = new QPushButton("下一关", this);
    m_backButton = new QPushButton("返回主菜单", this);

    m_restartButton->setFixedSize(220, 54);
    m_nextLevelButton->setFixedSize(220, 54);
    m_backButton->setFixedSize(220, 54);

    layout->addWidget(m_titleLabel);
    layout->addWidget(m_scoreLabel);
    layout->addWidget(m_restartButton, 0, Qt::AlignCenter);
    layout->addWidget(m_nextLevelButton, 0, Qt::AlignCenter);
    layout->addWidget(m_backButton, 0, Qt::AlignCenter);

    connect(m_restartButton, &QPushButton::clicked,
            this, &ResultPage::restartClicked);

    connect(m_nextLevelButton, &QPushButton::clicked,
            this, [this]() {
                emit nextLevelClicked(m_levelId + 1);
            });

    connect(m_backButton, &QPushButton::clicked,
            this, &ResultPage::backToMenuClicked);
}

void ResultPage::setResult(bool win, int score, int levelId)
{
    m_levelId = levelId;

    m_titleLabel->setText(win ? "胜利！" : "失败！");
    m_scoreLabel->setText(QString("分数：%1").arg(score));

    // 第三关之后暂时不显示下一关
    m_nextLevelButton->setVisible(win && levelId < 3);
}