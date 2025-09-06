#include <iostream>
#include <string>
// 组件接口
class IComponent {
public:
    virtual ~IComponent() = default;
    virtual void process() = 0;
    virtual std::string name() const = 0;
};
// 具体组件A
class ComponentA : public IComponent {
public:
    void process() override { std::cout << "ComponentA 正在处理任务" << std::endl; }
    std::string name() const override { return "ComponentA"; }
};
// 具体组件B
class ComponentB : public IComponent {
public:
    void process() override { std::cout << "ComponentB 正在处理任务" << std::endl; }
    std::string name() const override { return "ComponentB"; }
};
int main() {
    IComponent* comp1 = new ComponentA();
    IComponent* comp2 = new ComponentB();
    std::cout << comp1->name() << std::endl;
    comp1->process();
    std::cout << comp2->name() << std::endl;
    comp2->process();
    delete comp1;
    delete comp2;
    std::cin.get();
    return 0;
}
