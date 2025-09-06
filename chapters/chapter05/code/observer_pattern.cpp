#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <windows.h>

using namespace std;

// 观察者接口
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const string& taskInfo) = 0;
    virtual string getName() const = 0;
};

// 主题接口（被观察者）
class Subject {
public:
    virtual ~Subject() = default;
    virtual void addObserver(shared_ptr<Observer> observer) = 0;
    virtual void removeObserver(shared_ptr<Observer> observer) = 0;
    virtual void notifyObservers() = 0;
};

// 具体主题：任务进度
class TaskProgress : public Subject {
private:
    vector<shared_ptr<Observer>> observers;
    string progressInfo;
public:
    void addObserver(shared_ptr<Observer> observer) override {
        observers.push_back(observer);
        cout << "已添加观察者: " << observer->getName() << endl;
    }

    void removeObserver(shared_ptr<Observer> observer) override {
        auto it = find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            cout << "已移除观察者: " << observer->getName() << endl;
            observers.erase(it);
        }
    }

    void notifyObservers() override {
        cout << "\n[系统] 通知所有玩家任务进度更新: " << progressInfo << endl;
        for (auto& obs : observers) {
            obs->update(progressInfo);
        }
    }

    void setProgress(const string& info) {
        progressInfo = info;
        notifyObservers();
    }
};

// 具体观察者：玩家
class Player : public Observer, public enable_shared_from_this<Player> {
private:
    string name;
    string lastTaskInfo;
    TaskProgress* subject;
public:
    Player(const string& n, TaskProgress* s) : name(n), subject(s) {}

    void join() {
        subject->addObserver(shared_from_this());
    }

    void quit() {
        subject->removeObserver(shared_from_this());
    }

    void update(const string& taskInfo) override {
        lastTaskInfo = taskInfo;
        cout << "玩家 [" << name << "] 收到任务进度通知: " << lastTaskInfo << endl;
    }

    string getName() const override {
        return name;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 观察者模式示例：多人游戏任务进度通知 ===" << endl;

    // 创建任务进度主题
    auto taskProgress = make_unique<TaskProgress>();

    // 创建玩家（观察者）
    auto player1 = make_shared<Player>("小明", taskProgress.get());
    auto player2 = make_shared<Player>("小红", taskProgress.get());
    auto player3 = make_shared<Player>("小刚", taskProgress.get());

    // 玩家加入观察
    player1->join();
    player2->join();
    player3->join();

    // 第一次任务进度更新
    cout << "\n--- 任务进度更新1 ---" << endl;
    taskProgress->setProgress("击败10只怪物（3/10）");

    // 第二次任务进度更新
    cout << "\n--- 任务进度更新2 ---" << endl;
    taskProgress->setProgress("击败10只怪物（7/10）");

    // 小红退出观察
    cout << "\n--- 小红退出队伍 ---" << endl;
    player2->quit();

    // 第三次任务进度更新
    cout << "\n--- 任务进度更新3 ---" << endl;
    taskProgress->setProgress("击败10只怪物（10/10），任务完成！");

    cout << "\n✅ 演示结束，观察者模式实现了任务进度的实时通知！" << endl;
    cin.get();
    return 0;
}
