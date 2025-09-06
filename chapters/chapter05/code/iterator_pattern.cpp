#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <windows.h>
using namespace std;

// 抽象迭代器
class Iterator {
public:
    virtual bool hasNext() const = 0;
    virtual const string& next() = 0;
    virtual ~Iterator() = default;
};

// 抽象聚合
class Aggregate {
public:
    virtual shared_ptr<Iterator> createIterator() const = 0;
    virtual shared_ptr<Iterator> createReverseIterator() const = 0;
    virtual void addItem(const string& item) = 0;
    virtual ~Aggregate() = default;
};

// 具体聚合
class ConcreteAggregate : public Aggregate {
private:
    vector<string> items;
    // 正向迭代器
    class ConcreteIterator : public Iterator {
        const vector<string>& items;
        size_t index;
    public:
    ConcreteIterator(const vector<string>& items) : items(items), index(0) {}
    bool hasNext() const override { return index < items.size(); }
    const string& next() override {
    if (!hasNext()) throw out_of_range("没有更多元素");
            return items[index++];
        }
    };
    // 反向迭代器
    class ReverseIterator : public Iterator {
        const vector<string>& items;
        int index;
    public:
    ReverseIterator(const vector<string>& items) : items(items), index(static_cast<int>(items.size()) - 1) {}
    bool hasNext() const override { return index >= 0; }
    const string& next() override {
    if (!hasNext()) throw out_of_range("没有更多元素");
            return items[index--];
        }
    };
public:
    void addItem(const string& item) override { items.push_back(item); }
    shared_ptr<Iterator> createIterator() const override {
        return make_shared<ConcreteIterator>(items);
    }
    shared_ptr<Iterator> createReverseIterator() const override {
        return make_shared<ReverseIterator>(items);
    }
};

void printItems(const string& title, shared_ptr<Iterator> it) {
    cout << "\n" << title << endl;
    while (it->hasNext()) {
        cout << "  " << it->next() << endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 迭代器模式示例：遍历游戏物品 ===" << endl;
    ConcreteAggregate aggregate;
    aggregate.addItem("第一个元素");
    aggregate.addItem("第二个元素");
    aggregate.addItem("第三个元素");
    aggregate.addItem("第四个元素");
    aggregate.addItem("第五个元素");
    printItems("1. 正向遍历：", aggregate.createIterator());
    printItems("2. 反向遍历：", aggregate.createReverseIterator());

    cin.get();
    return 0;
}
