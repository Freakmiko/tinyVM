#pragma once
#include <stack>

const unsigned short NOP = 0;
const unsigned short LOAD = 1;
const unsigned short MOV = 3;
const unsigned short ADD = 4;
const unsigned short SUB = 5;
const unsigned short MUL = 6;
const unsigned short DIV = 7;
const unsigned short PUSH = 8;
const unsigned short POP = 9;
const unsigned short JMP = 10;
const unsigned short JIZ = 11;
const unsigned short JIH = 12;
const unsigned short JSR = 13;
const unsigned short RTS = 14;

class VirtualMachine
{
public:
	VirtualMachine();
	~VirtualMachine();
	void start();
private:
	void run();

	int programCounter;
	unsigned short registers[16];
	unsigned short memory[4096];
	std::stack<unsigned short> stack;

	bool cntProg;
};

