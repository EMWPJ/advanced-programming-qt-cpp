#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

// 角色属性抽象类
class RoleAttribute {
public:
    virtual ~RoleAttribute() = default;
    virtual void show() const = 0;
};

// 具体属性
class HPAttribute : public RoleAttribute {
public:
    void show() const override {
        cout << "生命值: 100" << endl;
    }
};
class AttackAttribute : public RoleAttribute {
public:
    void show() const override {
        cout << "攻击力: 30" << endl;
    }
};
class DefenseAttribute : public RoleAttribute {
public:
    void show() const override {
        cout << "防御力: 20" << endl;
    }
};

// 角色技能抽象类
class RoleSkill {
public:
    virtual ~RoleSkill() = default;
    virtual void use() const = 0;
};

// 具体技能
class ChargeSkill : public RoleSkill {
public:
    void use() const override {
        cout << "技能: 冲锋" << endl;
    }
};
class FireballSkill : public RoleSkill {
public:
    void use() const override {
        cout << "技能: 火球术" << endl;
    }
};
class StealthSkill : public RoleSkill {
public:
    void use() const override {
        cout << "技能: 隐身术" << endl;
    }
};

// 角色产品抽象类
class Role {
public:
    virtual ~Role() = default;
    virtual void attack() const = 0;
    virtual void defend() const = 0;
    virtual void useSkill() const = 0;
    virtual void show() const = 0;
    virtual void addAttribute(unique_ptr<RoleAttribute> attr) = 0;
    virtual void addSkill(unique_ptr<RoleSkill> skill) = 0;
protected:
    vector<unique_ptr<RoleAttribute>> attributes;
    vector<unique_ptr<RoleSkill>> skills;
    void showAttributes() const {
        for (const auto& attr : attributes) attr->show();
    }
    void showSkills() const {
        for (const auto& skill : skills) skill->use();
    }
};

// 具体角色
class Warrior : public Role {
public:
    void addAttribute(unique_ptr<RoleAttribute> attr) override {
        attributes.push_back(move(attr));
    }
    void addSkill(unique_ptr<RoleSkill> skill) override {
        skills.push_back(move(skill));
    }
    void attack() const override { cout << "战士攻击" << endl; }
    void defend() const override { cout << "战士防御" << endl; }
    void useSkill() const override { cout << "战士技能" << endl;
        showSkills(); }
    void show() const override { cout << "角色: 战士" << endl;
        showAttributes(); }
};

class Mage : public Role {
public:
    void addAttribute(unique_ptr<RoleAttribute> attr) override {
        attributes.push_back(move(attr));
    }
    void addSkill(unique_ptr<RoleSkill> skill) override {
        skills.push_back(move(skill));
    }
    void attack() const override { cout << "法师攻击" << endl; }
    void defend() const override { cout << "法师防御" << endl; }
    void useSkill() const override { cout << "法师技能" << endl;
        showSkills(); }
    void show() const override { cout << "角色: 法师" << endl;
        showAttributes(); }
};

class Assassin : public Role {
public:
    void addAttribute(unique_ptr<RoleAttribute> attr) override {
        attributes.push_back(move(attr));
    }
    void addSkill(unique_ptr<RoleSkill> skill) override {
        skills.push_back(move(skill));
    }
    void attack() const override { cout << "刺客攻击" << endl; }
    void defend() const override { cout << "刺客防御" << endl; }
    void useSkill() const override { cout << "刺客技能" << endl;
        showSkills(); }
    void show() const override { cout << "角色: 刺客" << endl;
        showAttributes(); }
};

// 抽象建造者
class RoleBuilder {
public:
    virtual ~RoleBuilder() = default;
    virtual void createRole() = 0;
    virtual void createRoleAttribute() = 0;
    virtual void createRoleSkill() = 0;
    virtual unique_ptr<Role> getResult() = 0;
protected:
    unique_ptr<Role> role;
};

// 具体建造者 - 战士
class WarriorBuilder : public RoleBuilder {
public:
    void createRole() override {
        role = make_unique<Warrior>();
    }
    void createRoleAttribute() override {
        role->addAttribute(make_unique<HPAttribute>());
        role->addAttribute(make_unique<AttackAttribute>());
        role->addAttribute(make_unique<DefenseAttribute>());
    }
    void createRoleSkill() override {
        role->addSkill(make_unique<ChargeSkill>());
    }
    unique_ptr<Role> getResult() override {
        return move(role);
    }
};

// 具体建造者 - 法师
class MageBuilder : public RoleBuilder {
public:
    void createRole() override {
        role = make_unique<Mage>();
    }
    void createRoleAttribute() override {
        role->addAttribute(make_unique<HPAttribute>());
        role->addAttribute(make_unique<AttackAttribute>());
    }
    void createRoleSkill() override {
        role->addSkill(make_unique<FireballSkill>());
    }
    unique_ptr<Role> getResult() override {
        return move(role);
    }
};

// 具体建造者 - 刺客
class AssassinBuilder : public RoleBuilder {
public:
    void createRole() override {
        role = make_unique<Assassin>();
    }
    void createRoleAttribute() override {
        role->addAttribute(make_unique<HPAttribute>());
        role->addAttribute(make_unique<AttackAttribute>());
        role->addAttribute(make_unique<DefenseAttribute>());
    }
    void createRoleSkill() override {
        role->addSkill(make_unique<StealthSkill>());
    }
    unique_ptr<Role> getResult() override {
        return move(role);
    }
};

// 指挥者
class RoleDirector {
public:
    void construct(RoleBuilder& builder) {
        builder.createRole();
        builder.createRoleAttribute();
        builder.createRoleSkill();
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== 建造者模式演示（游戏角色） ===" << endl;
    RoleDirector director;
    // 构建战士
    WarriorBuilder warriorBuilder;
    director.construct(warriorBuilder);
    auto warrior = warriorBuilder.getResult();
    cout << "\n构建的角色:" << endl;
    warrior->show();
    // 构建法师
    MageBuilder mageBuilder;
    director.construct(mageBuilder);
    auto mage = mageBuilder.getResult();
    cout << "\n构建的角色:" << endl;
    mage->show();
    // 构建刺客
    AssassinBuilder assassinBuilder;
    director.construct(assassinBuilder);
    auto assassin = assassinBuilder.getResult();
    cout << "\n构建的角色:" << endl;
    assassin->show();
    cin.get();
    return 0;
}