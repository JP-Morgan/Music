#pragma once

#include <QObject>
#include <QString>
#include <QMediaPlayer>
#include <QAudioOutput>

enum class PlayMode
{
    Sequential,
    RepeatOne,
    RepeatAll,
    Shuffle
};

class MusicPlayer : public QObject
{
    Q_OBJECT

public:
    explicit MusicPlayer(QObject *parent = nullptr);
    ~MusicPlayer() override;

    void setSource(const QString& filePath);
    void play();
    void pause();
    void stop();
    void setVolume(float volume);
    void seek(qint64 position);

    bool isPlaying() const;
    qint64 duration() const;
    qint64 position() const;
    float volume() const;

    PlayMode playMode() const;
    void setPlayMode(PlayMode mode);

    bool isMuted() const;
    void setMuted(bool muted);

    QString errorString() const;
    bool hasError() const;

signals:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void stateChanged(QMediaPlayer::PlaybackState state);
    void volumeChanged(float volume);
    void playModeChanged(PlayMode mode);
    void mutedChanged(bool muted);
    void errorOccurred(const QString& errorString);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
    PlayMode m_playMode;
    bool m_muted;
    float m_previousVolume;
};
