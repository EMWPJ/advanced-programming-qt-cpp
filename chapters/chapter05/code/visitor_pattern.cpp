#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <windows.h>
using namespace std;

// 抽象访问者
class GameCharacter;
class Warrior;
class Mage;
class Ranger;

class Visitor {
public:
    virtual void visitWarrior(Warrior* w) = 0;
    virtual void visitMage(Mage* m) = 0;
    virtual void visitRanger(Ranger* r) = 0;
    virtual ~Visitor() = default;
};

// 抽象元素
class GameCharacter {
public:
    virtual void accept(Visitor* visitor) = 0;
    virtual string getName() const = 0;
    virtual ~GameCharacter() = default;
};

// 具体元素：战士
class Warrior : public GameCharacter {
    string name;
    int hp;
    int attack;
public:
    Warrior(const string& n, int h, int a) : name(n), hp(h), attack(a) {}
    void accept(Visitor* visitor) override {
        visitor->visitWarrior(this);
    }
    string getName() const override { return name; }
    int getHP() const { return hp; }
    int getAttack() const { return attack; }
};

// 具体元素：法师
class Mage : public GameCharacter {
    string name;
    int mp;
    int magicAttack;
public:
    Mage(const string& n, int m, int ma) : name(n), mp(m), magicAttack(ma) {}
    void accept(Visitor* visitor) override {
        visitor->visitMage(this);
    }
    string getName() const override { return name; }
    int getMP() const { return mp; }
    int getMagicAttack() const { return magicAttack; }
};

// 具体元素：游侠
class Ranger : public GameCharacter {
    string name;
    int agility;
    int crit;
public:
    Ranger(const string& n, int ag, int cr) : name(n), agility(ag), crit(cr) {}
    void accept(Visitor* visitor) override {
        visitor->visitRanger(this);
    }
    string getName() const override { return name; }
    int getAgility() const { return agility; }
    int getCrit() const { return crit; }
};

// 对象结构：队伍
class Party {
    vector<shared_ptr<GameCharacter>> members;
public:
    void addMember(shared_ptr<GameCharacter> c) {
        members.push_back(c);
    }
    void accept(Visitor* visitor) {
        for (auto& m : members) {
            m->accept(visitor);
        }
    }
};

// 具体访问者：显示状态
class StatusVisitor : public Visitor {
public:
    void visitWarrior(Warrior* w) override {
        cout << "战士[" << w->getName() << "] HP: " << w->getHP() << " 攻击: " << w->getAttack() << endl;
    }
    void visitMage(Mage* m) override {
        cout << "法师[" << m->getName() << "] MP: " << m->getMP() << " 魔法攻击: " << m->getMagicAttack() << endl;
    }
    void visitRanger(Ranger* r) override {
        cout << "游侠[" << r->getName() << "] 敏捷: " << r->getAgility() << " 暴击: " << r->getCrit() << endl;
    }
};

// 具体访问者：计算战斗力
class PowerVisitor : public Visitor {
public:
    void visitWarrior(Warrior* w) override {
        int power = w->getHP() + w->getAttack() * 2;
        cout << "战士[" << w->getName() << "] 战斗力: " << power << endl;
    }
    void visitMage(Mage* m) override {
        int power = m->getMP() + m->getMagicAttack() * 3;
        cout << "法师[" << m->getName() << "] 战斗力: " << power << endl;
    }
    void visitRanger(Ranger* r) override {
        int power = r->getAgility() * 2 + r->getCrit() * 4;
        cout << "游侠[" << r->getName() << "] 战斗力: " << power << endl;
    }
};

// 具体访问者：发放奖励
class RewardVisitor : public Visitor {
public:
    void visitWarrior(Warrior* w) override {
        cout << "战士[" << w->getName() << "] 获得奖励：强化武器" << endl;
    }
    void visitMage(Mage* m) override {
        cout << "法师[" << m->getName() << "] 获得奖励：新魔法书" << endl;
    }
    void visitRanger(Ranger* r) override {
        cout << "游侠[" << r->getName() << "] 获得奖励：高级箭矢" << endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "访问者模式示例：RPG角色队伍" << endl;

    Party party;
    party.addMember(make_shared<Warrior>("亚瑟", 150, 40));
    party.addMember(make_shared<Mage>("梅林", 120, 60));
    party.addMember(make_shared<Ranger>("罗宾", 100, 30));

    cout << "\n1. 显示角色状态：" << endl;
    StatusVisitor statusVisitor;
    party.accept(&statusVisitor);

    cout << "\n2. 计算战斗力：" << endl;
    PowerVisitor powerVisitor;
    party.accept(&powerVisitor);

    cout << "\n3. 发放奖励：" << endl;
    RewardVisitor rewardVisitor;
    party.accept(&rewardVisitor);

    cout << "\n 访问者模式展示完毕" << endl;

    cin.get();
    return 0;
}
