/* 功能 游戏顶部信息栏
 * 负责显示
 * 金币 生命值 当前波次 总波次 分数 暂停按钮 返回按钮
 * 防御塔选择与拖拽建塔入口
 */

#include "ui/widgets/HUDWidget.h"

#include <QApplication>
#include <QDrag>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QVector>

namespace {

QString towerName(TowerType type)
{
    switch(type){
    case TowerType::ArrowTower: return "弩塔";
    case TowerType::MageTower: return "法塔";
    case TowerType::CannonTower: return "炮塔";
    case TowerType::IceTower: return "火塔";
    case TowerType::HolyTower: return "圣塔";
    }
    return "防御塔";
}

QString towerEffect(TowerType type)
{
    switch(type){
    case TowerType::ArrowTower: return "稳定单体输出，攻速较快";
    case TowerType::MageTower: return "高伤害魔法弹，适合打厚血敌人";
    case TowerType::CannonTower: return "重型炮击，单发伤害最高";
    case TowerType::IceTower: return "快速火焰弹，压制低血量敌人";
    case TowerType::HolyTower: return "圣坛能量弹，射程和伤害均衡";
    }
    return "基础攻击";
}

int towerCost(TowerType type)
{
    switch(type){
    case TowerType::ArrowTower: return 50;
    case TowerType::MageTower: return 100;
    case TowerType::CannonTower: return 150;
    case TowerType::IceTower: return 80;
    case TowerType::HolyTower: return 120;
    }
    return 50;
}

int towerDamage(TowerType type)
{
    switch(type){
    case TowerType::ArrowTower: return 28;
    case TowerType::MageTower: return 64;
    case TowerType::CannonTower: return 105;
    case TowerType::IceTower: return 22;
    case TowerType::HolyTower: return 44;
    }
    return 0;
}

QString towerTooltip(TowerType type)
{
    return QString("%1\n伤害：%2\n花费：%3 金币\n效果：%4\n\n点击选择，或拖动到地图可建区域放置")
        .arg(towerName(type))
        .arg(towerDamage(type))
        .arg(towerCost(type))
        .arg(towerEffect(type));
}

class TowerDragButton : public QPushButton {
public:
    TowerDragButton(TowerType type, const QString& iconPath, const QString& text, QWidget* parent = nullptr)
        : QPushButton(text, parent), m_type(type), m_iconPath(iconPath)
    {
        setFixedSize(52, 52);
        setIcon(QIcon(iconPath));
        setIconSize(QSize(34, 34));
        setToolTip(towerTooltip(type));
    }

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if(event->button() == Qt::LeftButton){
            m_pressPos = event->pos();
        }
        QPushButton::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if(!(event->buttons() & Qt::LeftButton)){
            QPushButton::mouseMoveEvent(event);
            return;
        }

        if((event->pos() - m_pressPos).manhattanLength() < QApplication::startDragDistance()){
            QPushButton::mouseMoveEvent(event);
            return;
        }

        auto* mime = new QMimeData;
        mime->setData("application/x-thelastkeep-tower", QByteArray::number(static_cast<int>(m_type)));

        auto* drag = new QDrag(this);
        drag->setMimeData(mime);

        QPixmap pixmap(m_iconPath);
        if(!pixmap.isNull()){
            QPixmap scaled = pixmap.scaled(46, 46, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            drag->setPixmap(scaled);
            drag->setHotSpot(QPoint(scaled.width() / 2, scaled.height() / 2));
        }

        drag->exec(Qt::CopyAction);
    }

private:
    TowerType m_type;
    QString m_iconPath;
    QPoint m_pressPos;
};

}

