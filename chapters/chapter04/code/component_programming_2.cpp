#include <iostream>
#include <memory>
#include <unordered_map>
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

// 组件管理器
class ComponentManager {
public:
    void registerComponent(const std::shared_ptr<IComponent>& comp) {
        components_[comp->name()] = comp;
    }
    std::shared_ptr<IComponent> getComponent(const std::string& name) const {
        auto it = components_.find(name);
        return it != components_.end() ? it->second : nullptr;
    }
    void processAll() {
        for (auto& [n, c] : components_) {
            std::cout << "[" << n << "] ";
            c->process();
        }
    }
private:
    std::unordered_map<std::string, std::shared_ptr<IComponent>> components_;
};

int main() {
    ComponentManager manager;
    manager.registerComponent(std::make_shared<ComponentA>());
    manager.registerComponent(std::make_shared<ComponentB>());
    manager.processAll();
    auto comp = manager.getComponent("ComponentA");
    if (comp) {
        std::cout << "单独处理: " << comp->name() << std::endl;
        comp->process();
    }
    return 0;
}
