#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

// 技能请求类
class SkillRequest {
private:
    string description;
    int power; // 技能威力
public:
    SkillRequest(const string& desc, int p) : description(desc), power(p) {}
    const string& getDescription() const { return description; }
    int getPower() const { return power; }
    void setPower(int p) { power = p; }
};

// 抽象处理者
class SkillHandler {
protected:
    shared_ptr<SkillHandler> next;
    string name;

public:
    SkillHandler(const string& n) : name(n), next(nullptr) {}

    void setNext(shared_ptr<SkillHandler> handler) {
        next = handler;
    }

    virtual void handle(shared_ptr<SkillRequest> request) = 0;

    virtual ~SkillHandler() = default;
};

// 具体处理者 - 装备加成
class EquipmentHandler : public SkillHandler {
public:
    EquipmentHandler(const string& n) : SkillHandler(n) {}

    void handle(shared_ptr<SkillRequest> request) override {
        cout << name << " 装备加成：技能威力提升20%" << endl;
        int newPower = static_cast<int>(request->getPower() * 1.2);
        request->setPower(newPower);
        if (next) next->handle(request);
    }
};

// 具体处理者 - Buff增益
class BuffHandler : public SkillHandler {
public:
    BuffHandler(const string& n) : SkillHandler(n) {}

    void handle(shared_ptr<SkillRequest> request) override {
        cout << name << " Buff增益：技能威力提升30%" << endl;
        int newPower = static_cast<int>(request->getPower() * 1.3);
        request->setPower(newPower);
        if (next) next->handle(request);
    }
};

// 具体处理者 - 护盾吸收
class ShieldHandler : public SkillHandler {
public:
    ShieldHandler(const string& n) : SkillHandler(n) {}

    void handle(shared_ptr<SkillRequest> request) override {
        cout << name << " 护盾吸收：技能威力减少50点" << endl;
        int newPower = request->getPower() - 50;
        if (newPower < 0) newPower = 0;
        request->setPower(newPower);
        if (next) next->handle(request);
    }
};

// 具体处理者 - 最终结算
class FinalHandler : public SkillHandler {
public:
    FinalHandler(const string& n) : SkillHandler(n) {}

    void handle(shared_ptr<SkillRequest> request) override {
        cout << name << " 最终结算：技能 \"" << request->getDescription()
             << "\" 最终威力为 " << request->getPower() << endl;
    }
};

int main() {
    // 设置控制台编码为UTF-8，解决中文显示问题
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 责任链模式示例：游戏技能触发链 ===" << endl;

    // 创建处理者
    auto equipment = make_shared<EquipmentHandler>("装备系统");
    auto buff = make_shared<BuffHandler>("Buff系统");
    auto shield = make_shared<ShieldHandler>("护盾系统");
    auto final = make_shared<FinalHandler>("结算系统");

    // 构建责任链
    equipment->setNext(buff);
    buff->setNext(shield);
    shield->setNext(final);

    // 创建技能请求
    auto skill1 = make_shared<SkillRequest>("火球术", 100);
    auto skill2 = make_shared<SkillRequest>("雷霆一击", 200);

    cout << "\n1. 释放技能：火球术" << endl;
    equipment->handle(skill1);

    cout << "\n2. 释放技能：雷霆一击" << endl;
    equipment->handle(skill2);

    cin.get();
    return 0;
}
