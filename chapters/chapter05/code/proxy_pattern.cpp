#include <iostream>
#include <memory>
#include <string>
#include <windows.h>

using namespace std;

// 抽象主题（接口）
class GameResource {
public:
    virtual ~GameResource() = default;
    virtual void load() = 0;
    virtual void render() = 0;
};

// 真实主题
class RealGameResource : public GameResource {
    string name;
    bool loaded = false;
public:
    RealGameResource(const string& n) : name(n) {}
    void load() override {
        if (!loaded) {
            cout << "[真实资源] 加载: " << name << endl;
            loaded = true;
        }
    }
    void render() override {
        if (!loaded) {
            cout << "[真实资源] 未加载，无法渲染: " << name << endl;
            return;
        }
        cout << "[真实资源] 渲染: " << name << endl;
    }
};

// 代理
class ResourceProxy : public GameResource {
    string name;
    unique_ptr<RealGameResource> realResource;
public:
    ResourceProxy(const string& n) : name(n) {
        cout << "[代理] 创建资源代理: " << name << endl;
    }
    void load() override {
        if (!realResource) {
            cout << "[代理] 延迟加载资源: " << name << endl;
            realResource = make_unique<RealGameResource>(name);
            realResource->load();
        }
    }
    void render() override {
        if (!realResource) load();
        realResource->render();
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 代理模式：资源延迟加载 ===" << endl;

    ResourceProxy tree("大树模型");
    ResourceProxy house("别墅模型");

    cout << "\n--- 渲染大树 ---" << endl;
    tree.render();

    cout << "\n--- 再次渲染大树 ---" << endl;
    tree.render();

    cout << "\n--- 渲染别墅 ---" << endl;
    house.render();

    cout << "\n--- 预加载别墅资源 ---" << endl;
    house.load();

    cout << "\n--- 再次渲染别墅 ---" << endl;
    house.render();

    cout << "\n演示完成！" << endl;

    cin.get();
    return 0;
}
