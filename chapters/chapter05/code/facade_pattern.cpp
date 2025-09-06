#include <iostream>
#include <string>
#include <memory>
#include <windows.h>
using namespace std;

// 子系统：智能灯光
class LightSystem {
public:
    void on() { cout << "智能灯光已开启" << endl; }
    void off() { cout << "智能灯光已关闭" << endl; }
    void setBrightness(int level) {
        cout << "智能灯光亮度设置为：" << level << "%" << endl;
    }
};

// 子系统：智能空调
class AirConditioner {
public:
    void on() { cout << "智能空调已开启" << endl; }
    void off() { cout << "智能空调已关闭" << endl; }
    void setTemperature(int temp) {
        cout << "智能空调温度设置为：" << temp << "℃" << endl;
    }
};

// 子系统：安全监控
class SecuritySystem {
public:
    void arm() { cout << "安全监控系统已布防" << endl; }
    void disarm() { cout << "安全监控系统已撤防" << endl; }
};

// 外观类：智能家居外观
class SmartHomeFacade {
private:
    unique_ptr<LightSystem> light;
    unique_ptr<AirConditioner> air;
    unique_ptr<SecuritySystem> security;
public:
    SmartHomeFacade() {
        light = make_unique<LightSystem>();
        air = make_unique<AirConditioner>();
        security = make_unique<SecuritySystem>();
    }

    // 回家模式
    void enterHomeMode() {
        cout << "🏠 [外观] 启动回家模式..." << endl;
        security->disarm();
        light->on();
        light->setBrightness(80);
        air->on();
        air->setTemperature(24);
        cout << "✅ 回家模式已开启，欢迎回家！" << endl;
    }

    // 离家模式
    void leaveHomeMode() {
        cout << "🚪 [外观] 启动离家模式..." << endl;
        light->off();
        air->off();
        security->arm();
        cout << "✅ 离家模式已开启，家中安全！" << endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 外观模式示例：智能家居系统 ===" << endl;

    SmartHomeFacade facade;

    cout << "\n【场景1】回家：" << endl;
    facade.enterHomeMode();

    cout << "\n【场景2】离家：" << endl;
    facade.leaveHomeMode();

    cout << "\n演示结束，外观模式让复杂的子系统操作变得简单！" << endl;

    cin.get();
    return 0;
}
