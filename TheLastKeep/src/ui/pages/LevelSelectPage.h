/* Directory: src/ui/pages
 * Purpose: level selection page.
 * Shows the level select background, level buttons, and back button.
 */

#pragma once

#include <QWidget>
#include <QPixmap>

class QPushButton;
class QString;

class LevelSelectPage : public QWidget {
    Q_OBJECT

public:
    explicit LevelSelectPage(QWidget *parent = nullptr);

signals:
    void levelSelected(int levelId);
    void backClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUi();
    void setupImageButton(QPushButton *button, const QString &imagePath);

private:
    QPushButton *m_tutorialButton = nullptr;
    QPushButton *m_level1Button = nullptr;
    QPushButton *m_level2Button = nullptr;
    QPushButton *m_level3Button = nullptr;
    QPushButton *m_backButton = nullptr;

    QPixmap m_background;
};
