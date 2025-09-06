#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <windows.h>

using namespace std;

// 道具属性抽象类

class ItemAttribute {
public:
    virtual ~ItemAttribute() = default;
    virtual unique_ptr<ItemAttribute> clone() const = 0;
    virtual void show() const = 0;
};

// 具体属性：攻击力
class AttackAttribute : public ItemAttribute {
    int value;
public:
    AttackAttribute(int v) : value(v) {}
    unique_ptr<ItemAttribute> clone() const override {
        return make_unique<AttackAttribute>(*this);
    }
    void show() const override {
        cout << "攻击力: " << value << " ";
    }
};

// 具体属性：防御力
class DefenseAttribute : public ItemAttribute {
    int value;
public:
    DefenseAttribute(int v) : value(v) {}
    unique_ptr<ItemAttribute> clone() const override {
        return make_unique<DefenseAttribute>(*this);
    }
    void show() const override {
        cout << "防御力: " << value << " ";
    }
};

// 具体属性：恢复量
class HealAttribute : public ItemAttribute {
    int value;
public:
    HealAttribute(int v) : value(v) {}
    unique_ptr<ItemAttribute> clone() const override {
        return make_unique<HealAttribute>(*this);
    }
    void show() const override {
        cout << "恢复量: " << value << " ";
    }
};

// 道具效果抽象类
class ItemEffect {
public:
    virtual ~ItemEffect() = default;
    virtual unique_ptr<ItemEffect> clone() const = 0;
    virtual void show() const = 0;
};

// 具体效果：中毒
class PoisonEffect : public ItemEffect {
public:
    unique_ptr<ItemEffect> clone() const override {
        return make_unique<PoisonEffect>(*this);
    }
    void show() const override {
        cout << "效果: 中毒 ";
    }
};

// 具体效果：加速
class SpeedEffect : public ItemEffect {
public:
    unique_ptr<ItemEffect> clone() const override {
        return make_unique<SpeedEffect>(*this);
    }
    void show() const override {
        cout << "效果: 加速 ";
    }
};

// 具体效果：回血
class HealEffect : public ItemEffect {
public:
    unique_ptr<ItemEffect> clone() const override {
        return make_unique<HealEffect>(*this);
    }
    void show() const override {
        cout << "效果: 回血 ";
    }
};

// 道具抽象类
class Item {
public:
    virtual ~Item() = default;
    virtual unique_ptr<Item> clone() const = 0;
    virtual void showInfo() const = 0;
};

// 具体道具：武器
class Weapon : public Item {
    string name;
    vector<unique_ptr<ItemAttribute>> attributes;
    vector<unique_ptr<ItemEffect>> effects;
public:
    Weapon(const string& n) : name(n) {}
    Weapon(const Weapon& other) : name(other.name) {
        for (const auto& attr : other.attributes)
            attributes.push_back(attr->clone());
        for (const auto& eff : other.effects)
            effects.push_back(eff->clone());
    }
    void addAttribute(unique_ptr<ItemAttribute> attr) {
        attributes.push_back(move(attr));
    }
    void addEffect(unique_ptr<ItemEffect> eff) {
        effects.push_back(move(eff));
    }
    unique_ptr<Item> clone() const override {
        return make_unique<Weapon>(*this);
    }
    void showInfo() const override {
        cout << "武器: " << name << " | ";
        for (const auto& attr : attributes) attr->show();
        for (const auto& eff : effects) eff->show();
        cout << endl;
    }
    void setName(const string& n) { name = n; }
};

