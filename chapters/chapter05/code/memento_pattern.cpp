#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <windows.h>
using namespace std;

// 备忘录（Memento）：负责存储玩家的角色属性、任务进度等内部状态数据
class Memento {
private:
    string state;
public:
    Memento(const string& s) : state(s) {}
    string getState() const { return state; }
};

// 发起者（Originator）：游戏角色本身，能够生成当前状态的备忘录，并可根据备忘录恢复到历史状态
class Originator {
private:
    string state;
public:
    void setState(const string& s) { state = s; }
    string getState() const { return state; }
    shared_ptr<Memento> saveStateToMemento() {
        return make_shared<Memento>(state);
    }
    void getStateFromMemento(shared_ptr<Memento> memento) {
        state = memento->getState();
    }
};

// 管理者（Caretaker）：负责保存和管理备忘录对象
class Caretaker {
private:
    vector<shared_ptr<Memento>> mementoList;
public:
    void add(shared_ptr<Memento> memento) {
        mementoList.push_back(memento);
    }
    shared_ptr<Memento> get(int index) {
        if (index >= 0 && index < mementoList.size())
            return mementoList[index];
        return nullptr;
    }
    int size() const { return mementoList.size(); }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 备忘录模式示例：保存与恢复游戏进度 ===" << endl;

    Originator player;
    Caretaker caretaker;

    // 初始状态
    player.setState("等级1，生命100，任务进度0%");
    cout << "当前状态: " << player.getState() << endl;
    caretaker.add(player.saveStateToMemento());

    // 升级
    player.setState("等级2，生命120，任务进度20%");
    cout << "当前状态: " << player.getState() << endl;
    caretaker.add(player.saveStateToMemento());

    // 受伤
    player.setState("等级2，生命60，任务进度20%");
    cout << "当前状态: " << player.getState() << endl;
    caretaker.add(player.saveStateToMemento());

    // 完成任务
    player.setState("等级3，生命80，任务进度100%");
    cout << "当前状态: " << player.getState() << endl;
    caretaker.add(player.saveStateToMemento());

    cout << "\n--- 恢复到之前的状态 ---" << endl;
    // 恢复到受伤前
    player.getStateFromMemento(caretaker.get(1));
    cout << "恢复到状态2: " << player.getState() << endl;

    // 恢复到初始状态
    player.getStateFromMemento(caretaker.get(0));
    cout << "恢复到状态1: " << player.getState() << endl;

    return 0;
}