HUDWidget::HUDWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(92);
    setAutoFillBackground(false);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(34, 12, 34, 12);
    layout->setSpacing(14);

    auto* coinIcon = new QLabel(this);
    QPixmap coin(":/images/coin.png");
    if(!coin.isNull()){
        coinIcon->setPixmap(coin.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    coinIcon->setFixedSize(34, 34);

    m_goldLabel = new QLabel("Gold 260", this);
    m_hpLabel = new QLabel("HP 1000/1000", this);
    m_waveLabel = new QLabel("Wave 1/3", this);
    m_towerHintLabel = new QLabel("Tower", this);

    const QString labelStyle = "QLabel { color: #f8ead2; font-size: 17px; font-weight: 700; background: transparent; }";
    m_goldLabel->setStyleSheet(labelStyle);
    m_hpLabel->setStyleSheet(labelStyle);
    m_waveLabel->setStyleSheet(labelStyle);
    m_towerHintLabel->setStyleSheet("QLabel { color: #f8ead2; font-size: 14px; font-weight: 700; background: transparent; }");

    m_pauseButton = new QPushButton("暂停", this);
    m_backButton = new QPushButton("返回主菜单", this);

    const QString buttonStyle =
        "QPushButton {"
        " color: #f8ead2;"
        " background: rgba(42, 27, 18, 105);"
        " border: 1px solid rgba(248, 234, 210, 95);"
        " border-radius: 4px;"
        " padding: 7px 14px;"
        " font-size: 15px;"
        " font-weight: 700;"
        "}"
        "QPushButton:hover { background: rgba(77, 48, 31, 170); }"
        "QPushButton:pressed { background: rgba(30, 18, 14, 210); }";
    m_pauseButton->setStyleSheet(buttonStyle);
    m_backButton->setStyleSheet(buttonStyle);

    layout->addWidget(coinIcon);
    layout->addWidget(m_goldLabel);
    layout->addSpacing(8);
    layout->addWidget(m_hpLabel);
    layout->addSpacing(8);
    layout->addWidget(m_waveLabel);
    layout->addStretch();
    layout->addWidget(m_towerHintLabel);
    layout->addWidget(createTowerButton(TowerType::ArrowTower, ":/images/tower_crossbow.png", "50"));
    layout->addWidget(createTowerButton(TowerType::MageTower, ":/images/tower_magic.png", "100"));
    layout->addWidget(createTowerButton(TowerType::CannonTower, ":/images/tower_cannon.png", "150"));
    layout->addWidget(createTowerButton(TowerType::IceTower, ":/images/tower_flame.png", "80"));
    layout->addWidget(createTowerButton(TowerType::HolyTower, ":/images/tower_shrine.png", "120"));
    layout->addSpacing(16);
    layout->addWidget(m_pauseButton);
    layout->addWidget(m_backButton);

    connect(m_pauseButton, &QPushButton::clicked, this, &HUDWidget::pauseClicked);
    connect(m_backButton, &QPushButton::clicked, this, &HUDWidget::backClicked);
    refreshTowerButtonStyles();
}

void HUDWidget::setStats(int gold, int hp, int maxHp, int wave, int totalWave)
{
    if(m_goldLabel){
        m_goldLabel->setText(QString("Gold %1").arg(gold));
    }
    if(m_hpLabel){
        m_hpLabel->setText(QString("HP %1/%2").arg(hp).arg(maxHp));
    }
    if(m_waveLabel){
        m_waveLabel->setText(QString("Wave %1/%2").arg(wave).arg(totalWave));
    }
}

void HUDWidget::setPaused(bool paused)
{
    if(m_pauseButton){
        m_pauseButton->setText(paused ? "继续" : "暂停");
    }
}

void HUDWidget::setSelectedTower(TowerType type)
{
    m_selectedTower = type;
    refreshTowerButtonStyles();
}

void HUDWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(18, 14, 12, 68));
}

QPushButton* HUDWidget::createTowerButton(TowerType type, const QString& iconPath, const QString& text)
{
    auto* button = new TowerDragButton(type, iconPath, text, this);

    m_towerButtons.append(button);
    m_towerButtonTypes.append(type);

    connect(button, &QPushButton::clicked, this, [this, type]() {
        setSelectedTower(type);
        emit towerSelected(type);
    });

    return button;
}

void HUDWidget::refreshTowerButtonStyles()
{
    for(int i = 0; i < m_towerButtons.size(); ++i){
        m_towerButtons[i]->setStyleSheet(towerButtonStyle(m_towerButtonTypes[i] == m_selectedTower));
    }
}

QString HUDWidget::towerButtonStyle(bool selected) const
{
    if(selected){
        return
            "QPushButton {"
            " color: #fff7d8;"
            " background: rgba(108, 74, 34, 195);"
            " border: 2px solid rgba(255, 226, 134, 230);"
            " border-radius: 5px;"
            " font-size: 12px;"
            " font-weight: 800;"
            " text-align: bottom center;"
            " padding-bottom: 2px;"
            "}"
            "QPushButton:hover { background: rgba(130, 88, 40, 220); }";
    }

    return
        "QPushButton {"
        " color: #f8ead2;"
        " background: rgba(42, 27, 18, 115);"
        " border: 1px solid rgba(248, 234, 210, 95);"
        " border-radius: 5px;"
        " font-size: 12px;"
        " font-weight: 800;"
        " text-align: bottom center;"
        " padding-bottom: 2px;"
        "}"
        "QPushButton:hover { background: rgba(77, 48, 31, 175); }";
}