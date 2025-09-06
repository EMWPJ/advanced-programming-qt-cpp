#include <iostream>
#include <memory>
#include <string>
#include <windows.h>
using namespace std;
// 播放器组件接口
class Player {
public:
    virtual ~Player() = default;
    virtual void play() = 0;
};
// 具体播放器
class ConcretePlayer : public Player {
public:
    void play() override {
        cout << "播放音频文件" << endl;
    }
};

// 装饰器基类
class PlayerDecorator : public Player {
protected:
    unique_ptr<Player> player;
public:
    PlayerDecorator(unique_ptr<Player> p) : player(move(p)) {}
    virtual ~PlayerDecorator() = default;
};
// 具体装饰器：均衡器
class EqualizerDecorator : public PlayerDecorator {
public:
    EqualizerDecorator(unique_ptr<Player> p) : PlayerDecorator(move(p)) {}
    void play() override {
        player->play();
        cout << "均衡器功能：调整音效" << endl;
    }
};

// 具体装饰器：音量增强
class VolumeBoostDecorator : public PlayerDecorator {
public:
    VolumeBoostDecorator(unique_ptr<Player> p) : PlayerDecorator(move(p)) {}
    void play() override {
        player->play();
        cout << "音量增强功能：提升音量" << endl;
    }
};
// 具体装饰器：歌词显示
class LyricsDecorator : public PlayerDecorator {
public:
    LyricsDecorator(unique_ptr<Player> p) : PlayerDecorator(move(p)) {}
    void play() override {
        player->play();
        cout << "歌词显示功能：同步显示歌词" << endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 装饰器模式演示 ===" << endl;
    // 基础播放器
    unique_ptr<Player> player = make_unique<ConcretePlayer>();
    cout << "\n基础播放器：" << endl;
    player->play();
    // 增加均衡器功能
    player = make_unique<EqualizerDecorator>(move(player));
    cout << "\n添加均衡器功能后：" << endl;
    player->play();
    // 增加音量增强功能
    player = make_unique<VolumeBoostDecorator>(move(player));
    cout << "\n再添加音量增强功能后：" << endl;
    player->play();
    // 增加歌词显示功能
    player = make_unique<LyricsDecorator>(move(player));
    cout << "\n再添加歌词显示功能后：" << endl;
    player->play();

    cin.get();
    return 0;
}
