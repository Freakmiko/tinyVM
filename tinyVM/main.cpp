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