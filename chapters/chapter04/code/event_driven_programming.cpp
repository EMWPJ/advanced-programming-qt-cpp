#include <iostream>
#include <functional>
#include <map>
#include <string>
// 简单的事件驱动框架
class EventEmitter {
public:
    using Handler = std::function<void()>;
    // 注册事件处理器
    void on(const std::string& event, Handler handler) { handlers[event] = handler; }
    // 触发事件
    void emit(const std::string& event) {
        if (handlers.count(event)) { handlers[event](); }
        else { std::cout << "未找到事件处理器: " << event << std::endl; }
    }
private:
    std::map<std::string, Handler> handlers;
};
int main() {
    EventEmitter emitter;
    // 注册事件及其处理器
    emitter.on("start", []() { std::cout << "程序启动事件被触发！" << std::endl; });
    emitter.on("click", []() { std::cout << "按钮点击事件被触发！" << std::endl; });
    emitter.on("exit", []() { std::cout << "程序退出事件被触发！" << std::endl; });
    // 模拟事件触发
    emitter.emit("start");
    emitter.emit("click");
    emitter.emit("exit");
    emitter.emit("unknown"); // 未注册事件
    std::cin.get();
    return 0;
}
