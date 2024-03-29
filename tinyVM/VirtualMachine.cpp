#include "VirtualMachine.h"
#include <iostream>
#include <fstream>
#include <sstream>

VirtualMachine::VirtualMachine() : programCounter(0), cntProg(true), shiftCounter(0)
{
	for(int i = 0; i < 4096; ++i) {
		memory[i] = 0;
		profiler[i] = 0;

		if(i < 16) {
			registers[i] = 0;
		}
	}
}


void VirtualMachine::start()
{
	do
	{
		profiler[programCounter]++;
		run();
	} while (cntProg);

	double sum = 0;
	for (int i = 0; i < 4096; ++i) {
		sum += profiler[i];
	}

	std::ifstream codeFile(this->filePath);
	std::ofstream outputFile("output.txt");
	std::string currentLine("");
	int counter = 0;
	while(!codeFile.eof()) {
		std::getline(codeFile, currentLine);
		outputFile << profiler[counter++] / sum * 100 << "%: " << currentLine << std::endl;
	}
	codeFile.close();
	outputFile.close();
}

void VirtualMachine::run()
{
	unsigned short opCode = memory[programCounter];
	// use bitmask (15) to filter out the command
	unsigned short command = opCode & 0xF;
	unsigned short idx = (opCode >> 4) & 0xF;
	unsigned short idy = (opCode >> 8) & 0xF;
	unsigned short fromMem = (opCode >> 12) & 0x1;
	unsigned short toMem = (opCode >> 13) & 0x1;
	unsigned short value = (opCode >> 4) & 0xFFF;

    std::string shiftString;
    for(int i = 0; i < shiftCounter; ++i) {
        shiftString += "\t";
    }


	switch (command)
	{
	case NOP:
		// std::cout << shiftString << "NOP command found" << std::endl;
		programCounter++;
		break;
	case LOAD:
		// std::cout << shiftString << "LOAD command found" << std::endl;
		registers[0] = value;
		// std::cout << shiftString << "Register 0 loaded the value: " << value << std::endl;
		programCounter++;
		break;
	case MOV:
		// std::cout << shiftString << "MOV command found" << std::endl;
		if (fromMem == 0 && toMem == 0)
		{
			registers[idx] = registers[idy];
			// std::cout << shiftString << "Register[" << idx
//				<< "] now has the value from Register["
//				<< idy << "](" << registers[idy] << ")" << std::endl;
		}
		else if (fromMem == 0 && toMem == 1)
		{
			memory[registers[idx]] = registers[idy];
			// std::cout << shiftString << "Memory[" << registers[idx]
//				<< "] now has the value from Register["
//				<< idy << "](" << registers[idy] << ")" << std::endl;
		}
		else if (fromMem == 1 && toMem == 0)
		{
			registers[idx] = memory[registers[idy]];
			// std::cout << shiftString << "Register[" << idx
//				<< "] now has the value from Memory["
//				<< registers[idy] << "](" << memory[registers[idy]] << ")" << std::endl;
		}
		else if (fromMem == 1 && toMem == 1)
		{
			memory[registers[idx]] = memory[registers[idy]];
			// std::cout << shiftString << "Memory[" << memory[registers[idx]]
//				<< "] now has the value from Memory["
//				<< registers[idy] << "](" << memory[registers[idy]] << ")" << std::endl;
		}
		programCounter++;
		break;
	case ADD:
		// std::cout << shiftString << "ADD command found" << std::endl;
		registers[idx] += registers[idy];
		// std::cout << shiftString << "Register[" << idx << "] += Register[" << idy << "] (" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	case SUB:
		// std::cout << shiftString << "SUB command found" << std::endl;
		registers[idx] -= registers[idy];
		// std::cout << shiftString << "Register[" << idx << "] -= Register[" << idy << "] (" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	case MUL:
		// std::cout << shiftString << "MUL command found" << std::endl;
		registers[idx] *= registers[idy];
		// std::cout << shiftString << "Register[" << idx << "] *= Register[" << idy << "] (" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	case DIV:
		// std::cout << shiftString << "DIV command found" << std::endl;
		registers[idx] /= registers[idy];
		// std::cout << shiftString << "Register[" << idx << "] /= Register[" << idy << "] (" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	case PUSH:
		// std::cout << shiftString << "PUSH command found" << std::endl;
		registerStack.push(registers[idx]);
		// std::cout << shiftString << "Register[" << idx << "] pushed onto stack" << std::endl;
		programCounter++;
		break;
	case POP:
		// std::cout << shiftString << "POP command found" << std::endl;
		registers[idx] = registerStack.top();
		registerStack.pop();
		// std::cout << shiftString << "Popped top from stack into Register[" << idx << "](" << registers[idx] << ")" << std::endl;
		programCounter++;
		break;
	case JMP:
		// std::cout << shiftString << "JMP command found" << std::endl;
		programCounter = value;
		break;
	case JIZ:
		// std::cout << shiftString << "JIZ command found" << std::endl;
		if (registers[0] == 0)
			programCounter = value;
		else
			programCounter++;
		break;
	case JIH:
		// std::cout << shiftString << "JIH command found" << std::endl;
		if (registers[0] > 0)
			programCounter = value;
		else
			programCounter++;
		break;
	case JSR:
		// std::cout << shiftString << "JSR command found" << std::endl;
		subroutineStack.push(++programCounter);
		programCounter = value;
            shiftCounter++;
		break;
	case RTS:
		// std::cout << shiftString << "RTS command found" << std::endl;
            shiftCounter--;
		if (subroutineStack.empty()) {
			cntProg = false;
            programCounter++;
		}
		else {
			programCounter = subroutineStack.top();
			subroutineStack.pop();
		}
		break;
	default:
		// std::cout << shiftString << "Invalid command!" << std::endl;
		cntProg = false;
	}
	// std::cout << shiftString << std::endl;
}

