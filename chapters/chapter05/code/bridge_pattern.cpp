#include <iostream>
#include <memory>
#include <string>
#include <windows.h>

using namespace std;

// 音频格式实现接口
class AudioFormat {
public:
    virtual ~AudioFormat() = default;
    virtual void play(const string& fileName) = 0;
};

// 具体实现：MP3格式
class Mp3Format : public AudioFormat {
public:
    void play(const string& fileName) override {
        cout << "播放MP3音频文件: " << fileName << endl;
    }
};

// 具体实现：MP4格式
class Mp4Format : public AudioFormat {
public:
    void play(const string& fileName) override {
        cout << "播放MP4音频文件: " << fileName << endl;
    }
};

// 具体实现：VLC格式
class VlcFormat : public AudioFormat {
public:
    void play(const string& fileName) override {
        cout << "播放VLC音频文件: " << fileName << endl;
    }
};

// 抽象播放器
class Player {
protected:
    unique_ptr<AudioFormat> format;
public:
    Player(unique_ptr<AudioFormat> fmt) : format(move(fmt)) {}
    virtual ~Player() = default;
    virtual void play(const string& fileName) = 0;
};

// Windows平台播放器
class WindowsPlayer : public Player {
public:
    WindowsPlayer(unique_ptr<AudioFormat> fmt) : Player(move(fmt)) {}
    void play(const string& fileName) override {
        cout << "[Windows平台] ";
        format->play(fileName);
    }
};

// Linux平台播放器
class LinuxPlayer : public Player {
public:
    LinuxPlayer(unique_ptr<AudioFormat> fmt) : Player(move(fmt)) {}
    void play(const string& fileName) override {
        cout << "[Linux平台] ";
        format->play(fileName);
    }
};

// Mac平台播放器
class MacPlayer : public Player {
public:
    MacPlayer(unique_ptr<AudioFormat> fmt) : Player(move(fmt)) {}
    void play(const string& fileName) override {
        cout << "[Mac平台] ";
        format->play(fileName);
    }
};

int main() {
    // 设置控制台编码为UTF-8，解决中文显示问题
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 桥接模式演示 ===" << endl;

    cout << "\n1. Windows平台播放MP3:" << endl;
    auto winMp3 = make_unique<WindowsPlayer>(make_unique<Mp3Format>());
    winMp3->play("beyond_the_horizon.mp3");

    cout << "\n2. Linux平台播放MP4:" << endl;
    auto linuxMp4 = make_unique<LinuxPlayer>(make_unique<Mp4Format>());
    linuxMp4->play("alone.mp4");

    cout << "\n3. Mac平台播放VLC:" << endl;
    auto macVlc = make_unique<MacPlayer>(make_unique<VlcFormat>());
    macVlc->play("far_far_away.vlc");

    cout << "\n4. Windows平台播放VLC:" << endl;
    auto winVlc = make_unique<WindowsPlayer>(make_unique<VlcFormat>());
    winVlc->play("mind_me.vlc");

    cin.get();
    return 0;
}