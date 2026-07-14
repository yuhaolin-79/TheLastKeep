#pragma once

#include <QPixmap>
#include <QWidget>

class QCheckBox;
class QPushButton;

class SettingsPage : public QWidget {
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);

    void setMusicEnabled(bool enabled);

signals:
    void musicEnabledChanged(bool enabled);
    void backClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUi();

private:
    QCheckBox *m_musicCheckBox = nullptr;
    QPushButton *m_backButton = nullptr;
    QPixmap m_background;
};
