#include "ui/pages/StoryPage.h"

#include "common/GameConstants.h"

#include <QColor>
#include <QDebug>
#include <QFile>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QScrollBar>
#include <QTextBrowser>
#include <QVBoxLayout>

StoryPage::StoryPage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(GameConstants::WindowWidth, GameConstants::WindowHeight);

    m_background.load(":/images/background.png");
    if (m_background.isNull()) {
        qWarning() << "StoryPage background load failed: :/images/background.png";
    }

    setupUi();
}

void StoryPage::setupUi()
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setAlignment(Qt::AlignCenter);

    auto *panel = new QFrame(this);
    panel->setFixedSize(820, 620);
    panel->setStyleSheet(R"(
        QFrame {
            background-color: rgba(18, 14, 12, 214);
            border: 2px solid rgba(234, 188, 104, 210);
            border-radius: 8px;
        }
    )");

    auto *panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(48, 34, 48, 34);
    panelLayout->setSpacing(20);

    m_titleLabel = new QLabel(storyTitle(m_levelId), panel);
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

    m_storyText = new QTextBrowser(panel);
    m_storyText->setText(loadStoryText(m_levelId));
    m_storyText->setReadOnly(true);
    m_storyText->setOpenExternalLinks(false);
    m_storyText->setFrameShape(QFrame::NoFrame);
    m_storyText->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_storyText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_storyText->setStyleSheet(R"(
        QTextBrowser {
            color: #FFF3D4;
            background: rgba(0, 0, 0, 72);
            border: 1px solid rgba(234, 188, 104, 95);
            border-radius: 6px;
            padding: 18px 22px;
            font-size: 20px;
            line-height: 150%;
        }
        QScrollBar:vertical {
            width: 12px;
            background: rgba(0, 0, 0, 90);
            margin: 4px 0 4px 0;
        }
        QScrollBar::handle:vertical {
            background: rgba(234, 188, 104, 190);
            border-radius: 5px;
            min-height: 44px;
        }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0;
        }
    )");

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(24);
    buttonLayout->setAlignment(Qt::AlignCenter);

    m_backButton = new QPushButton(QStringLiteral("返回"), panel);
    m_continueButton = new QPushButton(QStringLiteral("开始守城"), panel);
    setupCommandButton(m_backButton, false);
    setupCommandButton(m_continueButton, true);

    buttonLayout->addWidget(m_backButton);
    buttonLayout->addWidget(m_continueButton);

    panelLayout->addWidget(m_titleLabel);
    panelLayout->addWidget(m_storyText, 1);
    panelLayout->addLayout(buttonLayout);
    rootLayout->addWidget(panel, 0, Qt::AlignCenter);

    connect(m_backButton, &QPushButton::clicked,
            this, &StoryPage::backClicked);

    connect(m_continueButton, &QPushButton::clicked,
            this, &StoryPage::continueClicked);
}

void StoryPage::setLevel(int levelId)
{
    if (levelId < 0 || levelId > 3) {
        levelId = 0;
    }

    m_levelId = levelId;

    if (m_titleLabel) {
        m_titleLabel->setText(storyTitle(m_levelId));
    }

    if (m_storyText) {
        m_storyText->setText(loadStoryText(m_levelId));
        m_storyText->verticalScrollBar()->setValue(0);
    }
}

QString StoryPage::storyTitle(int levelId) const
{
    switch (levelId) {
    case 1:
        return QStringLiteral("第一章：黑雾下的古道");
    case 2:
        return QStringLiteral("第二章：双路夹击");
    case 3:
        return QStringLiteral("终章：黎明决战");
    case 0:
    default:
        return QStringLiteral("序章：最后的王堡");
    }
}

QString StoryPage::storyResourcePath(int levelId) const
{
    switch (levelId) {
    case 1:
        return QStringLiteral(":/story_level01.txt");
    case 2:
        return QStringLiteral(":/story_level02.txt");
    case 3:
        return QStringLiteral(":/story_level03.txt");
    case 0:
    default:
        return QStringLiteral(":/story.txt");
    }
}

QString StoryPage::loadStoryText(int levelId) const
{
    const QString resourcePath = storyResourcePath(levelId);
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "StoryPage story load failed:" << resourcePath;
        return QStringLiteral("王堡的火光仍在等待你的命令。");
    }

    return QString::fromUtf8(file.readAll()).trimmed();
}

void StoryPage::setupCommandButton(QPushButton *button, bool primary) const
{
    if (!button) {
        return;
    }

    button->setFixedSize(220, 58);
    button->setCursor(Qt::PointingHandCursor);
    button->setFocusPolicy(Qt::NoFocus);

    if (primary) {
        button->setStyleSheet(R"(
            QPushButton {
                color: #2B1A0D;
                background-color: #EABC68;
                border: 2px solid #FFE19A;
                border-radius: 8px;
                font-size: 20px;
                font-weight: 800;
            }
            QPushButton:hover { background-color: #F3D28A; }
            QPushButton:pressed {
                padding-top: 3px;
                padding-left: 3px;
                background-color: #C7822A;
            }
        )");
        return;
    }

    button->setStyleSheet(R"(
        QPushButton {
            color: #FFF3D4;
            background-color: rgba(42, 27, 18, 160);
            border: 2px solid rgba(234, 188, 104, 170);
            border-radius: 8px;
            font-size: 20px;
            font-weight: 800;
        }
        QPushButton:hover { background-color: rgba(77, 48, 31, 210); }
        QPushButton:pressed {
            padding-top: 3px;
            padding-left: 3px;
            background-color: rgba(30, 18, 14, 230);
        }
    )");
}

void StoryPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    if (!m_background.isNull()) {
        painter.drawPixmap(rect(), m_background);
        painter.fillRect(rect(), QColor(0, 0, 0, 72));
    } else {
        painter.fillRect(rect(), QColor("#1E1E1E"));
    }
}