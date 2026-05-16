# 🎵 个人音乐播放器

基于 Qt 6 + QMediaPlayer 构建的轻量级本地音乐播放器，支持多种音频格式与播放模式。

## ✨ 功能特性

- 🎧 **多格式支持**：MP3、WAV、FLAC、OGG、AAC、M4A、WMA、AIFF、Opus
- 🔀 **四种播放模式**：顺序播放、单曲循环、列表循环、随机播放
- 🔊 **音量控制**：支持音量调节与静音切换
- 📂 **元数据解析**：自动从文件名解析歌手、标题（如 `周杰伦 - 夜曲.mp3`）
- 🔍 **音乐搜索**：按歌名、歌手、专辑搜索本地音乐
- ↕️ **排序功能**：按歌名或歌手排序音乐列表
- 🎨 **现代 UI**：无边框窗口、窗口拖拽移动

## 🏗️ 架构设计

```
MyMusic (UI 层)
    ↓
PlaybackController (编排层)
    ↓         ↓
MusicPlayer   MusicLibrary
(播放引擎)    (数据层)
```

- **MusicPlayer**：封装 QMediaPlayer，提供播放控制、音量、播放模式
- **MusicLibrary**：管理音乐元数据，支持搜索与排序
- **PlaybackController**：协调播放逻辑，处理自动切歌、播放模式切换

## 🛠️ 技术栈

- **Qt 6.10.2** — 跨平台 GUI 框架
- **QMediaPlayer + QAudioOutput** — 音频播放
- **Visual Studio 2022 + v145** — 编译环境
- **MSVC** — 编译器

## 🚀 编译

```bash
# 使用 Visual Studio 打开 .sln 文件，编译 x64 Debug 或 Release 配置
```

## 📁 项目结构

```
My_Music/
├── main.cpp                    # 程序入口
├── My_Music.h / .cpp           # 主窗口
├── My_Music.ui                 # Qt UI 设计文件
├── MusicPlayer.h / .cpp        # 播放器封装
├── MusicLibrary.h / .cpp      # 音乐库管理
├── PlaybackController.h / .cpp # 播放编排控制器
└── My_Music.vcxproj            # VS 项目文件
```

## 📝 命名规范

文件名格式：`歌手 - 歌名.mp3`（推荐）或直接使用歌名

示例：
- `周杰伦 - 夜曲.mp3`
- `01. 林俊杰 - 江南.flac`

---

*Personal Music Player Project — Built with Qt*
