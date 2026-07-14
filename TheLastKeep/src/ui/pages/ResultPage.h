#pragma once

#include <QPixmap>
#include <QString>
#include <QWidget>

class QFrame;
class QLabel;
class QPushButton;
class QTimer;

class ResultPage : public QWidget {
    Q_OBJECT

public:
    explicit ResultPage(QWidget *parent = nullptr);

    void setResult(bool win, int score, int levelId, const QString &backgroundPath);
    void stopAutoReturnTimer();

signals:
    void backToMenuClicked();
    void restartClicked();
    void nextLevelClicked(int nextLevelId);
    void autoReturnToMainRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUi();
    void setupButton(QPushButton *button, bool primary) const;
    void setBackgroundPath(const QString &backgroundPath);

private:
    QPixmap m_background;
    QFrame *m_panel = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_messageLabel = nullptr;
    QLabel *m_scoreLabel = nullptr;

    QPushButton *m_restartButton = nullptr;
    QPushButton *m_nextLevelButton = nullptr;
    QPushButton *m_backButton = nullptr;
    QTimer *m_autoReturnTimer = nullptr;

    int m_levelId = 0;
    bool m_fullscreenResultImage = false;
};