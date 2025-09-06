#include <iostream>
#include <functional>
// Property类用于实现属性的声明式绑定，支持属性值变化时自动通知回调
template<typename T>
class Property {
public:
    using Callback = std::function<void(const T&)>; // 属性变化时的回调类型
    Property(T v) : value(v) {}
    void set(T v) {
        if (value != v) {
            value = v;
            if (cb) cb(value);
        }
    }
    T get() const { return value; }
    void bind(Callback f) { cb = f; }
private:
    T value;      // 属性值
    Callback cb;  // 属性变化时的回调
};
int main() {
    // 定义三个属性：width、height、area
    Property<int> width(100), height(50), area(0);
    // 定义一个函数，当width或height变化时自动更新area
    auto updateArea = [&]() { area.set(width.get() * height.get()); };
    width.bind([&](int){ updateArea(); });
    height.bind([&](int){ updateArea(); });
    // 初始化时先计算一次area
    updateArea();
    // 输出初始值
    std::cout << width.get() << " " << height.get() << " " << area.get() << std::endl;
    // 修改width/height，area会自动更新
    width.set(200); // height.set(80);
    std::cout << width.get() << " " << height.get() << " " << area.get() << std::endl;
    return 0;
}
