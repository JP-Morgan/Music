# My Music — 领域语言

## 领域概念

- **MusicPlayer** — 封装 QMediaPlayer 的音频播放能力
- **MusicLibrary** — 管理本地音乐文件元数据列表（MusicMeta）
- **PlaybackController** — 编排层，协调 MusicPlayer 与 MusicLibrary
- **MusicMeta** — 描述一首音乐的元数据结构（filePath、title、artist、album、duration）

## 播放模式

- **Sequential** — 顺序播放
- **RepeatOne** — 单曲循环
- **RepeatAll** — 列表循环
- **Shuffle** — 随机播放

## 架构

```
MyMusic (UI)
    ↓
PlaybackController (编排)
    ↓         ↓
MusicPlayer    MusicLibrary
```
