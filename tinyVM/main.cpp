#include <iostream>

#include "VirtualMachine.h"

int fib(int n);

int main(int argc, char* argv[])
{
	VirtualMachine vm;
	vm.readProgram("test.vm");
	vm.start();
//	std::cout << fib(6) << std::endl;
	return 0;
}

int fib(int n) {
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	else
		return fib(n-1) + fib(n-2);
}