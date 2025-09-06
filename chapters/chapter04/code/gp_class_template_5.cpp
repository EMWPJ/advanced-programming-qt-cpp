#include <iostream>
#include <vector>
#include <list>
#include <string>
// 模板模板参数示例：容器包装类
template <template <typename, typename> class Container, typename T>
class ContainerWrapper {
public:
    ContainerWrapper() = default;
    void add(const T& value) {
        m_container.push_back(value);
    }
    void print() const {
        std::cout << "Container contents: ";
        for (const auto& item : m_container) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
private:
    Container<T, std::allocator<T>> m_container;
};
int main() {
    // 使用std::vector作为模板模板参数
    ContainerWrapper<std::vector, int> intVectorWrapper;
    intVectorWrapper.add(1);
    intVectorWrapper.add(2);
    intVectorWrapper.add(3);
    intVectorWrapper.print();

    // 使用std::list作为模板模板参数
    ContainerWrapper<std::list, std::string> stringListWrapper;
    stringListWrapper.add("Hello");
    stringListWrapper.add("World");
    stringListWrapper.print();
    std::cin.get();
    return 0;
}
