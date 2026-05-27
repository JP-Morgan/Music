#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent)
    , m_player(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_playMode(PlayMode::Sequential)
    , m_muted(false)
    , m_previousVolume(0.5f)
{
    m_player->setAudioOutput(m_audioOutput);

    connect(m_player, &QMediaPlayer::durationChanged, this, &MusicPlayer::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MusicPlayer::positionChanged);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &MusicPlayer::stateChanged);
    connect(m_audioOutput, &QAudioOutput::volumeChanged, this, &MusicPlayer::volumeChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MusicPlayer::mediaStatusChanged);
    connect(m_player, &QMediaPlayer::errorOccurred, this, [this](QMediaPlayer::Error error, const QString& errorString) {
        Q_UNUSED(error)
        emit errorOccurred(errorString);
    });
}

MusicPlayer::~MusicPlayer() {}

void MusicPlayer::setSource(const QString& filePath)
{
    m_player->setSource(QUrl::fromLocalFile(filePath));
}

void MusicPlayer::play() { m_player->play(); }
void MusicPlayer::pause() { m_player->pause(); }
void MusicPlayer::stop() { m_player->stop(); }

void MusicPlayer::setVolume(float volume)
{
    m_audioOutput->setVolume(volume);
    if (volume > 0.0f && m_muted)
    {
        m_muted = false;
        emit mutedChanged(false);
    }
}

void MusicPlayer::seek(qint64 position) { m_player->setPosition(position); }
bool MusicPlayer::isPlaying() const { return m_player->playbackState() == QMediaPlayer::PlayingState; }
qint64 MusicPlayer::duration() const { return m_player->duration(); }
qint64 MusicPlayer::position() const { return m_player->position(); }
float MusicPlayer::volume() const { return m_audioOutput->volume(); }
PlayMode MusicPlayer::playMode() const { return m_playMode; }
bool MusicPlayer::isMuted() const { return m_muted; }
QString MusicPlayer::errorString() const { return m_player->errorString(); }
bool MusicPlayer::hasError() const { return m_player->error() != QMediaPlayer::NoError; }

void MusicPlayer::setPlayMode(PlayMode mode)
{
    if (m_playMode != mode)
    {
        m_playMode = mode;
        emit playModeChanged(mode);
    }
}

void MusicPlayer::setMuted(bool muted)
{
    if (m_muted != muted)
    {
        m_muted = muted;
        if (muted)
        {
            m_previousVolume = m_audioOutput->volume();
            m_audioOutput->setVolume(0.0f);
        }
        else
        {
            m_audioOutput->setVolume(m_previousVolume);
        }
        emit mutedChanged(muted);
    }
}
