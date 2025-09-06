#include <iostream>
#include <string>

template<typename T = int, int N = 10>
class DefaultArray {
public:
    DefaultArray() { for (int i = 0; i < N; ++i) m_data[i] = T(); }
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
    DefaultArray<> a;
    for (int i = 0; i < a.size(); ++i) a[i] = i + 1;
    a.print();

    DefaultArray<double> b;
    for (int i = 0; i < b.size(); ++i) b[i] = i * 1.1;
    b.print();

    DefaultArray<std::string, 3> c;
    c[0] = "Hello"; c[1] = "C++"; c[2] = "Template";
    c.print();
    std::cin.get();
    return 0;
}
