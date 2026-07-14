#include "ui/pages/StartPage.h"
#include "common/GameConstants.h"

#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QPushButton>

StartPage::StartPage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(GameConstants::WindowWidth, GameConstants::WindowHeight);

    m_background.load(":/images/background.png");
    if (m_background.isNull()) {
        qWarning() << "StartPage background load failed: :/images/background.png";
    }

    setupUi();
}

void StartPage::setupUi()
{
    m_startButton = new QPushButton("", this);
    m_levelSelectButton = new QPushButton("", this);
    m_settingsButton = new QPushButton("", this);
    m_quitButton = new QPushButton("", this);

    const int buttonW = 340;
    const int buttonH = 85;
    const int buttonX = 880;
    const int startY = 210;
    const int gap = 95;

    m_startButton->setFixedSize(buttonW, buttonH);
    m_levelSelectButton->setFixedSize(buttonW, buttonH);
    m_settingsButton->setFixedSize(buttonW, buttonH);
    m_quitButton->setFixedSize(buttonW, buttonH);

    m_startButton->move(buttonX, startY);
    m_levelSelectButton->move(buttonX, startY + gap);
    m_settingsButton->move(buttonX, startY + gap * 2);
    m_quitButton->move(buttonX, startY + gap * 3);

    setupImageButton(m_startButton, ":/images/ui/start_button.png");
    setupImageButton(m_levelSelectButton, ":/images/ui/level_select_button.png");
    setupImageButton(m_settingsButton, ":/images/ui/setting_button.png");
    setupImageButton(m_quitButton, ":/images/ui/quit_button.png");

    connect(m_startButton, &QPushButton::clicked,
            this, &StartPage::startClicked);

    connect(m_levelSelectButton, &QPushButton::clicked,
            this, &StartPage::levelSelectClicked);

    connect(m_settingsButton, &QPushButton::clicked,
            this, &StartPage::settingsClicked);

    connect(m_quitButton, &QPushButton::clicked,
            this, &StartPage::quitClicked);
}

void StartPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (!m_background.isNull()) {
        painter.drawPixmap(rect(), m_background);
    } else {
        painter.fillRect(rect(), QColor("#1E1E1E"));
    }
}

void StartPage::setupImageButton(QPushButton *button, const QString &imagePath)
{
    if (!button) {
        return;
    }

    QPixmap testPixmap(imagePath);
    if (testPixmap.isNull()) {
        qWarning() << "Button image load failed:" << imagePath;
    }

    button->setCursor(Qt::PointingHandCursor);
    button->setFocusPolicy(Qt::NoFocus);

    const QString style = QString(R"(
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
