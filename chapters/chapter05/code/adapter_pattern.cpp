#include <iostream>
#include <memory>
#include <windows.h>

using namespace std;

// 目标接口 - 媒体播放器
class MediaPlayer {
public:
    virtual ~MediaPlayer() = default;
    virtual void play(const string& audioType, const string& fileName) = 0;
};

// 高级媒体播放器接口
class AdvancedMediaPlayer {
public:
    virtual ~AdvancedMediaPlayer() = default;
    virtual void playVlc(const string& fileName) = 0;
    virtual void playMp4(const string& fileName) = 0;
};

// 具体实现类 - VLC播放器
class VlcPlayer : public AdvancedMediaPlayer {
public:
    void playVlc(const string& fileName) override {
        cout << "使用VLC播放器播放: " << fileName << endl;
    }
    void playMp4(const string& fileName) override {
        // VLC播放器不支持MP4
    }
};

// 具体实现类 - MP4播放器
class Mp4Player : public AdvancedMediaPlayer {
public:
    void playVlc(const string& fileName) override {
        // MP4播放器不支持VLC
    }
    void playMp4(const string& fileName) override {
        cout << "使用MP4播放器播放: " << fileName << endl;
    }
};

// 适配器类，实现MediaPlayer接口，内部组合AdvancedMediaPlayer
class MediaAdapter : public MediaPlayer {
private:
    unique_ptr<AdvancedMediaPlayer> advancedPlayer;
public:
    MediaAdapter(const string& audioType) {
        if (audioType == "vlc") {
            advancedPlayer = make_unique<VlcPlayer>();
        } else if (audioType == "mp4") {
            advancedPlayer = make_unique<Mp4Player>();
        }
    }
    void play(const string& audioType, const string& fileName) override {
        if (audioType == "vlc") {
            advancedPlayer->playVlc(fileName);
        } else if (audioType == "mp4") {
            advancedPlayer->playMp4(fileName);
        }
    }
};

// 已有的Mp3Player类，实现MediaPlayer接口，只能播放MP3
class Mp3Player : public MediaPlayer {
public:
    void play(const string& audioType, const string& fileName) override {
        if (audioType == "mp3") {
            cout << "使用内置播放器播放MP3: " << fileName << endl;
        } else {
            cout << "Mp3Player不支持的格式: " << audioType << endl;
        }
    }
};

// 客户端只依赖MediaPlayer接口，通过MediaAdapter适配多种格式
class AudioPlayer : public MediaPlayer {
private:
    unique_ptr<MediaAdapter> mediaAdapter;
    unique_ptr<Mp3Player> mp3Player;
public:
    AudioPlayer() {
        mp3Player = make_unique<Mp3Player>();
    }
    void play(const string& audioType, const string& fileName) override {
        if (audioType == "mp3") {
            mp3Player->play(audioType, fileName);
        } else if (audioType == "vlc" || audioType == "mp4") {
            mediaAdapter = make_unique<MediaAdapter>(audioType);
            mediaAdapter->play(audioType, fileName);
        } else {
            cout << "不支持的音频格式: " << audioType << endl;
        }
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== 适配器模式演示 ===" << endl;
    auto audioPlayer = make_unique<AudioPlayer>();

    cout << "\n1. 播放MP3文件:" << endl;
    audioPlayer->play("mp3", "beyond_the_horizon.mp3");

    cout << "\n2. 播放MP4文件:" << endl;
    audioPlayer->play("mp4", "alone.mp4");

    cout << "\n3. 播放VLC文件:" << endl;
    audioPlayer->play("vlc", "far_far_away.vlc");

    cout << "\n4. 播放不支持的文件格式:" << endl;
    audioPlayer->play("avi", "mind_me.avi");

    cin.get();
    return 0;
}