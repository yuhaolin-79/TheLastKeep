#include "ui/pages/SettingsPage.h"
#include "common/GameConstants.h"

#include <QCheckBox>
#include <QColor>
#include <QDebug>
#include <QFont>
#include <QFrame>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(GameConstants::WindowWidth, GameConstants::WindowHeight);

    m_background.load(":/images/background.png");
    if (m_background.isNull()) {
        qWarning() << "SettingsPage background load failed: :/images/background.png";
    }

    setupUi();
}

void SettingsPage::setMusicEnabled(bool enabled)
{
    if (m_musicCheckBox) {
        m_musicCheckBox->setChecked(enabled);
    }
}

void SettingsPage::setupUi()
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setAlignment(Qt::AlignCenter);

    auto *panel = new QFrame(this);
    panel->setFixedSize(420, 280);
    panel->setStyleSheet(R"(
        QFrame {
            background-color: rgba(18, 16, 14, 190);
            border: 2px solid rgba(234, 188, 104, 210);
            border-radius: 8px;
        }
    )");

    auto *panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(36, 30, 36, 30);
    panelLayout->setSpacing(24);
    panelLayout->setAlignment(Qt::AlignCenter);

    auto *titleLabel = new QLabel(QStringLiteral("设置"), panel);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Microsoft YaHei", 28, QFont::Bold));
    titleLabel->setStyleSheet("QLabel { color: #F3D28A; background: transparent; border: none; }");

    m_musicCheckBox = new QCheckBox(QStringLiteral("播放背景音乐"), panel);
    m_musicCheckBox->setCursor(Qt::PointingHandCursor);
    m_musicCheckBox->setMinimumHeight(54);
    m_musicCheckBox->setFont(QFont("Microsoft YaHei", 18, QFont::Bold));
    m_musicCheckBox->setStyleSheet(R"(
        QCheckBox {
            color: #FFF3D4;
            spacing: 14px;
            background: transparent;
            border: none;
        }
        QCheckBox::indicator {
            width: 32px;
            height: 32px;
        }
        QCheckBox::indicator:unchecked {
            border: 2px solid #EABC68;
            background-color: rgba(0, 0, 0, 90);
        }
        QCheckBox::indicator:checked {
            border: 2px solid #FFE19A;
            background-color: #D59A3A;
        }
    )");

    m_backButton = new QPushButton(QStringLiteral("返回"), panel);
    m_backButton->setFixedSize(220, 58);
    m_backButton->setCursor(Qt::PointingHandCursor);
    m_backButton->setFocusPolicy(Qt::NoFocus);
    m_backButton->setFont(QFont("Microsoft YaHei", 18, QFont::Bold));
    m_backButton->setStyleSheet(R"(
        QPushButton {
            color: #2B1A0D;
            background-color: #EABC68;
            border: 2px solid #FFE19A;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #F3D28A;
        }
        QPushButton:pressed {
            padding-top: 3px;
            padding-left: 3px;
            background-color: #C7822A;
        }
    )");

    panelLayout->addWidget(titleLabel);
    panelLayout->addWidget(m_musicCheckBox, 0, Qt::AlignCenter);
    panelLayout->addWidget(m_backButton, 0, Qt::AlignCenter);
    rootLayout->addWidget(panel, 0, Qt::AlignCenter);

    connect(m_musicCheckBox, &QCheckBox::toggled,
            this, &SettingsPage::musicEnabledChanged);

    connect(m_backButton, &QPushButton::clicked,
            this, &SettingsPage::backClicked);
}

void SettingsPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    if (!m_background.isNull()) {
        painter.drawPixmap(rect(), m_background);
    } else {
        painter.fillRect(rect(), QColor("#1E1E1E"));
    }
}
