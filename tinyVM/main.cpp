#include <iostream>

#include "VirtualMachine.h"

int main(int argc, char* argv[])
{
	VirtualMachine vm;
	vm.start();
	int c;
	std::cin >> c;
	return 0;
}