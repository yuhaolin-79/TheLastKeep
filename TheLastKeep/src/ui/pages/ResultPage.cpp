#include "ui/pages/ResultPage.h"

#include "common/GameConstants.h"

#include <QColor>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

ResultPage::ResultPage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(GameConstants::WindowWidth, GameConstants::WindowHeight);
    setBackgroundPath(":/images/background.png");
    setupUi();

    m_autoReturnTimer = new QTimer(this);
    m_autoReturnTimer->setSingleShot(true);
    m_autoReturnTimer->setInterval(10000);
    connect(m_autoReturnTimer, &QTimer::timeout,
            this, &ResultPage::autoReturnToMainRequested);
}

void ResultPage::setupUi()
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setAlignment(Qt::AlignCenter);

    m_panel = new QFrame(this);
    m_panel->setFixedSize(620, 360);
    m_panel->setStyleSheet(R"(
        QFrame {
            background-color: rgba(18, 14, 12, 210);
            border: 2px solid rgba(234, 188, 104, 210);
            border-radius: 8px;
        }
    )");

    auto *layout = new QVBoxLayout(m_panel);
    layout->setContentsMargins(44, 34, 44, 34);
    layout->setSpacing(18);
    layout->setAlignment(Qt::AlignCenter);

    m_titleLabel = new QLabel(QStringLiteral("战斗结束"), m_panel);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(R"(
        QLabel {
            color: #F3D28A;
            font-size: 34px;
            font-weight: 800;
            background: transparent;
            border: none;
        }
    )");

    m_messageLabel = new QLabel(m_panel);
    m_messageLabel->setAlignment(Qt::AlignCenter);
    m_messageLabel->setWordWrap(true);
    m_messageLabel->setStyleSheet(R"(
        QLabel {
            color: #FFF3D4;
            font-size: 18px;
            line-height: 145%;
            background: transparent;
            border: none;
        }
    )");

    m_scoreLabel = new QLabel(m_panel);
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    m_scoreLabel->setStyleSheet(R"(
        QLabel {
            color: #EABC68;
            font-size: 20px;
            font-weight: 700;
            background: transparent;
            border: none;
        }
    )");

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(14);
    buttonLayout->setAlignment(Qt::AlignCenter);

    m_backButton = new QPushButton(QStringLiteral("返回关卡"), m_panel);
    m_restartButton = new QPushButton(QStringLiteral("重新挑战"), m_panel);
    m_nextLevelButton = new QPushButton(QStringLiteral("下一关"), m_panel);

    setupButton(m_backButton, false);
    setupButton(m_restartButton, false);
    setupButton(m_nextLevelButton, true);

    buttonLayout->addWidget(m_backButton);
    buttonLayout->addWidget(m_restartButton);
    buttonLayout->addWidget(m_nextLevelButton);

    layout->addWidget(m_titleLabel);
    layout->addWidget(m_messageLabel);
    layout->addWidget(m_scoreLabel);
    layout->addSpacing(8);
    layout->addLayout(buttonLayout);
    rootLayout->addWidget(m_panel, 0, Qt::AlignCenter);

    connect(m_restartButton, &QPushButton::clicked,
            this, &ResultPage::restartClicked);

    connect(m_nextLevelButton, &QPushButton::clicked,
            this, [this]() {
                emit nextLevelClicked(m_levelId + 1);
            });

    connect(m_backButton, &QPushButton::clicked,
            this, &ResultPage::backToMenuClicked);
}

void ResultPage::setupButton(QPushButton *button, bool primary) const
{
    if (!button) {
        return;
    }

    button->setFixedSize(150, 54);
    button->setCursor(Qt::PointingHandCursor);
    button->setFocusPolicy(Qt::NoFocus);

    if (primary) {
        button->setStyleSheet(R"(
            QPushButton {
                color: #2B1A0D;
                background-color: #EABC68;
                border: 2px solid #FFE19A;
                border-radius: 8px;
                font-size: 18px;
                font-weight: 800;
            }
            QPushButton:hover { background-color: #F3D28A; }
            QPushButton:pressed { padding-top: 3px; padding-left: 3px; background-color: #C7822A; }
        )");
        return;
    }

    button->setStyleSheet(R"(
        QPushButton {
            color: #FFF3D4;
            background-color: rgba(42, 27, 18, 160);
            border: 2px solid rgba(234, 188, 104, 170);
            border-radius: 8px;
            font-size: 17px;
            font-weight: 800;
        }
        QPushButton:hover { background-color: rgba(77, 48, 31, 210); }
        QPushButton:pressed { padding-top: 3px; padding-left: 3px; background-color: rgba(30, 18, 14, 230); }
    )");
}

void ResultPage::setResult(bool win, int score, int levelId, const QString &backgroundPath)
{
    stopAutoReturnTimer();
    m_levelId = levelId;
    m_fullscreenResultImage = !win || levelId >= 3;

    if (m_fullscreenResultImage) {
        setBackgroundPath(win ? ":/images/victory.png" : ":/images/defeat.png");
        m_panel->hide();
        m_autoReturnTimer->start();
        update();
        return;
    }

    setBackgroundPath(backgroundPath);
    m_panel->show();

    m_titleLabel->setText(QStringLiteral("守城成功"));
    m_messageLabel->setText(
        QStringLiteral("魔物的攻势被暂时击退。整顿防线，准备迎接下一次进攻。"));
    m_scoreLabel->setText(QStringLiteral("战斗评分：%1").arg(score));

    m_nextLevelButton->setVisible(levelId < 3);
    update();
}

void ResultPage::stopAutoReturnTimer()
{
    if (m_autoReturnTimer) {
        m_autoReturnTimer->stop();
    }
}
void ResultPage::setBackgroundPath(const QString &backgroundPath)
{
    m_background.load(backgroundPath);
    if (m_background.isNull()) {
        m_background.load(":/images/background.png");
    }
}

void ResultPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    if (!m_background.isNull()) {
        painter.drawPixmap(rect(), m_background);
        if (!m_fullscreenResultImage) {
            painter.fillRect(rect(), QColor(0, 0, 0, 92));
        }
    } else {
        painter.fillRect(rect(), QColor("#1E1E1E"));
    }
}