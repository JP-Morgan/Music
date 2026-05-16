#pragma once

#include <QObject>
#include <QTimer>
#include "MusicPlayer.h"

enum class PlaybackState
{
    Stopped,
    Playing,
    Paused
};

class MusicLibrary;

class PlaybackController : public QObject
{
    Q_OBJECT

public:
    PlaybackController(MusicPlayer* player, MusicLibrary* library, QObject* parent = nullptr);

    void play();
    void pause();
    void stop();
    void togglePlay();
    void next();
    void previous();
    void setVolume(int volume);
    void seek(qint64 position);
    void setPlayMode(PlayMode mode);
    void playAt(int index);

    bool isPlaying() const;
    PlayMode playMode() const;
    int volume() const;

signals:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void stateChanged(PlaybackState state);
    void currentMediaChanged(const QString& filePath, const QString& title, const QString& artist);
    void playModeChanged(PlayMode mode);
    void volumeChanged(int volume);

private slots:
    void onPlayerStateChanged(QMediaPlayer::PlaybackState state);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onCurrentChanged(int index);
    void onPositionTimer();

private:
    void playCurrent();
    void reshuffle();
    static PlaybackState fromQtState(QMediaPlayer::PlaybackState state);

    MusicPlayer* m_player;
    MusicLibrary* m_library;
    QTimer* m_positionTimer;
    QList<int> m_shuffledIndices;
    int m_shufflePosition;
};
