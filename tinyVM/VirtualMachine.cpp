#include "VirtualMachine.h"
#include <iostream>

VirtualMachine::VirtualMachine() : programCounter(0), cntProg(true)
{
	memory[0] = NOP;
	memory[1] = LOAD + (1283 << 4);
	memory[2] = MOV + (1 << 4) + (12 << 8);
	memory[3] = ADD + (1 << 4) + (1 << 8);
	memory[4] = SUB + (1 << 4) + (12 << 8);
	memory[5] = MUL + (1 << 4) + (2 << 8);
	memory[6] = DIV + (1 << 4) + (2 << 8);
	memory[7] = 15;

	memory[1283] = 69;
	registers[12] = 420;
	registers[2] = 2;
}


void VirtualMachine::start()
{
	do
	{
		run();
	} while (cntProg);
}

void VirtualMachine::run()
{
	unsigned short opCode = memory[programCounter];
	// use bitmask (15) to filter out the command
	unsigned short command = opCode & 0xF;
	unsigned short idx = (opCode >> 4) & 0xF;
	unsigned short idy = (opCode >> 8) & 0xF;
	unsigned short fromMem = (opCode >> 12) & 0xF;
	unsigned short toMem = (opCode >> 13) & 0xF;
	unsigned short value = (opCode >> 4) & 0xFFF;

	switch(command)
	{
	case NOP:
		std::cout << "NOP command found" << std::endl;
		programCounter++;
		break;
	case LOAD:
		std::cout << "LOAD command found" << std::endl;
		registers[0] = memory[value];
		std::cout << "Register 0 loaded the value: " << memory[value] << std::endl;
		programCounter++;
		break;
	case MOV:
		std::cout << "MOV command found" << std::endl;
		if(fromMem == 0 && toMem == 0)
		{
			registers[idx] = registers[idy];
			std::cout << "Register[" << idx 
				<< "] now has the value from Register[" 
				<< idy << "](" << registers[idy] << ")" << std::endl;
		}
		// TODO: implement to and from Memory
		programCounter++;
		break;
	case ADD:
		std::cout << "ADD command found" << std::endl;
		registers[idx] += registers[idy];
		std::cout << "Register[" << idx << "] += Register[" << idy << "] (" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	case SUB:
		std::cout << "SUB command found" << std::endl;
		registers[idx] -= registers[idy];
		std::cout << "Register[" << idx << "] -= Register[" << idy << "] (" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	case MUL:
		std::cout << "MUL command found" << std::endl;
		registers[idx] *= registers[idy];
		std::cout << "Register[" << idx << "] *= Register[" << idy << "] (" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	case DIV:
		std::cout << "DIV command found" << std::endl;
		registers[idx] /= registers[idy];
		std::cout << "Register[" << idx << "] /= Register[" << idy << "] (" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	default:
		std::cout << "Invalid command!" << std::endl;
		cntProg = false;
	}
}

VirtualMachine::~VirtualMachine()
{
}