VirtualMachine::~VirtualMachine()
{
}

void VirtualMachine::readProgram(const std::string& filePath)
{
	this->filePath = filePath;
	std::ifstream program(filePath);
	if (program.fail())
	{
		// std::cout << "Could not open file!";
		exit(EXIT_FAILURE);
	}


	std::string currentLine("");

	unsigned short command = 0;
	int programLine = 0;
	unsigned short rx = 0;
	unsigned short ry = 0;
	unsigned short value = 0;

	while (!program.eof())
	{
		command = 0;
		rx = 0;
		ry = 0;
		value = 0;

		std::getline(program, currentLine);

		// read the from-mem
		std::size_t found = currentLine.find(",(");
		if (found != std::string::npos)
		{
			// std::cout << "Found a (" << std::endl;
			currentLine.replace(found + 1, 1, "");
			found = currentLine.find(")");
			currentLine.replace(found, 1, "");
			command += 1 << (12);
		}

		// read the to-mem
		found = currentLine.find("(");
		if (found != std::string::npos)
		{
			// std::cout << "Found a (" << std::endl;
			currentLine.replace(found, 1, "");
			found = currentLine.find(")");
			currentLine.replace(found, 1, "");
			command += 1 << (13);
		}

		std::string subString;
		std::stringstream currLinStream(currentLine);
		std::getline(currLinStream, subString, ' ');
		// std::cout << "Command: " << subString << std::endl;

		if (subString == "NOP" || subString.find_first_not_of(' ') == std::string::npos)
		{
			command = NOP;
			memory[programLine++] = command;
		}
		else if (subString == "LOAD")
		{
			command = LOAD;
			currLinStream >> value;
			// std::cout << "Value: " << value << std::endl;
			command += (value << 4);
			memory[programLine++] = command;
		}
		else if (subString == "MOV")
		{
			char comma;

			command += MOV;
			currLinStream >> rx;
			// std::cout << "RX: " << rx << std::endl;
			currLinStream >> comma;
			currLinStream >> ry;
			// std::cout << "RY: " << ry << std::endl;
			command += (rx << 4) + (ry << 8);
			memory[programLine++] = command;
		}
		else if (subString == "ADD")
		{
			char comma;

			command = ADD;
			currLinStream >> rx;
			// std::cout << "RX: " << rx << std::endl;
			currLinStream >> comma;
			currLinStream >> ry;
			// std::cout << "RY: " << ry << std::endl;
			command += (rx << 4) + (ry << 8);
			memory[programLine++] = command;
		}
		else if (subString == "SUB")
		{
			char comma;

			command = SUB;
			currLinStream >> rx;
			// std::cout << "RX: " << rx << std::endl;
			currLinStream >> comma;
			currLinStream >> ry;
			// std::cout << "RY: " << ry << std::endl;
			command += (rx << 4) + (ry << 8);
			memory[programLine++] = command;
		}
		else if (subString == "MUL")
		{
			char comma;

			command = MUL;
			currLinStream >> rx;
			// std::cout << "RX: " << rx << std::endl;
			currLinStream >> comma;
			currLinStream >> ry;
			// std::cout << "RY: " << ry << std::endl;
			command += (rx << 4) + (ry << 8);
			memory[programLine++] = command;
		}
		else if (subString == "DIV")
		{
			char comma;

			command = DIV;
			currLinStream >> rx;
			// std::cout << "RX: " << rx << std::endl;
			currLinStream >> comma;
			currLinStream >> ry;
			// std::cout << "RY: " << ry << std::endl;
			command += (rx << 4) + (ry << 8);
			memory[programLine++] = command;
		}
		else if (subString == "PUSH")
		{
			command = PUSH;
			currLinStream >> rx;
			// std::cout << "RX: " << rx << std::endl;
			command += (rx << 4);
			memory[programLine++] = command;
		}
		else if (subString == "POP")
		{
			command = POP;
			currLinStream >> rx;
			// std::cout << "RX: " << rx << std::endl;
			command += (rx << 4);
			memory[programLine++] = command;
		}
		else if (subString == "JMP")
		{
			command = JMP;
			currLinStream >> value;
			// std::cout << "Value: " << value << std::endl;
			command += (value << 4);
			memory[programLine++] = command;
		}
		else if (subString == "JIZ")
		{
			command = JIZ;
			currLinStream >> value;
			// std::cout << "Value: " << value << std::endl;
			command += (value << 4);
			memory[programLine++] = command;
		}
		else if (subString == "JIH")
		{
			command = JIH;
			currLinStream >> value;
			// std::cout << "Value: " << value << std::endl;
			command += (value << 4);
			memory[programLine++] = command;
		}
		else if (subString == "JSR")
		{
			command = JSR;
			currLinStream >> value;
			// std::cout << "Value: " << value << std::endl;
			command += (value << 4);
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
