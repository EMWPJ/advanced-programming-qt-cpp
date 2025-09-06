#include <iostream>
#include <functional>
#include <vector>
// 简单的惰性求值类
template<typename T>
class LazyValue {
    std::function<T()> thunk;
    mutable bool evaluated = false;
    mutable T value;
public:
    LazyValue(std::function<T()> f) : thunk(f) {}
    T get() const {
        if (!evaluated) {
            value = thunk();
            evaluated = true;
        }
        return value;
    }
};
int main() {
    int x = 10, y = 20;
    LazyValue<int> lazy_sum([=] { std::cout << "计算 x + y\n"; return x + y; });
    std::cout << "未计算 lazy_sum\n";
    std::cout << "lazy_sum = " << lazy_sum.get() << std::endl;
    std::cout << "再次获取 lazy_sum = " << lazy_sum.get() << std::endl;
    int n = 8;
    std::vector<LazyValue<int>> fibs;
    fibs.emplace_back([] { return 0; });
    fibs.emplace_back([] { return 1; });
    for (int i = 2; i < n; ++i)
        fibs.emplace_back([i, &fibs] { return fibs[i-1].get() + fibs[i-2].get(); });
    std::cout << "斐波那契数列前" << n << "项: ";
    for (int i = 0; i < n; ++i)
        std::cout << fibs[i].get() << " ";
    std::cout << std::endl;
    return 0;
}
