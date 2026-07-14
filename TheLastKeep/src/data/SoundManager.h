#pragma once

#include <memory>

class QAudioOutput;
class QMediaPlayer;

class SoundManager {
public:
    static SoundManager &instance();

    void startBackgroundMusic();
    void stopBackgroundMusic();
    void playButtonClick();
    void setMusicEnabled(bool enabled);
    bool isMusicEnabled() const;

private:
    SoundManager();
    ~SoundManager();

    SoundManager(const SoundManager &) = delete;
    SoundManager &operator=(const SoundManager &) = delete;

    void ensurePlayer();
    void ensureButtonClickPlayer();
    void saveMusicEnabled() const;

private:
    bool m_musicEnabled = true;
    std::unique_ptr<QAudioOutput> m_audioOutput;
    std::unique_ptr<QMediaPlayer> m_player;
    std::unique_ptr<QAudioOutput> m_buttonClickAudioOutput;
    std::unique_ptr<QMediaPlayer> m_buttonClickPlayer;
};
