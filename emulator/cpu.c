#include "cpu.h"

// NEVER free program! (TODO memcpy it)
void initializeCPU(CPU *cpu, char *program, int programLength) {
	cpu->memory = malloc(sizeof(char) * MEMORY_SIZE);
	cpu->program = program;
	cpu->programLength = programLength;
	cpu->pc = 0;
	cpu->sp = cpu->a = cpu->x = cpu->y = cpu->ps = 0; // TODO: move to reset function
}

void writeMemory(CPU *cpu, char *buffer, int start, int offset) {
	int i;
	for(i = 0; i < offset; i++) {
		cpu->memory[start + i] = buffer[i];
	}
}

void readMemory(CPU *cpu, char *buffer, int start, int offset) {
	int i;
	for(i = 0; i < offset; i++) {
		buffer[i] = cpu->memory[start + i];
	}
}

void printMemory(CPU *cpu) {
	int i, j;

	for(i = 0; i < MEMORY_PAGES; i++) {
		printf("=== Page %i\n", i);
		for(j = 0;  j < PAGE_SIZE; j++) {
			printf("%i ", cpu->memory[(i * PAGE_SIZE) + j]);
		}
		printf("\n");
	}
}

void freeCPU(CPU *cpu) {
	free(cpu->memory);
}

void updateStatusFlag(CPU *cpu, char operationResult) { // operationResult can be accumulator, X, Y or any registers
	cpu->ps = (operationResult == 0 ? cpu->ps | 0x2 : cpu->ps & 0xFD ); // sets zero flag
	cpu->ps = (operationResult & 0x80 != 0 ? cpu->ps | 0x80 : cpu-> ps & 0x7F );
}

void step(CPU *cpu) { // main code is here
	char currentOpcode = cpu->program[cpu->pc++]; // read program byte number 'program counter' (starting at 0)
	switch(currentOpcode) {
		case 0x0: // BRK impl
		break;
		case 0x01: // ORA X,ind
			cpu->a |= cpu->memory[cpu->program[cpu->pc++] + cpu->x]; // OR with accumulator and memory at (next byte after opcode + X)
			updateStatusFlag(cpu, cpu->a);
			// cpu->
		break;
	}
}

int main() {
	const char program[] = { 0x01, 0x39 };
	CPU cpu;
	initializeCPU(&cpu, program, sizeof(program));

	char *buf = malloc(sizeof(char));
	buf[0] = 0x50;
	writeMemory(&cpu, buf, 0x40, 1);
	free(buf);
	
	cpu.x = 0x1;
	cpu.a = 0x10;
	
	step(&cpu);

	printMemory(&cpu);
	printf("cpu->a: %i\n", cpu.a);
	freeCPU(&cpu);

	return 0;
}