#pragma once

#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include "ui_My_Music.h"
#include "MusicPlayer.h"
#include "MusicLibrary.h"

class PlaybackController;

class MyMusic : public QWidget
{
    Q_OBJECT

public:
    explicit MyMusic(QWidget *parent = nullptr);
    ~MyMusic() override;

private:
    void initWindowStyle();
    void setupConnections();

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onMediaInfoChanged(const QString& filePath, const QString& title, const QString& artist);

private:
    Ui::My_MusicClass ui;
    QPoint m_dragPosition;
    MusicPlayer* m_player;
    MusicLibrary* m_library;
    PlaybackController* m_controller;
};
