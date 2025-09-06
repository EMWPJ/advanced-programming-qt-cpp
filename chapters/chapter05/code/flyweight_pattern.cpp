#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <windows.h>
using namespace std;

// 享元接口：树
class Tree {
public:
    virtual ~Tree() = default;
    virtual void display(int x, int y, int height) const = 0;
    virtual string getType() const = 0;
};

// 具体享元：具体树种
class ConcreteTree : public Tree {
    string type;      // 树种类
    string texture;   // 树的贴图（内部状态，假设为字符串）
public:
    ConcreteTree(const string& t, const string& tex) : type(t), texture(tex) {}
    void display(int x, int y, int height) const override {
        cout << "在(" << x << "," << y << ")处显示[" << type << "]树，高度" << height
             << "，贴图资源：" << texture << endl;
    }
    string getType() const override { return type; }
};

// 享元工厂：管理和复用树种对象
class TreeFactory {
    unordered_map<string, shared_ptr<Tree>> treeMap;
public:
    shared_ptr<Tree> getTree(const string& type) {
        auto it = treeMap.find(type);
        if (it != treeMap.end()) {
            //cout << "重用已有树种：" << type << endl;
            return it->second;
        }
        // 假设贴图资源名与树种类一致
        auto tree = make_shared<ConcreteTree>(type, type + "_texture.png");
        treeMap[type] = tree;
        //cout << "创建新树种：" << type << endl;
        return tree;
    }
    size_t getTreeTypeCount() const { return treeMap.size(); }
    void listTreeTypes() const {
        cout << "享元池中的树种：";
        for (const auto& p : treeMap) cout << p.first << " ";
        cout << endl;
    }
};

// 每棵树的外部状态
struct TreeInfo {
    int x, y;
    int height;
    string type;
    TreeInfo(int px, int py, int h, const string& t) : x(px), y(py), height(h), type(t) {}
};

// 地图应用：管理所有树的外部状态
class Forest {
    vector<TreeInfo> trees;
    unique_ptr<TreeFactory> factory;
public:
    Forest() { factory = make_unique<TreeFactory>(); }
    void plantTree(int x, int y, int height, const string& type) {
        trees.emplace_back(x, y, height, type);
    }
    void display() {
        cout << "\n🌳 地图上树木分布如下：" << endl;
        for (const auto& t : trees) {
            auto tree = factory->getTree(t.type);
            tree->display(t.x, t.y, t.height);
        }
    }
    void showStatistics() {
        cout << "\n📊 享元模式统计：" << endl;
        cout << "总树木数：" << trees.size() << endl;
        cout << "树种享元对象数：" << factory->getTreeTypeCount() << endl;
        cout << "内存节省：" << trees.size() - factory->getTreeTypeCount() << " 个对象" << endl;
        cout << "节省比例：" << (1.0 - static_cast<double>(factory->getTreeTypeCount()) / trees.size()) * 100 << "%" << endl;
        factory->listTreeTypes();
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 享元模式示例：地图树木渲染 ===" << endl;

    Forest forest;
    // 假设地图上有成千上万棵树，这里只演示部分
    cout << "\n🌱 种植树木..." << endl;
    for (int i = 0; i < 1000; i++) {
        forest.plantTree(rand() % 100, rand() % 100, rand() % 100, "松树");
    }
    for (int i = 0; i < 1000; i++) {
        forest.plantTree(rand() % 100, rand() % 100, rand() % 100, "杨树");
    }
    for (int i = 0; i < 1000; i++) {
        forest.plantTree(rand() % 100, rand() % 100, rand() % 100, "枫树");
    }
    for (int i = 0; i < 1000; i++) {
        forest.plantTree(rand() % 100, rand() % 100, rand() % 100, "桃树");
    }
    // 显示所有树
    forest.display();
    // 显示享元统计信息
    forest.showStatistics();

    cin.get();
    return 0;
}
