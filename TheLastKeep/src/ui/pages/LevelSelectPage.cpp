/* Directory: src/ui/pages
 * Purpose: level selection page.
 * Shows the guide level, levels 1-3, and a back button.
 */

#include "ui/pages/LevelSelectPage.h"
#include "common/GameConstants.h"

#include <QPushButton>
#include <QPainter>
#include <QDebug>
#include <QColor>
#include <QList>
#include <QString>

LevelSelectPage::LevelSelectPage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(GameConstants::WindowWidth, GameConstants::WindowHeight);

    m_background.load(":/images/Level_select_background.png");
    if (m_background.isNull()) {
        qWarning() << "LevelSelectPage background load failed: :/images/Level_select_background.png";
    }

    setupUi();
}

void LevelSelectPage::setupUi()
{
    m_tutorialButton = new QPushButton("", this);
    m_level1Button = new QPushButton("", this);
    m_level2Button = new QPushButton("", this);
    m_level3Button = new QPushButton("", this);
    m_backButton = new QPushButton("", this);

    const int buttonW = 255;
    const int buttonH = 85;

    QList<QPushButton *> buttons = {
        m_tutorialButton,
        m_level1Button,
        m_level2Button,
        m_level3Button,
        m_backButton
    };

    for (QPushButton *button : buttons) {
        button->setFixedSize(buttonW, buttonH);
    }

    const int buttonStepY = 95;
    const int buttonX = (GameConstants::WindowWidth - buttonW) / 2;
    const int totalButtonsH = buttonH + buttonStepY * (buttons.size() - 1);
    const int startY = (GameConstants::WindowHeight - totalButtonsH) / 2 + 60;

    m_tutorialButton->move(buttonX, startY);
    m_level1Button->move(buttonX, startY + buttonStepY);
    m_level2Button->move(buttonX, startY + buttonStepY * 2);
    m_level3Button->move(buttonX, startY + buttonStepY * 3);
    m_backButton->move(buttonX, startY + buttonStepY * 4);

    setupImageButton(m_tutorialButton, ":/images/ui/tutorial_level_button.png");
    setupImageButton(m_level1Button, ":/images/ui/level01_button.png");
    setupImageButton(m_level2Button, ":/images/ui/level02_button.png");
    setupImageButton(m_level3Button, ":/images/ui/level03_button.png");
    setupImageButton(m_backButton, ":/images/ui/back_button.png");

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

void LevelSelectPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (!m_background.isNull()) {
        painter.drawPixmap(rect(), m_background);
    } else {
        painter.fillRect(rect(), QColor("#1E1E1E"));
    }
}

void LevelSelectPage::setupImageButton(QPushButton *button, const QString &imagePath)
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
