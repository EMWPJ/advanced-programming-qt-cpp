#include <iostream>
#include <queue>
#include <string>
#include <mutex>
#include <windows.h>
using namespace std;
// 饿汉式单例
class PrinterManagerEager {
private:
    queue<string> printQueue;
    mutex mtx;
    PrinterManagerEager() { cout << "饿汉式打印机实例已创建" << endl; }
    PrinterManagerEager(const PrinterManagerEager&) = delete;
    PrinterManagerEager& operator=(const PrinterManagerEager&) = delete;
    static PrinterManagerEager instance; // 静态成员，类加载时创建
public:
    static PrinterManagerEager& getInstance() { return instance; }
    void addJob(const string& job) {
        lock_guard<mutex> lock(mtx);
        printQueue.push(job);
        cout << "[饿汉式] 已添加打印任务: " << job << endl;
    }
    void processJob() {
        lock_guard<mutex> lock(mtx);
        if (!printQueue.empty()) {
            cout << "[饿汉式] 正在打印: " << printQueue.front() << endl;
            printQueue.pop();
        } else {
            cout << "[饿汉式] 当前无待打印任务" << endl;
        }
    }
    int jobCount() {
        lock_guard<mutex> lock(mtx);
        return static_cast<int>(printQueue.size());
    }
};
// 静态成员初始化
PrinterManagerEager PrinterManagerEager::instance;

// 懒汉式单例
class PrinterManagerLazy {
private:
    queue<string> printQueue; mutex mtx;
    static PrinterManagerLazy* instance;
    static mutex instanceMtx;
    PrinterManagerLazy() { cout << "懒汉式打印机实例已创建" << endl; }
    PrinterManagerLazy(const PrinterManagerLazy&) = delete;
    PrinterManagerLazy& operator=(const PrinterManagerLazy&) = delete;
public:
    static PrinterManagerLazy* getInstance() {
        if (instance == nullptr) {
            lock_guard<mutex> lock(instanceMtx);
            if (instance == nullptr) { instance = new PrinterManagerLazy(); }
        }
        return instance;
    }
    void addJob(const string& job) {
        lock_guard<mutex> lock(mtx);
        printQueue.push(job);
        cout << "[懒汉式] 已添加打印任务: " << job << endl;
    }
    void processJob() {
        lock_guard<mutex> lock(mtx);
        if (!printQueue.empty()) {
            cout << "[懒汉式] 正在打印: " << printQueue.front() << endl;
            printQueue.pop();
        } else {
            cout << "[懒汉式] 当前无待打印任务" << endl;
        }
    }
    int jobCount() {
        lock_guard<mutex> lock(mtx);
        return static_cast<int>(printQueue.size());
    }
};
// 静态成员初始化
PrinterManagerLazy* PrinterManagerLazy::instance = nullptr;
mutex PrinterManagerLazy::instanceMtx;

// 静态局部变量单例
class PrinterManagerStatic {
private:
    queue<string> printQueue; mutex mtx;
    PrinterManagerStatic() {
        cout << "静态局部变量式打印机实例已创建" << endl;
    }
    PrinterManagerStatic(const PrinterManagerStatic&) = delete;
    PrinterManagerStatic& operator=(const PrinterManagerStatic&) = delete;
public:
    static PrinterManagerStatic& getInstance() {
        static PrinterManagerStatic instance;
        return instance;
    }
    void addJob(const string& job) {
        lock_guard<mutex> lock(mtx);
        printQueue.push(job);
        cout << "[静态局部变量式] 已添加打印任务: " << job << endl;
    }
    void processJob() {
        lock_guard<mutex> lock(mtx);
        if (!printQueue.empty()) {
            cout << "[静态局部变量式] 正在打印: " << printQueue.front() << endl;
            printQueue.pop();
        } else {
            cout << "[静态局部变量式] 当前无待打印任务" << endl;
        }
    }
    int jobCount() {
        lock_guard<mutex> lock(mtx);
        return static_cast<int>(printQueue.size());
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 打印队列管理系统（单例模式三种实现） ===" << endl;

    // 饿汉式
    cout << "\n--- 饿汉式单例 ---" << endl;
    PrinterManagerEager& eager1 = PrinterManagerEager::getInstance();
    PrinterManagerEager& eager2 = PrinterManagerEager::getInstance();
    cout << "eager1 和 eager2 是同一实例? " << ((&eager1 == &eager2) ? "是" : "否") << endl;
    eager1.addJob("Eager_文档A.pdf");
    eager2.addJob("Eager_图片B.png");
    cout << "当前队列任务数: " << eager1.jobCount() << endl;
    eager1.processJob();
    eager2.processJob();
    eager1.processJob();
    // 懒汉式
    cout << "\n--- 懒汉式单例 ---" << endl;
    PrinterManagerLazy* lazy1 = PrinterManagerLazy::getInstance();
    PrinterManagerLazy* lazy2 = PrinterManagerLazy::getInstance();
    cout << "lazy1 和 lazy2 是同一实例? " << ((lazy1 == lazy2) ? "是" : "否") << endl;
    lazy1->addJob("Lazy_报告C.docx");
    lazy2->addJob("Lazy_表格D.xlsx");
    cout << "当前队列任务数: " << lazy1->jobCount() << endl;
    lazy1->processJob();
    lazy2->processJob();
    lazy1->processJob();
    // 静态局部变量式
    cout << "\n--- 静态局部变量式单例 ---" << endl;
    PrinterManagerStatic& static1 = PrinterManagerStatic::getInstance();
    PrinterManagerStatic& static2 = PrinterManagerStatic::getInstance();
    cout << "static1 和 static2 是同一实例? " << ((&static1 == &static2) ? "是" : "否") << endl;
    static1.addJob("Static_合同E.pdf");
    static2.addJob("Static_图片F.png");
    cout << "当前队列任务数: " << static1.jobCount() << endl;
    static1.processJob();
    static2.processJob();
    static1.processJob();

    cout << "\n全部测试完毕。" << endl;

    cin.get();
    return 0;
}