#include <iostream>
#include <map>
#include <memory>
#include <windows.h>

using namespace std;

// 抽象表达式
class Expression {
public:
    virtual int interpret(const map<string, int>& context) = 0;
    virtual ~Expression() = default;
};

// 终结符表达式 - 常量
class NumberExpression : public Expression {
    int value;
public:
    NumberExpression(int v) : value(v) {}
    int interpret(const map<string, int>& context) override {
        return value;
    }
};

// 终结符表达式 - 变量
class VariableExpression : public Expression {
    string name;
public:
    VariableExpression(const string& n) : name(n) {}
    int interpret(const map<string, int>& context) override {
        auto it = context.find(name);
        if (it != context.end()) {
            return it->second;
        }
        return 0;
    }
};

// 非终结符表达式 - 加法
class AddExpression : public Expression {
    shared_ptr<Expression> left;
    shared_ptr<Expression> right;
public:
    AddExpression(shared_ptr<Expression> l, shared_ptr<Expression> r)
        : left(l), right(r) {}
    int interpret(const map<string, int>& context) override {
        return left->interpret(context) + right->interpret(context);
    }
};

// 非终结符表达式 - 乘法
class MultiplyExpression : public Expression {
    shared_ptr<Expression> left;
    shared_ptr<Expression> right;
public:
    MultiplyExpression(shared_ptr<Expression> l, shared_ptr<Expression> r)
        : left(l), right(r) {}
    int interpret(const map<string, int>& context) override {
        return left->interpret(context) * right->interpret(context);
    }
};

// 非终结符表达式 - 减法
class SubtractExpression : public Expression {
    shared_ptr<Expression> left;
    shared_ptr<Expression> right;
public:
    SubtractExpression(shared_ptr<Expression> l, shared_ptr<Expression> r)
        : left(l), right(r) {}
    int interpret(const map<string, int>& context) override {
        return left->interpret(context) - right->interpret(context);
    }
};

int main() {
    // 设置控制台编码为UTF-8，解决中文显示问题
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 解释器模式示例：游戏属性公式解析 ===" << endl;

    // 创建角色属性上下文
    map<string, int> context;
    context["力量"] = 20;
    context["武器加成"] = 15;

    cout << "\n角色属性上下文:" << endl;
    cout << "力量 = " << context["力量"] << endl;
    cout << "武器加成 = " << context["武器加成"] << endl;

    // 构建表达式：攻击力 = 力量 * 2 + 武器加成
    // 即 AddExpression(MultiplyExpression(VariableExpression("力量"), NumberExpression(2)), VariableExpression("武器加成"))
    auto expr = make_shared<AddExpression>(
        make_shared<MultiplyExpression>(
            make_shared<VariableExpression>("力量"),
            make_shared<NumberExpression>(2)
        ),
        make_shared<VariableExpression>("武器加成")
    );

    cout << "\n表达式计算:" << endl;
    cout << "攻击力 = 力量 * 2 + 武器加成 = " << expr->interpret(context) << endl;
    cout << "计算过程:" << endl;
    int li = context["力量"];
    int wuqi = context["武器加成"];
    cout << "  力量 * 2 = " << li << " * 2 = " << li * 2 << endl;
    cout << "  (力量 * 2) + 武器加成 = " << (li * 2) << " + " << wuqi << " = " << expr->interpret(context) << endl;

    cin.get();
    return 0;
}
