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
	memory[7] = PUSH + (1 << 4);
	memory[8] = POP + (2 << 4);
	memory[9] = JMP + (420 << 4);
	memory[420] = LOAD + (1283 << 4);
	memory[15] = 15;

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
		// TODO: Shorten this... maybe?
		if(fromMem == 0 && toMem == 0)
		{
			registers[idx] = registers[idy];
			std::cout << "Register[" << idx 
				<< "] now has the value from Register[" 
				<< idy << "](" << registers[idy] << ")" << std::endl;
		}
		else if(fromMem == 0 && toMem == 1)
		{
			memory[registers[idx]] = registers[idy];
			std::cout << "Memory[" << registers[idx]
				<< "] now has the value from Register[" 
				<< idy << "](" << registers[idy] << ")" << std::endl;
		}
		else if(fromMem == 1 && toMem == 0)
		{
			registers[idx] = memory[registers[idy]];
			std::cout << "Register[" << idx 
				<< "] now has the value from Memory[" 
				<< registers[idy] << "](" << memory[registers[idy]] << ")" << std::endl;
		} 
		else if(fromMem == 1 && toMem == 1)
		{
			memory[registers[idx]] = memory[registers[idy]];
			std::cout << "Memory[" << memory[registers[idx]]
				<< "] now has the value from Memory[" 
				<< registers[idy] << "](" << memory[registers[idy]] << ")" << std::endl;
		}
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
	case PUSH:
		// TODO: Not sure if Rx is a parameter!
		std::cout << "PUSH command found" << std::endl;
		stack.push(registers[idx]);
		std::cout << "Register[" << idx << "] pushed onto stack" << std::endl;
		programCounter++;
		break;
	case POP:
		// TODO: Not sure if Rx is a parameter!
		std::cout << "POP command found" << std::endl;
		registers[idx] = stack.top();
		std::cout << "Popped top from stack into Register[" << idx << "](" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	case JMP:
		std::cout << "JMP command found" << std::endl;
		programCounter = value;
		break;
	default:
		std::cout << "Invalid command!" << std::endl;
		cntProg = false;
	}
}

VirtualMachine::~VirtualMachine()
{
}
