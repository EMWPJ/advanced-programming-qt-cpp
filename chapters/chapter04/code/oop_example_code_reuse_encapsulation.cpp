#include <iostream>
#include <string>

// 封装：将数据和操作数据的方法封装在类中
class Account {
public:
    Account(const std::string& owner, double balance)
        : owner(owner), balance(balance) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
        }
    }

    double getBalance() const {
        return balance;
    }

private:
    std::string owner;
    double balance; // 封装：外部无法直接访问
};

int main() {
    Account acc("张三", 1000.0);
    acc.deposit(500.0);
    acc.withdraw(200.0);
    std::cout << "最终余额：" << acc.getBalance() << " 元" << std::endl;
    std::cin.get();
    return 0;
}
