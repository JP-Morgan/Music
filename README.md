# 🎵 个人音乐播放器

基于 Qt 6 + QMediaPlayer 构建的轻量级本地音乐播放器。

## ✨ 功能

- 🎧 **多格式支持**：MP3、WAV、FLAC、OGG、AAC、M4A、WMA、AIFF、Opus
- 🔀 **四种播放模式**：顺序、单曲循环、列表循环、随机播放
- 🔊 **音量控制**：支持音量调节与静音切换
- 📂 **元数据解析**：自动从文件名解析歌手、标题（如 `周杰伦 - 夜曲.mp3`）
- 🔍 **音乐搜索**：按歌名、歌手、专辑搜索
- ↕️ **排序功能**：按歌名或歌手排序
- 🎨 **现代 UI**：无边框窗口、窗口拖拽移动

## 🏗️ 架构

```
MyMusic (UI 层)
    ↓
PlaybackController (编排层)
    ↓         ↓
MusicPlayer    MusicLibrary
```

## 🛠️ 技术栈

- Qt 6.10.2 | QMediaPlayer + QAudioOutput
- Visual Studio 2022 + v145 | MSVC

## 🚀 编译

用 VS 打开 `.sln` 或 `.slnx` 文件，编译 x64 Debug/Release。

## 📁 项目结构

```
My_Music/
├── main.cpp / My_Music.h / .cpp / .ui   # 主窗口
├── MusicPlayer.h / .cpp                  # 播放器封装
├── MusicLibrary.h / .cpp                 # 音乐库管理
├── PlaybackController.h / .cpp           # 播放编排
└── My_Music.vcxproj                     # VS 项目文件
```

*文件名格式：`歌手 - 歌名.mp3` 或直接歌名*
