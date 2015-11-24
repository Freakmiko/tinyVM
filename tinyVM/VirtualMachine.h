#pragma once
#include <stack>
#include <string>

const unsigned short NOP = 0;
const unsigned short LOAD = 1;
const unsigned short MOV = 2;
const unsigned short ADD = 3;
const unsigned short SUB = 4;
const unsigned short MUL = 5;
const unsigned short DIV = 6;
const unsigned short PUSH = 7;
const unsigned short POP = 8;
const unsigned short JMP = 9;
const unsigned short JIZ = 10;
const unsigned short JIH = 11;
const unsigned short JSR = 12;
const unsigned short RTS = 13;

class VirtualMachine
{
public:
	VirtualMachine();
	~VirtualMachine();
	void readProgram(const std::string& filePath);
	void start();
private:
	void run();

private:
	int programCounter;
	unsigned short registers[16];
	unsigned short memory[4096];
	int profiler[4096];
	std::stack<unsigned short> registerStack;
	std::stack<int> subroutineStack;

	bool cntProg;

	int shiftCounter;
	std::string filePath;
};

