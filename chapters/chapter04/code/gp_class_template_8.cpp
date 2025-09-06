#include <iostream>
#include <string>
// 基类模板
template<typename T>
class Base {
public:
    Base(const T& value) : m_value(value) {}
    void show() const { std::cout << "Base value: " << m_value << std::endl; }
protected:
    T m_value;
};
// 派生类模板，继承自基类模板
template<typename T>
class Derived : public Base<T> {
public:
    Derived(const T& value, const std::string& name)
        : Base<T>(value), m_name(name) {}
    void show() const { std::cout << "Derived name: " << m_name << ", value: " << this->m_value << std::endl; }
    void showBase() const { Base<T>::show(); }
private:
    std::string m_name;
};

int main() {
    Base<int> baseObj(100);
    baseObj.show();
    Derived<int> derivedObj(200, "派生对象");
    derivedObj.show();
    derivedObj.showBase();
    Derived<std::string> derivedStrObj("Hello", "字符串派生");
    derivedStrObj.show();
    derivedStrObj.showBase();
    std::cin.get();
    return 0;
}
