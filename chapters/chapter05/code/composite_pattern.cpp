#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>
using namespace std;
// 组件基类
class Component {
public:
    virtual ~Component() = default;
    virtual void display(int depth = 0) const = 0;
    virtual void add(shared_ptr<Component> c) {}
};
// 文件（叶子节点）
class File : public Component {
    string name;
    int size;
public:
    File(const string& n, int s) : name(n), size(s) {}
    void display(int depth = 0) const override {
        cout << string(depth * 2, ' ') << "📄 " << name << " (" << size << " bytes)" << endl;
    }
};
// 文件夹（组合节点）
class Folder : public Component {
    string name;
    vector<shared_ptr<Component>> children;
public:
    Folder(const string& n) : name(n) {}
    void add(shared_ptr<Component> c) override {
        children.push_back(c);
    }
    void display(int depth = 0) const override {
        cout << string(depth * 2, ' ') << "📁 " << name << "/" << endl;
        for (const auto& c : children) c->display(depth + 1);
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // 构建一个复杂的树形结构
    auto root = make_shared<Folder>("根目录");
    auto docs = make_shared<Folder>("文档");
    auto codes = make_shared<Folder>("代码");

    // 文档文件夹下有两个文件和一个子文件夹
    docs->add(make_shared<File>("说明.txt", 120));
    docs->add(make_shared<File>("计划.docx", 300));
    auto reports = make_shared<Folder>("报告");
    reports->add(make_shared<File>("2023年总结.pdf", 500));
    docs->add(reports);

    // 代码文件夹下有一个文件和一个子文件夹
    codes->add(make_shared<File>("main.cpp", 80));
    auto utils = make_shared<Folder>("工具");
    utils->add(make_shared<File>("util.h", 20));
    utils->add(make_shared<File>("util.cpp", 60));
    codes->add(utils);

    // 根目录下添加三个文件夹
    root->add(docs);
    root->add(codes);

    // 展示树形结构
    root->display();

    cin.get();
    return 0;
}
