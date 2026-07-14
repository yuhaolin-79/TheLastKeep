#pragma once

#include <QPixmap>
#include <QWidget>

class QLabel;
class QPushButton;
class QTextBrowser;

class StoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit StoryPage(QWidget *parent = nullptr);
    void setLevel(int levelId);

signals:
    void continueClicked();
    void backClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUi();
    QString storyTitle(int levelId) const;
    QString storyResourcePath(int levelId) const;
    QString loadStoryText(int levelId) const;
    void setupCommandButton(QPushButton *button, bool primary) const;

private:
    QPixmap m_background;
    QLabel *m_titleLabel = nullptr;
    QTextBrowser *m_storyText = nullptr;
    QPushButton *m_continueButton = nullptr;
    QPushButton *m_backButton = nullptr;
    int m_levelId = 0;
};