#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>
using namespace std;

// 抽象命令
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// 接收者：玩家
class Player {
private:
    string name;
    int x, y;
public:
    Player(const string& n) : name(n), x(0), y(0) {}

    void move(int dx, int dy) {
        x += dx;
        y += dy;
        cout << name << " 移动到位置 (" << x << ", " << y << ")" << endl;
    }

    void attack(const string& target) {
        cout << name << " 攻击了 " << target << "！" << endl;
    }

    void useSkill(const string& skill) {
        cout << name << " 使用了技能：" << skill << endl;
    }

    void showPosition() {
        cout << name << " 当前坐标: (" << x << ", " << y << ")" << endl;
    }
};

// 具体命令：移动
class MoveCommand : public Command {
private:
    Player* player;
    int dx, dy;
    int prevX, prevY;
public:
    MoveCommand(Player* p, int dx, int dy) : player(p), dx(dx), dy(dy), prevX(0), prevY(0) {}

    void execute() override {
        // 记录上一次位置
        player->showPosition();
        prevX = dx;
        prevY = dy;
        player->move(dx, dy);
    }

    void undo() override {
        player->move(-dx, -dy);
        cout << "撤销移动操作" << endl;
    }
};

// 具体命令：攻击
class AttackCommand : public Command {
private:
    Player* player;
    string target;
public:
    AttackCommand(Player* p, const string& t) : player(p), target(t) {}

    void execute() override {
        player->attack(target);
    }

    void undo() override {
        cout << "撤销攻击操作（无法真正撤销，仅做演示）" << endl;
    }
};

// 具体命令：使用技能
class SkillCommand : public Command {
private:
    Player* player;
    string skill;
public:
    SkillCommand(Player* p, const string& s) : player(p), skill(s) {}

    void execute() override {
        player->useSkill(skill);
    }

    void undo() override {
        cout << "撤销技能操作（无法真正撤销，仅做演示）" << endl;
    }
};

// 宏命令：组合多个命令
class MacroCommand : public Command {
private:
    vector<shared_ptr<Command>> commands;
public:
    void addCommand(shared_ptr<Command> cmd) {
        commands.push_back(cmd);
    }

    void execute() override {
        for (auto& cmd : commands) {
            cmd->execute();
        }
    }

    void undo() override {
        // 撤销顺序与执行相反
        for (auto it = commands.rbegin(); it != commands.rend(); ++it) {
            (*it)->undo();
        }
    }
};

// 空命令
class NoCommand : public Command {
public:
    void execute() override {}
    void undo() override {}
};

// 请求者：操作记录与撤销
class Invoker {
private:
    vector<shared_ptr<Command>> history;
public:
    void executeCommand(shared_ptr<Command> cmd) {
        cmd->execute();
        history.push_back(cmd);
    }

    void undo() {
        if (!history.empty()) {
            history.back()->undo();
            history.pop_back();
        } else {
            cout << "没有可撤销的操作" << endl;
        }
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 命令模式RPG示例 ===" << endl;

    // 创建玩家
    Player player("勇者");

    // 创建命令
    auto move1 = make_shared<MoveCommand>(&player, 2, 3);
    auto attack1 = make_shared<AttackCommand>(&player, "史莱姆");
    auto skill1 = make_shared<SkillCommand>(&player, "火球术");

    // 宏命令：一键连招
    auto macro = make_shared<MacroCommand>();
    macro->addCommand(move1);
    macro->addCommand(attack1);
    macro->addCommand(skill1);

    // 请求者
    Invoker invoker;
    cout << "\n1. 执行移动命令：" << endl;
    invoker.executeCommand(move1);
    cout << "\n2. 执行攻击命令：" << endl;
    invoker.executeCommand(attack1);
    cout << "\n3. 执行技能命令：" << endl;
    invoker.executeCommand(skill1);
    cout << "\n4. 执行宏命令（连招）：" << endl;
    invoker.executeCommand(macro);
    cout << "\n5. 撤销上一次操作（宏命令）：" << endl;
    invoker.undo();
    cout << "\n6. 撤销上一次操作（技能）：" << endl;
    invoker.undo();
    cout << "\n7. 撤销上一次操作（攻击）：" << endl;
    invoker.undo();
    cout << "\n8. 撤销上一次操作（移动）：" << endl;
    invoker.undo();
    cout << "\n9. 再次撤销（无操作）：" << endl;
    invoker.undo();

    cin.get();
    return 0;
}
