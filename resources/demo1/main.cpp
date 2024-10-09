#include <iostream>


// 函数声明
extern int add(int a, int b);
extern int subtract(int a, int b);

int main() {
    int a = 5, b = 3;

    // 使用来自 math_functions 的函数
    std::cout << "Sum: " << add(a, b) << std::endl;
    std::cout << "Difference: " << subtract(a, b) << std::endl;

    return 0;
}