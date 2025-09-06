#include <iostream>
#include <memory>
#include <string>
#include <windows.h>
using namespace std;

// 抽象状态
class State {
public:
    virtual ~State() = default;
    virtual void handle() = 0;
    virtual string getName() const = 0;
};

// 具体状态：正常
class NormalState : public State {
public:
    void handle() override {
        cout << "角色处于【正常】状态，可以自由行动。" << endl;
    }
    string getName() const override {
        return "正常";
    }
};

// 具体状态：中毒
class PoisonedState : public State {
public:
    void handle() override {
        cout << "角色处于【中毒】状态，每回合损失生命值，行动受限！" << endl;
    }
    string getName() const override {
        return "中毒";
    }
};

// 具体状态：眩晕
class StunnedState : public State {
public:
    void handle() override {
        cout << "角色处于【眩晕】状态，无法行动！" << endl;
    }
    string getName() const override {
        return "眩晕";
    }
};

// 上下文：角色
class CharacterContext {
private:
    unique_ptr<State> state;
public:
    CharacterContext() : state(nullptr) {}

    void setState(unique_ptr<State> newState) {
        if (state) {
            cout << "角色状态由【" << state->getName() << "】切换为【" << newState->getName() << "】" << endl;
        } else {
            cout << "角色状态初始化为【" << newState->getName() << "】" << endl;
        }
        state = move(newState);
    }

    void action() const {
        if (state) {
            state->handle();
        } else {
            cout << "角色无状态，无法行动。" << endl;
        }
    }

    string getStateName() const {
        return state ? state->getName() : "无状态";
    }
};

int main() {
    // 设置控制台编码为UTF-8，解决中文显示问题
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "状态模式示例：RPG角色状态切换" << endl;
    CharacterContext hero;
    cout << "初始状态: " << hero.getStateName() << endl;
    // 设置为正常状态
    hero.setState(make_unique<NormalState>());
    cout << "当前状态: " << hero.getStateName() << endl;
    hero.action();
    // 切换为中毒状态
    hero.setState(make_unique<PoisonedState>());
    cout << "当前状态: " << hero.getStateName() << endl;
    hero.action();
    // 切换为眩晕状态
    hero.setState(make_unique<StunnedState>());
    cout << "当前状态: " << hero.getStateName() << endl;
    hero.action();
    // 再切回正常状态
    hero.setState(make_unique<NormalState>());
    cout << "当前状态: " << hero.getStateName() << endl;
    hero.action();
    cout << "状态模式演示完毕。" << endl;
    return 0;
}