#include "VirtualMachine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

VirtualMachine::VirtualMachine() : programCounter(0), cntProg(true)
{
	/*memory[0] = NOP;
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
	registers[2] = 2;*/
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

	switch (command)
	{
	case NOP:
		std::cout << "NOP command found" << std::endl;
		programCounter++;
		break;
	case LOAD:
		std::cout << "LOAD command found" << std::endl;
		registers[0] = value;
		std::cout << "Register 0 loaded the value: " << value << std::endl;
		programCounter++;
		break;
	case MOV:
		std::cout << "MOV command found" << std::endl;
		// TODO: Shorten this... maybe?
		if (fromMem == 0 && toMem == 0)
		{
			registers[idx] = registers[idy];
			std::cout << "Register[" << idx
				<< "] now has the value from Register["
				<< idy << "](" << registers[idy] << ")" << std::endl;
		}
		else if (fromMem == 0 && toMem == 1)
		{
			memory[registers[idx]] = registers[idy];
			std::cout << "Memory[" << registers[idx]
				<< "] now has the value from Register["
				<< idy << "](" << registers[idy] << ")" << std::endl;
		}
		else if (fromMem == 1 && toMem == 0)
		{
			registers[idx] = memory[registers[idy]];
			std::cout << "Register[" << idx
				<< "] now has the value from Memory["
				<< registers[idy] << "](" << memory[registers[idy]] << ")" << std::endl;
		}
		else if (fromMem == 1 && toMem == 1)
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
		registerStack.push(registers[idx]);
		std::cout << "Register[" << idx << "] pushed onto stack" << std::endl;
		programCounter++;
		break;
	case POP:
		// TODO: Not sure if Rx is a parameter!
		std::cout << "POP command found" << std::endl;
		registers[idx] = registerStack.top();
		registerStack.pop();
		std::cout << "Popped top from stack into Register[" << idx << "](" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	case JMP:
		std::cout << "JMP command found" << std::endl;
		programCounter = value;
		break;
	case JIZ:
		std::cout << "JIZ command found" << std::endl;
		if (registers[0] == 0)
			programCounter = value;
		else
			programCounter++;
		break;
	case JIH:
		std::cout << "JIH command found" << std::endl;
		if (registers[0] > 0)
			programCounter = value;
		else
			programCounter++;
		break;
	case JSR:
		std::cout << "JSR command found" << std::endl;
		subroutineStack.push(++programCounter);
		programCounter = value;
		break;
	case RTS:
		std::cout << "RTS command found" << std::endl;
		if (subroutineStack.empty()) {
			cntProg = false;
		}
		else {
			programCounter = subroutineStack.top();
			subroutineStack.pop();
		}
		break;
	default:
		std::cout << "Invalid command!" << std::endl;
		cntProg = false;
	}
	std::cout << std::endl;
}

VirtualMachine::~VirtualMachine()
{
}

void VirtualMachine::readProgram(const std::string filePath)
{
	std::ifstream program(filePath);
	if (program.fail())
	{
		std::cout << "Could not open file!";
		exit(EXIT_FAILURE);
	}

	std::string currentLine("");

	unsigned short command = 0;
	char buffer[255];
	int programLine = 0;

	while (!program.eof())
	{
		std::getline(program, currentLine);

		std::string subString;
		std::stringstream currLinStream(currentLine);
		std::getline(currLinStream, subString, ' ');
		//currLinStream.read(buffer, subString.length());
		std::cout << "Command: " << subString << std::endl;

		if (subString == "NOP")
		{
			command = NOP;
			memory[programLine++] = command;
		}
		else if (subString == "LOAD")
		{
			command = LOAD;
			unsigned short value;
			currLinStream >> value;
			std::cout << "Value: " << value << std::endl;
			command += (value << 4);
			memory[programLine++] = command;
		}
		else if (subString == "MOV") 
		{
			int rx;
			int ry;
			char comma;

			command = MOV;
			currLinStream >> rx;
			std::cout << "RX: " << rx << std::endl;
			currLinStream >> comma;
			currLinStream >> ry;
			std::cout << "RY: " << ry << std::endl;
			command += (rx << 4) + (ry << 8);
			memory[programLine++] = command;
		}
		else if (subString == "ADD")
		{
			int rx;
			int ry;
			char comma;

			command = ADD;
			currLinStream >> rx;
			std::cout << "RX: " << rx << std::endl;
			currLinStream >> comma;
			currLinStream >> ry;
			std::cout << "RY: " << ry << std::endl;
			command += (rx << 4) + (ry << 8);
			memory[programLine++] = command;
		}
		else if (subString == "SUB")
		{
			int rx;
			int ry;
			char comma;

			command = SUB;
			currLinStream >> rx;
			std::cout << "RX: " << rx << std::endl;
			currLinStream >> comma;
			currLinStream >> ry;
			std::cout << "RY: " << ry << std::endl;
			command += (rx << 4) + (ry << 8);
			memory[programLine++] = command;
		}
		else if (subString == "MUL")
		{
			int rx;
			int ry;
			char comma;

			command = MUL;
			currLinStream >> rx;
			std::cout << "RX: " << rx << std::endl;
			currLinStream >> comma;
			currLinStream >> ry;
			std::cout << "RY: " << ry << std::endl;
			command += (rx << 4) + (ry << 8);
			memory[programLine++] = command;
		}
		else if (subString == "DIV")
		{
			int rx;
			int ry;
			char comma;

			command = DIV;
			currLinStream >> rx;
			std::cout << "RX: " << rx << std::endl;
			currLinStream >> comma;
			currLinStream >> ry;
			std::cout << "RY: " << ry << std::endl;
			command += (rx << 4) + (ry << 8);
			memory[programLine++] = command;
		}
		else if (subString == "PUSH")
		{
			int rx;

			command = PUSH;
			currLinStream >> rx;
			std::cout << "RX: " << rx << std::endl;
			command += (rx << 4);
			memory[programLine++] = command;
		}
		else if (subString == "POP")
		{
			int rx;

			command = POP;
			currLinStream >> rx;
			std::cout << "RX: " << rx << std::endl;
			command += (rx << 4);
			memory[programLine++] = command;
		}
		else if (subString == "RTS")
		{
			command = RTS;
			memory[programLine++] = command;
		}
	}

	program.close();
}
