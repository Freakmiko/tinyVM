#include <iostream>

#include "VirtualMachine.h"

int main(int argc, char* argv[])
{
	VirtualMachine vm;
	vm.readProgram("test.vm");
	vm.start();
	return 0;
}