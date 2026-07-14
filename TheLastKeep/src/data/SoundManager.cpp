#include "data/SoundManager.h"

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QSettings>
#include <QUrl>

namespace {
constexpr auto kMusicEnabledKey = "audio/musicEnabled";
constexpr auto kBackgroundMusicPath = "qrc:/sounds/main_music.mp3";
constexpr auto kButtonClickSoundPath = "qrc:/sounds/button_click.mp3";
constexpr float kBackgroundMusicVolume = 0.45f;
constexpr float kButtonClickVolume = 0.75f;
}

SoundManager &SoundManager::instance()
{
    static SoundManager manager;
    return manager;
}

SoundManager::SoundManager()
{
    QSettings settings;
    m_musicEnabled = settings.value(kMusicEnabledKey, true).toBool();
}

SoundManager::~SoundManager() = default;

void SoundManager::startBackgroundMusic()
{
    if (!m_musicEnabled) {
        return;
    }

    ensurePlayer();
    if (m_player->playbackState() != QMediaPlayer::PlayingState) {
        m_player->play();
    }
}

void SoundManager::stopBackgroundMusic()
{
    if (m_player) {
        m_player->stop();
    }
}

void SoundManager::playButtonClick()
{
    ensureButtonClickPlayer();
    m_buttonClickPlayer->stop();
    m_buttonClickPlayer->setPosition(0);
    m_buttonClickPlayer->play();
}

void SoundManager::setMusicEnabled(bool enabled)
{
    if (m_musicEnabled == enabled) {
        return;
    }

    m_musicEnabled = enabled;
    saveMusicEnabled();

    if (m_musicEnabled) {
        startBackgroundMusic();
    } else {
        stopBackgroundMusic();
    }
}

bool SoundManager::isMusicEnabled() const
{
    return m_musicEnabled;
}

void SoundManager::ensurePlayer()
{
    if (m_player) {
        return;
    }

    m_audioOutput = std::make_unique<QAudioOutput>();
    m_audioOutput->setVolume(kBackgroundMusicVolume);

    m_player = std::make_unique<QMediaPlayer>();
    m_player->setAudioOutput(m_audioOutput.get());
    m_player->setSource(QUrl(kBackgroundMusicPath));
    m_player->setLoops(QMediaPlayer::Infinite);
}

void SoundManager::ensureButtonClickPlayer()
{
    if (m_buttonClickPlayer) {
        return;
    }

    m_buttonClickAudioOutput = std::make_unique<QAudioOutput>();
    m_buttonClickAudioOutput->setVolume(kButtonClickVolume);

    m_buttonClickPlayer = std::make_unique<QMediaPlayer>();
    m_buttonClickPlayer->setAudioOutput(m_buttonClickAudioOutput.get());
    m_buttonClickPlayer->setSource(QUrl(kButtonClickSoundPath));
}

void SoundManager::saveMusicEnabled() const
{
    QSettings settings;
    settings.setValue(kMusicEnabledKey, m_musicEnabled);
}
