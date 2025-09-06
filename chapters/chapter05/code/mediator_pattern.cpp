#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <windows.h>

using namespace std;

// 前向声明
class Player;

// 抽象中介者
class Mediator {
public:
    virtual void sendMessage(const string& message, Player* sender) = 0;
    virtual void addPlayer(Player* player) = 0;
    virtual ~Mediator() = default;
};

// 抽象同事类（玩家基类）
class Player {
protected:
    Mediator* mediator;
    string name;

public:
    Player(const string& n, Mediator* med) : name(n), mediator(med) {
        if (mediator) {
            mediator->addPlayer(this);
        }
    }

    virtual void receiveMessage(const string& message, Player* sender) = 0;
    virtual void sendMessage(const string& message) = 0;

    const string& getName() const { return name; }

    virtual ~Player() = default;
};

// 具体中介者 - 房间
class Room : public Mediator {
private:
    vector<Player*> players;

public:
    void addPlayer(Player* player) override {
        players.push_back(player);
    }

    void sendMessage(const string& message, Player* sender) override {
        cout << "房间广播消息: [" << sender->getName() << "] " << message << endl;
        // 向所有其他玩家发送消息
        for (auto player : players) {
            if (player != sender) {
                player->receiveMessage(message, sender);
            }
        }
    }
};

// 具体同事类 - 普通玩家
class NormalPlayer : public Player {
public:
    NormalPlayer(const string& name, Mediator* mediator)
        : Player(name, mediator) {}

    void sendMessage(const string& message) override {
        cout << getName() << " 发送消息: " << message << endl;
        mediator->sendMessage(message, this);
    }

    void receiveMessage(const string& message, Player* sender) override {
        cout << getName() << " 收到来自 " << sender->getName()
             << " 的消息: " << message << endl;
    }
};

int main() {
    // 设置控制台编码为UTF-8，解决中文显示问题
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 中介者模式示例：房间集中管理玩家交互 ===" << endl;

    // 创建房间（中介者）
    auto room = make_shared<Room>();

    // 创建玩家
    auto p1 = make_shared<NormalPlayer>("玩家A", room.get());
    auto p2 = make_shared<NormalPlayer>("玩家B", room.get());
    auto p3 = make_shared<NormalPlayer>("玩家C", room.get());

    // 玩家发送消息
    cout << "\n1. 玩家A发送消息：" << endl;
    p1->sendMessage("大家好，我是A！");
    cout << endl;

    cout << "2. 玩家B发送消息：" << endl;
    p2->sendMessage("欢迎A加入房间！");
    cout << endl;

    cout << "3. 玩家C发送消息：" << endl;
    p3->sendMessage("我们一起组队吧！");
    cout << endl;

    return 0;
}
