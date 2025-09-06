#include <iostream>
// 定义一个函数，作为事件发生时的响应（回调函数）
void onButtonClick() {
    std::cout << "Button is clicked!" << std::endl;
}
// 模拟一个“按钮”对象，它有一个点击事件
class Button {
public:
    // 保存事件回调函数的指针
    void (*onClick)() = nullptr;
    // 模拟点击按钮的动作
    void click() {
        std::cout << "Button is being clicked..." << std::endl;
        if (onClick) {
            onClick();  // 触发事件
        }
    }
};

// 主函数演示
int main() {
    Button btn;

    // 注册事件：把 onButtonClick 函数赋给 onClick
    btn.onClick = onButtonClick;

    // 模拟用户点击
    btn.click();
    // 等待用户输入
    std::cin.get();
    return 0;
}