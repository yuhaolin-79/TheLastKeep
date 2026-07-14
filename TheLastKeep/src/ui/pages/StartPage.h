#pragma once

#include <QPixmap>
#include <QWidget>

class QPushButton;
class QString;

class StartPage : public QWidget {
    Q_OBJECT

public:
    explicit StartPage(QWidget *parent = nullptr);

signals:
    void startClicked();
    void levelSelectClicked();
    void settingsClicked();
    void quitClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUi();
    void setupImageButton(QPushButton *button, const QString &imagePath);

private:
    QPushButton *m_startButton = nullptr;
    QPushButton *m_levelSelectButton = nullptr;
    QPushButton *m_settingsButton = nullptr;
    QPushButton *m_quitButton = nullptr;

    QPixmap m_background;
};
