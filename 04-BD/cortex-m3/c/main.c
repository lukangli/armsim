#include <stdint.h>

int a = 10;

int func1(int a1, int a2, int a3)
{
    return a1 + a2 + a3;
}

int func2()
{
    int func2_a = 10;
    return func2_a;
}

int func(int a1, int a2, int a3, int a4, int a5, int a6)
{
    long c = 10;
    c += 5; //局部变量
	return a1 + a2 + a3 + a4 + a5 + a6;
}

int main(void)
{
    a += 5; //全局变量
	func(1, 2, 3, 4, 5, 6);
    func1(1, 2, 3);
    func2();
	return 0;
}
