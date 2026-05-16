#include "MusicLibrary.h"
#include <QDir>
#include <QFileInfo>
#include <algorithm>

static QString stripNumberPrefix(const QString& name)
{
    int pos = 0;
    while (pos < name.length() && (name[pos].isDigit() || name[pos] == '.' || name[pos] == '-'))
    {
        pos++;
    }
    if (pos > 0 && pos < name.length() && name[pos] == ' ')
    {
        pos++;
    }
    return name.mid(pos).trimmed();
}

MusicLibrary::MusicLibrary(QObject *parent)
    : QObject(parent)
    , m_currentIndex(-1)
{
}

MusicLibrary::~MusicLibrary()
{
}

void MusicLibrary::loadFromDirectory(const QString& directory)
{
    QDir dir(directory);
    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    m_musicList.clear();
    for (const QFileInfo& file : files)
    {
        if (isMusicFile(file))
        {
            m_musicList.append(parseMeta(file));
        }
    }

    m_currentIndex = m_musicList.isEmpty() ? -1 : 0;
    emit libraryChanged();
    if (m_currentIndex >= 0)
    {
        emit currentChanged(m_currentIndex);
    }
}

QStringList MusicLibrary::musicFiles() const
{
    QStringList files;
    for (const MusicMeta& meta : m_musicList)
    {
        files.append(meta.filePath);
    }
    return files;
}

QString MusicLibrary::currentFile() const
{
    if (m_currentIndex >= 0 && m_currentIndex < m_musicList.size())
    {
        return m_musicList.at(m_currentIndex).filePath;
    }
    return QString();
}

int MusicLibrary::currentIndex() const
{
    return m_currentIndex;
}

int MusicLibrary::count() const
{
    return m_musicList.size();
}

MusicMeta MusicLibrary::metaAt(int index) const
{
    if (index >= 0 && index < m_musicList.size())
    {
        return m_musicList.at(index);
    }
    return MusicMeta();
}

QList<MusicMeta> MusicLibrary::allMeta() const
{
    return m_musicList;
}

void MusicLibrary::setCurrentIndex(int index)
{
    if (index >= 0 && index < m_musicList.size() && index != m_currentIndex)
    {
        m_currentIndex = index;
        emit currentChanged(m_currentIndex);
    }
}

void MusicLibrary::next()
{
    if (hasNext())
    {
        m_currentIndex++;
        emit currentChanged(m_currentIndex);
    }
}

void MusicLibrary::previous()
{
    if (hasPrevious())
    {
        m_currentIndex--;
        emit currentChanged(m_currentIndex);
    }
}

bool MusicLibrary::hasNext() const
{
    return m_currentIndex < m_musicList.size() - 1;
}

bool MusicLibrary::hasPrevious() const
{
    return m_currentIndex > 0;
}

void MusicLibrary::updateDuration(int index, qint64 duration)
{
    if (index >= 0 && index < m_musicList.size())
    {
        m_musicList[index].duration = duration;
    }
}

QList<MusicMeta> MusicLibrary::search(const QString& keyword) const
{
    if (keyword.isEmpty())
    {
        return m_musicList;
    }

    QList<MusicMeta> results;
    QString lowerKeyword = keyword.toLower();
    for (const MusicMeta& meta : m_musicList)
    {
        if (meta.title.toLower().contains(lowerKeyword) ||
            meta.artist.toLower().contains(lowerKeyword) ||
            meta.album.toLower().contains(lowerKeyword))
        {
            results.append(meta);
        }
    }
    return results;
}

void MusicLibrary::sortByTitle()
{
    QString currentPath = currentFile();

    std::sort(m_musicList.begin(), m_musicList.end(), [](const MusicMeta& a, const MusicMeta& b) {
        return a.title.toLower() < b.title.toLower();
    });

    for (int i = 0; i < m_musicList.size(); i++)
    {
        if (m_musicList[i].filePath == currentPath)
        {
            m_currentIndex = i;
            break;
        }
    }

    emit libraryChanged();
}

void MusicLibrary::sortByArtist()
{
    QString currentPath = currentFile();

    std::sort(m_musicList.begin(), m_musicList.end(), [](const MusicMeta& a, const MusicMeta& b) {
        if (a.artist == b.artist)
        {
            return a.title.toLower() < b.title.toLower();
        }
        return a.artist.toLower() < b.artist.toLower();
    });

    for (int i = 0; i < m_musicList.size(); i++)
    {
        if (m_musicList[i].filePath == currentPath)
        {
            m_currentIndex = i;
            break;
        }
    }

    emit libraryChanged();
}

bool MusicLibrary::isMusicFile(const QFileInfo& fileInfo) const
{
    QString suffix = fileInfo.suffix().toLower();
    return suffix == "mp3" || suffix == "wav" || suffix == "flac" || suffix == "ogg"
        || suffix == "aac" || suffix == "m4a" || suffix == "wma"
        || suffix == "aiff" || suffix == "aif" || suffix == "opus";
}

MusicMeta MusicLibrary::parseMeta(const QFileInfo& fileInfo) const
{
    MusicMeta meta;
    meta.filePath = fileInfo.absoluteFilePath();

    QString fileName = fileInfo.completeBaseName();
    QString cleanName = stripNumberPrefix(fileName);

    int sepIndex = cleanName.indexOf(" - ");
    if (sepIndex > 0)
    {
        meta.artist = cleanName.left(sepIndex).trimmed();
        meta.title = cleanName.mid(sepIndex + 3).trimmed();
    }
    else
    {
        meta.title = cleanName;
    }

    if (meta.artist.isEmpty())
    {
        QDir dir(fileInfo.absolutePath());
        QString folderName = dir.dirName();
        if (!folderName.isEmpty() && folderName != ".")
        {
            meta.artist = folderName;
        }
    }

    return meta;
}
