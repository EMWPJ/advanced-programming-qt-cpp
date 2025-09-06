#include <iostream>
#include <memory>
#include <windows.h>
using namespace std;

// 抽象类：角色升级流程
class CharacterLevelUpProcess {
public:
    // 模板方法：规定升级流程
    void levelUp() {
        cout << "开始升级流程" << endl;
        fightMonsters();
        completeQuests();
        specialStep(); // 钩子方法，可选
        upgrade();
        cout << "升级流程结束" << endl;
    }
    virtual ~CharacterLevelUpProcess() = default;
protected:
    virtual void fightMonsters() = 0;
    virtual void completeQuests() = 0;
    virtual void upgrade() = 0;
    virtual void specialStep() {}
};

// 具体子类：战士升级
class WarriorLevelUp : public CharacterLevelUpProcess {
protected:
    void fightMonsters() override {
        cout << "战士疯狂刷怪，提升经验值..." << endl;
    }
    void completeQuests() override {
        cout << "战士完成主线与支线任务..." << endl;
    }
    void upgrade() override {
        cout << "战士升级！力量和生命值大幅提升！" << endl;
    }
    void specialStep() override {
        cout << "战士进行武器锻造，提升攻击力..." << endl;
    }
};

// 具体子类：法师升级
class MageLevelUp : public CharacterLevelUpProcess {
protected:
    void fightMonsters() override {
        cout << "法师远程施法击败怪物，积累经验..." << endl;
    }
    void completeQuests() override {
        cout << "法师完成魔法试炼任务..." << endl;
    }
    void upgrade() override {
        cout << "法师升级！魔法攻击和法力值大幅提升！" << endl;
    }
    void specialStep() override {
        cout << "法师学习新魔法技能..." << endl;
    }
};

// 具体子类：游侠升级
class RangerLevelUp : public CharacterLevelUpProcess {
protected:
    void fightMonsters() override {
        cout << "游侠利用弓箭远程猎杀怪物..." << endl;
    }
    void completeQuests() override {
        cout << "游侠完成探索与追踪任务..." << endl;
    }
    void upgrade() override {
        cout << "游侠升级！敏捷和暴击率提升！" << endl;
    }
    // 不重写specialStep，使用默认空实现
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "模板方法模式示例：角色升级流程" << endl;

    unique_ptr<CharacterLevelUpProcess> warrior = make_unique<WarriorLevelUp>();
    unique_ptr<CharacterLevelUpProcess> mage = make_unique<MageLevelUp>();
    unique_ptr<CharacterLevelUpProcess> ranger = make_unique<RangerLevelUp>();

    cout << "\n1. 战士升级流程" << endl;
    warrior->levelUp();

    cout << "\n2. 法师升级流程" << endl;
    mage->levelUp();

    cout << "\n3. 游侠升级流程" << endl;
    ranger->levelUp();

    cout << "\n模板方法模式保证了升级流程统一，细节可扩展" << endl;

    cin.get();
    return 0;
}