// 具体道具：防具
class Armor : public Item {
    string name;
    vector<unique_ptr<ItemAttribute>> attributes;
    vector<unique_ptr<ItemEffect>> effects;
public:
    Armor(const string& n) : name(n) {}
    Armor(const Armor& other) : name(other.name) {
        for (const auto& attr : other.attributes)
            attributes.push_back(attr->clone());
        for (const auto& eff : other.effects)
            effects.push_back(eff->clone());
    }
    void addAttribute(unique_ptr<ItemAttribute> attr) {
        attributes.push_back(move(attr));
    }
    void addEffect(unique_ptr<ItemEffect> eff) {
        effects.push_back(move(eff));
    }
    unique_ptr<Item> clone() const override {
        return make_unique<Armor>(*this);
    }
    void showInfo() const override {
        cout << "防具: " << name << " | ";
        for (const auto& attr : attributes) attr->show();
        for (const auto& eff : effects) eff->show();
        cout << endl;
    }
    void setName(const string& n) { name = n; }
};

// 具体道具：药水
class Potion : public Item {
    string name;
    vector<unique_ptr<ItemAttribute>> attributes;
    vector<unique_ptr<ItemEffect>> effects;
public:
    Potion(const string& n) : name(n) {}
    Potion(const Potion& other) : name(other.name) {
        for (const auto& attr : other.attributes)
            attributes.push_back(attr->clone());
        for (const auto& eff : other.effects)
            effects.push_back(eff->clone());
    }
    void addAttribute(unique_ptr<ItemAttribute> attr) {
        attributes.push_back(move(attr));
    }
    void addEffect(unique_ptr<ItemEffect> eff) {
        effects.push_back(move(eff));
    }
    unique_ptr<Item> clone() const override {
        return make_unique<Potion>(*this);
    }
    void showInfo() const override {
        cout << "药水: " << name << " | ";
        for (const auto& attr : attributes) attr->show();
        for (const auto& eff : effects) eff->show();
        cout << endl;
    }
    void setName(const string& n) { name = n; }
};

// 原型管理器
class ItemPrototypeManager {
    unordered_map<string, unique_ptr<Item>> prototypes;
public:
    void registerPrototype(const string& key, unique_ptr<Item> prototype) {
        prototypes[key] = move(prototype);
    }
    unique_ptr<Item> create(const string& key) {
        if (prototypes.count(key))
            return prototypes[key]->clone();
        return nullptr;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 原型模式示例：批量生成道具 ===" << endl;

    ItemPrototypeManager manager;

    // 1. 注册原型
    cout << "\n1. 注册原型:" << endl;
    auto sword = make_unique<Weapon>("铁剑");
    sword->addAttribute(make_unique<AttackAttribute>(15));
    sword->addEffect(make_unique<PoisonEffect>());
    manager.registerPrototype("sword", move(sword));

    auto shield = make_unique<Armor>("木盾");
    shield->addAttribute(make_unique<DefenseAttribute>(10));
    shield->addEffect(make_unique<SpeedEffect>());
    manager.registerPrototype("shield", move(shield));

    auto potion = make_unique<Potion>("小型治疗药水");
    potion->addAttribute(make_unique<HealAttribute>(30));
    potion->addEffect(make_unique<HealEffect>());
    manager.registerPrototype("potion", move(potion));

    // 2. 克隆原型
    cout << "\n2. 克隆原型:" << endl;

    // 克隆武器
    auto sword1 = manager.create("sword");
    if (sword1) {
        dynamic_cast<Weapon*>(sword1.get())->setName("铁剑+1");
        cout << "克隆武器1: ";
        sword1->showInfo();
    }
    auto sword2 = manager.create("sword");
    if (sword2) {
        dynamic_cast<Weapon*>(sword2.get())->setName("铁剑+2");
        cout << "克隆武器2: ";
        sword2->showInfo();
    }

    // 克隆防具
    auto shield1 = manager.create("shield");
    if (shield1) {
        dynamic_cast<Armor*>(shield1.get())->setName("木盾·强化");
        cout << "克隆防具: ";
        shield1->showInfo();
    }

    // 克隆药水
    auto potion1 = manager.create("potion");
    if (potion1) {
        dynamic_cast<Potion*>(potion1.get())->setName("大型治疗药水");
        cout << "克隆药水: ";
        potion1->showInfo();
    }

    cout << "\n3. 克隆完成" << endl;
    cout << "--------------------------------\n" << endl;
    cin.get();
    return 0;
}
