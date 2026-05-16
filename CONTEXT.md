# My Music — 领域语言

## 领域概念

- **音乐播放器 (MusicPlayer)** — 封装 QMediaPlayer 的音频播放能力，提供播放、暂停、停止、音量控制、静音、播放模式切换等基础操作
- **音乐库 (MusicLibrary)** — 管理本地音乐文件元数据列表（MusicMeta），提供索引导航、搜索、排序功能
- **播放控制器 (PlaybackController)** — 编排层，协调 MusicPlayer 与 MusicLibrary 之间的交互，处理自动下一首、播放模式逻辑
- **音乐元数据 (MusicMeta)** — 描述一首音乐的元数据结构，包含 filePath、title、artist、album、duration

## 播放模式

- **Sequential** — 顺序播放，播完最后一首停止
- **RepeatOne** — 单曲循环，当前曲目播放完毕后重新播放
- **RepeatAll** — 列表循环，播完最后一首回到第一首继续
- **Shuffle** — 随机播放，下一首随机选取

## UI 区域

- **header** — 窗口标题栏区域，包含搜索、窗口控制按钮
- **sidebar** — 左侧导航菜单（线上音乐、我的音乐等）
- **contentArea** — 右侧内容展示区，含 QStackedWidget 多页面切换
- **songInfoWidget** — 当前播放歌曲信息展示（封面、标题、歌手）
- **playbackControls** — 播放控制区域（上一首、播放/暂停、停止、下一首、音量）
- **progressBar** — 播放进度显示
- **timeInfoWidget** — 当前时间/总时间显示

## 架构层次

```
MyMusic (UI 层)
    ↓
PlaybackController (编排层)
    ↓         ↓
MusicPlayer   MusicLibrary
(播放引擎)    (数据层)
```
