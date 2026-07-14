/* 功能 每波结束后的卡牌选择界面
 * 负责显示三张卡
 * 玩家点一张后发信号
 * 不负责修改 buff，该部分由 CardManager 实现
 */

#include "ui/widgets/CardSelectWidget.h"

#include <QEnterEvent>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

class CardChoiceButton final : public QPushButton
{
public:
    explicit CardChoiceButton(QWidget *parent = nullptr)
        : QPushButton(parent)
    {
        setFixedSize(238, 332);
        setCursor(Qt::PointingHandCursor);
        setFocusPolicy(Qt::StrongFocus);
        setFlat(true);
    }

    void setCard(const CardInfo &card)
    {
        m_card = card;
        m_background.load(card.cardBackgroundPath);
        setAccessibleName(card.titleText);
        setAccessibleDescription(card.descText);
        update();
    }

    CardType cardType() const
    {
        return m_card.type;
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        const QRectF cardRect = rect().adjusted(3, 3, -3, -3);
        QPainterPath clipPath;
        clipPath.addRoundedRect(cardRect, 8, 8);
        painter.setClipPath(clipPath);

        if (!m_background.isNull()) {
            painter.drawPixmap(cardRect.toRect(), m_background);
        } else {
            painter.fillRect(cardRect, QColor("#33271F"));
        }

        painter.fillRect(cardRect, QColor(0, 0, 0, 38));
        painter.fillRect(QRectF(17, 18, width() - 34, 60), QColor(14, 10, 8, 198));
        painter.fillRect(QRectF(17, height() - 120, width() - 34, 102), QColor(14, 10, 8, 214));

        painter.setPen(QColor("#FFE3A3"));
        painter.setFont(QFont(font().family(), 20, QFont::Bold));
        painter.drawText(QRect(24, 24, width() - 48, 48),
                         Qt::AlignCenter | Qt::TextWordWrap,
                         m_card.titleText);

        painter.setPen(QColor("#FFF4D6"));
        painter.setFont(QFont(font().family(), 15, QFont::DemiBold));
        painter.drawText(QRect(29, height() - 108, width() - 58, 78),
                         Qt::AlignCenter | Qt::TextWordWrap,
                         m_card.descText);

        painter.setClipping(false);
        QColor borderColor("#B99555");
        int borderWidth = 2;
        if (underMouse() || hasFocus()) {
            borderColor = QColor("#FFE29A");
            borderWidth = 4;
        }
        if (isDown()) {
            borderColor = QColor("#D49436");
        }

        painter.setPen(QPen(borderColor, borderWidth));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(cardRect, 8, 8);

        if (!isEnabled()) {
            painter.setBrush(QColor(0, 0, 0, 112));
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(cardRect, 8, 8);
        }
    }

    void enterEvent(QEnterEvent *event) override
    {
        QPushButton::enterEvent(event);
        update();
    }

    void leaveEvent(QEvent *event) override
    {
        QPushButton::leaveEvent(event);
        update();
    }

private:
    CardInfo m_card{};
    QPixmap m_background;
};

CardSelectWidget::CardSelectWidget(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_StyledBackground, false);

    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(72, 54, 72, 48);
    rootLayout->setSpacing(8);

    auto *titleLabel = new QLabel(QStringLiteral("选择战利品"), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(R"(
        QLabel {
            color: #FFE3A3;
            font-size: 32px;
            font-weight: 800;
            background: transparent;
        }
    )");

    auto *subtitleLabel = new QLabel(
        QStringLiteral("选择一项缴获，强化本局防线"), this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet(R"(
        QLabel {
            color: #FFF4D6;
            font-size: 17px;
            font-weight: 600;
            background: transparent;
        }
    )");

    auto *cardLayout = new QHBoxLayout;
    cardLayout->setContentsMargins(0, 16, 0, 0);
    cardLayout->setSpacing(28);
    cardLayout->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < 3; ++i) {
        auto *button = new CardChoiceButton(this);
        m_cardButtons.append(button);
        cardLayout->addWidget(button);

        connect(button, &QPushButton::clicked, this, [this, button]() {
            if (m_selectionLocked) {
                return;
            }

            m_selectionLocked = true;
            for (CardChoiceButton *cardButton : m_cardButtons) {
                cardButton->setEnabled(false);
            }
            emit cardSelected(button->cardType());
        });
    }

    rootLayout->addWidget(titleLabel);
    rootLayout->addWidget(subtitleLabel);
    rootLayout->addLayout(cardLayout, 1);
}

void CardSelectWidget::setCards(const QVector<CardInfo> &cards)
{
    m_selectionLocked = false;

    for (int i = 0; i < m_cardButtons.size(); ++i) {
        CardChoiceButton *button = m_cardButtons[i];
        const bool hasCard = i < cards.size();
        button->setVisible(hasCard);
        button->setEnabled(hasCard);
        if (hasCard) {
            button->setCard(cards[i]);
        }
    }

    if (!cards.isEmpty()) {
        m_cardButtons.first()->setFocus(Qt::OtherFocusReason);
    }
}

void CardSelectWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor(5, 7, 8, 188));
}