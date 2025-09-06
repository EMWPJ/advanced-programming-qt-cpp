#include <iostream>

template<typename T, int N>
class ArrayWrapper {
public:
    ArrayWrapper() { for (int i = 0; i < N; ++i) m_data[i] = T(); }
    T& operator[](int i) { return m_data[i]; }
    int size() const { return N; }
    void print() const {
        for (int i = 0; i < N; ++i) std::cout << m_data[i] << " ";
        std::cout << std::endl;
    }
private:
    T m_data[N];
};

int main() {
    ArrayWrapper<int, 5> a;
    for (int i = 0; i < a.size(); ++i) a[i] = i * 2;
    a.print();
    ArrayWrapper<double, 3> b;
    b[0] = 3.14; b[1] = 2.71; b[2] = 1.41;
    b.print();
    std::cin.get();
    return 0;
}
