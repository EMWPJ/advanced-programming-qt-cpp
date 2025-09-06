#include <iostream>
#include <vector>
#include <string>
// 组件：游戏对象基类
class GameObject {
public:
    GameObject(const std::string& name) : name(name) {}
    virtual void update() = 0;
    virtual void render() = 0;
    virtual ~GameObject() {}
protected:
    std::string name;
};
// 组件：玩家
class Player : public GameObject {
public:
    Player(const std::string& name) : GameObject(name) {}
    void update() override {
        std::cout << "玩家[" << name << "] 正在更新状态。" << std::endl;
    }
    void render() override {
        std::cout << "渲染玩家[" << name << "]。" << std::endl;
    }
};
// 组件：敌人
class Enemy : public GameObject {
public:
    Enemy(const std::string& name) : GameObject(name) {}
    void update() override {
        std::cout << "敌人[" << name << "] 正在巡逻。" << std::endl;
    }
    void render() override {
        std::cout << "渲染敌人[" << name << "]。" << std::endl;
    }
};
// 子系统：场景管理
class Scene {
public:
    void addObject(GameObject* obj) {
        objects.push_back(obj);
    }
    void update() {
        std::cout << "场景更新所有对象：" << std::endl;
        for (auto obj : objects) {
            obj->update();
        }
    }
    void render() {
        std::cout << "场景渲染所有对象：" << std::endl;
        for (auto obj : objects) {
            obj->render();
        }
    }
    ~Scene() {
        for (auto obj : objects) {
            delete obj;
        }
    }
private:
    std::vector<GameObject*> objects;
};
// 游戏引擎
class GameEngine {
public:
    void loadScene(Scene* scene) {
        this->scene = scene;
    }
    void run() {
        std::cout << "游戏引擎启动。" << std::endl;
        if (scene) {
            scene->update();
            scene->render();
        }
        std::cout << "游戏引擎关闭。" << std::endl;
    }
private:
    Scene* scene = nullptr;
};

int main() {
    Scene* scene = new Scene();// 创建场景
    scene->addObject(new Player("主角"));
    scene->addObject(new Enemy("怪物A"));
    scene->addObject(new Enemy("怪物B"));// 创建游戏引擎并加载场景
    GameEngine engine;
    engine.loadScene(scene);
    engine.run();// 运行游戏
    delete scene;// 释放资源
    std::cin.get();
    return 0;
}
