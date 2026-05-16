#pragma once

#include <QObject>
#include <QStringList>
#include <QFileInfoList>

struct MusicMeta
{
    QString filePath;
    QString title;
    QString artist;
    QString album;
    qint64 duration = 0;
};

class MusicLibrary : public QObject
{
    Q_OBJECT

public:
    explicit MusicLibrary(QObject *parent = nullptr);
    ~MusicLibrary() override;

    void loadFromDirectory(const QString& directory);

    QStringList musicFiles() const;
    QString currentFile() const;
    int currentIndex() const;
    int count() const;

    MusicMeta metaAt(int index) const;
    QList<MusicMeta> allMeta() const;

    void setCurrentIndex(int index);
    void next();
    void previous();
    bool hasNext() const;
    bool hasPrevious() const;

    void updateDuration(int index, qint64 duration);

    QList<MusicMeta> search(const QString& keyword) const;
    void sortByTitle();
    void sortByArtist();

signals:
    void libraryChanged();
    void currentChanged(int index);

private:
    QList<MusicMeta> m_musicList;
    int m_currentIndex;

    bool isMusicFile(const QFileInfo& fileInfo) const;
    MusicMeta parseMeta(const QFileInfo& fileInfo) const;
};
