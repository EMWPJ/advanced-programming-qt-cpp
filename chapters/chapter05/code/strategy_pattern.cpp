#include <iostream>
#include <memory>
#include <string>
#include <windows.h>
using namespace std;

// 攻击策略接口
class AttackStrategy {
public:
    virtual ~AttackStrategy() = default;
    virtual void attack() const = 0;
    virtual string getName() const = 0;
};

// 具体策略：近战攻击
class MeleeAttack : public AttackStrategy {
public:
    void attack() const override {
        cout << "使用近战武器进行攻击！" << endl;
    }
    string getName() const override {
        return "近战攻击";
    }
};

// 具体策略：远程攻击
class RangedAttack : public AttackStrategy {
public:
    void attack() const override {
        cout << "使用远程武器进行攻击！" << endl;
    }
    string getName() const override {
        return "远程攻击";
    }
};

// 具体策略：魔法攻击
class MagicAttack : public AttackStrategy {
public:
    void attack() const override {
        cout << "释放魔法进行攻击！" << endl;
    }
    string getName() const override {
        return "魔法攻击";
    }
};

// 角色类，持有攻击策略
class Character {
private:
    string name;
    unique_ptr<AttackStrategy> strategy;
public:
    Character(const string& n) : name(n) {}

    void setStrategy(unique_ptr<AttackStrategy> s) {
        strategy = move(s);
        cout << name << " 切换为[" << strategy->getName() << "]策略。" << endl;
    }
    void attack() const {
        if (strategy) {
            cout << name << "：";
            strategy->attack();
        } else {
            cout << name << " 还没有选择攻击方式！" << endl;
        }
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 策略模式示例：角色攻击方式灵活切换 ===" << endl;

    Character hero("勇者");

    // 切换为近战攻击
    hero.setStrategy(make_unique<MeleeAttack>());
    hero.attack();

    // 切换为远程攻击
    hero.setStrategy(make_unique<RangedAttack>());
    hero.attack();

    // 切换为魔法攻击
    hero.setStrategy(make_unique<MagicAttack>());
    hero.attack();

    cout << "策略模式让角色可以灵活切换攻击方式！" << endl;

    cin.get();
    return 0;
}
