#include "PlaybackController.h"
#include "MusicLibrary.h"
#include <QRandomGenerator>

PlaybackController::PlaybackController(MusicPlayer* player, MusicLibrary* library, QObject* parent)
    : QObject(parent)
    , m_player(player)
    , m_library(library)
    , m_positionTimer(new QTimer(this))
    , m_shufflePosition(0)
{
    m_positionTimer->setInterval(200);

    connect(m_player, &MusicPlayer::stateChanged, this, &PlaybackController::onPlayerStateChanged);
    connect(m_player, &MusicPlayer::durationChanged, this, &PlaybackController::durationChanged);
    connect(m_player, &MusicPlayer::volumeChanged, this, [this](float vol) {
        emit volumeChanged(static_cast<int>(vol * 100.0f));
    });
    connect(m_player, &MusicPlayer::playModeChanged, this, &PlaybackController::playModeChanged);
    connect(m_player, &MusicPlayer::mediaStatusChanged, this, &PlaybackController::onMediaStatusChanged);
    connect(m_library, &MusicLibrary::currentChanged, this, &PlaybackController::onCurrentChanged);
    connect(m_positionTimer, &QTimer::timeout, this, &PlaybackController::onPositionTimer);
}

void PlaybackController::play()
{
    if (m_library->count() == 0)
    {
        return;
    }

    QString file = m_library->currentFile();
    if (file.isEmpty())
    {
        return;
    }

    if (m_player->playMode() == PlayMode::Shuffle && m_shuffledIndices.isEmpty())
    {
        reshuffle();
    }

    if (m_player->isPlaying())
    {
        return;
    }

    m_player->setSource(file);
    m_player->play();
    m_positionTimer->start();
}

void PlaybackController::pause()
{
    m_player->pause();
    m_positionTimer->stop();
}

void PlaybackController::stop()
{
    m_player->stop();
    m_positionTimer->stop();
}

void PlaybackController::togglePlay()
{
    if (m_player->isPlaying())
    {
        pause();
    }
    else
    {
        play();
    }
}

void PlaybackController::next()
{
    PlayMode mode = m_player->playMode();
    if (mode == PlayMode::Shuffle)
    {
        if (m_library->count() == 0)
        {
            return;
        }
        if (m_shuffledIndices.isEmpty())
        {
            reshuffle();
        }
        m_shufflePosition = (m_shufflePosition + 1) % m_shuffledIndices.size();
        m_library->setCurrentIndex(m_shuffledIndices[m_shufflePosition]);
    }
    else
    {
        if (m_library->hasNext())
        {
            m_library->next();
        }
        else if (mode == PlayMode::RepeatAll)
        {
            m_library->setCurrentIndex(0);
        }
    }
    playCurrent();
}

void PlaybackController::previous()
{
    PlayMode mode = m_player->playMode();
    if (mode == PlayMode::Shuffle)
    {
        if (m_shuffledIndices.isEmpty())
        {
            reshuffle();
        }
        if (m_shufflePosition > 0)
        {
            m_shufflePosition--;
        }
        else
        {
            m_shufflePosition = m_shuffledIndices.size() - 1;
        }
        m_library->setCurrentIndex(m_shuffledIndices[m_shufflePosition]);
    }
    else
    {
        if (m_library->hasPrevious())
        {
            m_library->previous();
        }
        else if (mode == PlayMode::RepeatAll)
        {
            m_library->setCurrentIndex(m_library->count() - 1);
        }
    }
    playCurrent();
}

void PlaybackController::setVolume(int volume)
{
    m_player->setVolume(volume / 100.0f);
}

void PlaybackController::seek(qint64 position)
{
    m_player->seek(position);
}

void PlaybackController::setPlayMode(PlayMode mode)
{
    if (mode == PlayMode::Shuffle)
    {
        reshuffle();
    }
    else
    {
        m_shuffledIndices.clear();
    }
    m_player->setPlayMode(mode);
}

void PlaybackController::playAt(int index)
{
    if (m_player->playMode() == PlayMode::Shuffle)
    {
        for (int i = 0; i < m_shuffledIndices.size(); i++)
        {
            if (m_shuffledIndices[i] == index)
            {
                m_shufflePosition = i;
                break;
            }
        }
    }
    m_library->setCurrentIndex(index);
    playCurrent();
}

bool PlaybackController::isPlaying() const
{
    return m_player->isPlaying();
}

PlayMode PlaybackController::playMode() const
{
    return m_player->playMode();
}

int PlaybackController::volume() const
{
    return static_cast<int>(m_player->volume() * 100.0f);
}

void PlaybackController::onPlayerStateChanged(QMediaPlayer::PlaybackState state)
{
    emit stateChanged(fromQtState(state));
}

void PlaybackController::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia)
    {
        m_positionTimer->stop();

        PlayMode mode = m_player->playMode();
        if (mode == PlayMode::RepeatOne)
        {
            m_player->seek(0);
            m_player->play();
            m_positionTimer->start();
        }
        else if (mode == PlayMode::Sequential && !m_library->hasNext())
        {
            stop();
        }
        else
        {
            next();
        }
    }
}

void PlaybackController::onCurrentChanged(int index)
{
    MusicMeta meta = m_library->metaAt(index);
    if (!meta.filePath.isEmpty())
    {
        emit currentMediaChanged(meta.filePath, meta.title, meta.artist);
    }
}

void PlaybackController::onPositionTimer()
{
    emit positionChanged(m_player->position());
}

void PlaybackController::playCurrent()
{
    QString file = m_library->currentFile();
    if (!file.isEmpty())
    {
        m_player->setSource(file);
        m_player->play();
        m_positionTimer->start();
    }
}

void PlaybackController::reshuffle()
{
    m_shuffledIndices.clear();
    int count = m_library->count();
    if (count == 0)
    {
        return;
    }

    for (int i = 0; i < count; i++)
    {
        m_shuffledIndices.append(i);
    }

    for (int i = m_shuffledIndices.size() - 1; i > 0; i--)
    {
        int j = QRandomGenerator::global()->bounded(i + 1);
        m_shuffledIndices.swapItemsAt(i, j);
    }

    m_shufflePosition = 0;
}

PlaybackState PlaybackController::fromQtState(QMediaPlayer::PlaybackState state)
{
    switch (state)
    {
    case QMediaPlayer::PlayingState:
        return PlaybackState::Playing;
    case QMediaPlayer::PausedState:
        return PlaybackState::Paused;
    case QMediaPlayer::StoppedState:
    default:
        return PlaybackState::Stopped;
    }
}
