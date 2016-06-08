// Name: Tyler Gauntlett
// Course: Systems Software
// Date: 5/26/2016

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

const char* getBasicOPCode(int code);
void updateGlobals(int code, int L, int M);
void nextPC();
int BASE(int L, int base);
void LIT(int L, int M);
void LOD(int L, int M);
void STO(int L, int M);
void INIT();
void CAL(int L, int M);
void INC(int L, int M);
void JMP(int L, int M);
void JPC(int L, int M);
void SIO(int L, int M);
void OPR(int L, int M);

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

struct instruction{
    int op;
    int l;
    int m;
    int line;
};

int SP = 0;
int BP = 1;
int PC = 0;
int incSize = 0;
int stack[MAX_STACK_HEIGHT];
int stackBar = 0;

void getBasicAssembly(struct instruction *inputArray) {
	FILE *inputFile, *outputFile;
	char outputFileName[] = "inputToBasicAssembly.txt";
	char *mode = "r";
	int line = 0;

	inputFile = fopen("mcode.txt", mode);
	outputFile = fopen(outputFileName, "w");

	if (inputFile == NULL)
	{
		perror("Error");
	}
	else {
		struct instruction instruct;

		fprintf(outputFile, "Line\tOP\tL\tM\n");
		while (!feof(inputFile))
		{
			fscanf(inputFile, "%d", &instruct.op);
			fscanf(inputFile, "%d", &instruct.l);
			fscanf(inputFile, "%d", &instruct.m);
			instruct.line = line;
			inputArray[line] = instruct;

			fprintf(outputFile, "%d\t%s\t%d\t%d\n", instruct.line, getBasicOPCode(instruct.op), instruct.l, instruct.m);

			line++;
		}
	}
	fclose(inputFile);
	fclose(outputFile);
}

const char* getBasicOPCode(int code) {

	switch (code) {
	case 1:
		return "lit";
	case 2:
		return "opr";
	case 3:
		return "lod";
	case 4:
		return "sto";
	case 5:
		return "cal";
	case 6:
		return "inc";
	case 7:
		return "jmp";
	case 8:
		return "jpc";
	case 9:
		return "sio";
	case 10:
		return "sio";
	case 11:
		return "sio";
	default:
		return "Invalid code.";
	}
}

void updateGlobals(int code, int L, int M) {

	switch (code) {
	case 1:
		LIT(L, M);
		return;
	case 2:
		OPR(L, M);
		return;
	case 3:
		LOD(L, M);
		return;
	case 4:
		STO(L, M);
		return;
	case 5:
		CAL(L, M);
		return;
	case 6:
		INC(L, M);
		return;
	case 7:
		JMP(L, M);
		return;
	case 8:
		JPC(L, M);
		return;
	case 9:
		SIO(L, M);
		return;
	case 10:
		SIO(L, M);
		return;
	case 11:
		SIO(L, M);
		return;
	default:
		return;
	}
}

void getAdvancedAssembly(struct instruction *inputArray) {
	FILE *inputFile, *outputFile;
	char outputFileName[] = "inputToAdvancedAssembly.txt";
	char *mode = "r";
	int line = 0;

	void INIT();

	inputFile = fopen("mcode.txt", mode);
	outputFile = fopen(outputFileName, "w");

	if (inputFile == NULL)
	{
		perror("Error");
	}
	else {
		// Init headers.
		fprintf(outputFile, "\t\t\t\tpc\tbp\tsp\tstack\n");
		fprintf(outputFile, "Initial values\t\t\t%d\t%d\t%d\n", PC, BP, SP);
		
		// Run the first command.
		fprintf(outputFile, "%d\t%s\t%d\t%d\t", inputArray[0].line, getBasicOPCode(inputArray[0].op), inputArray[0].l, inputArray[0].m);
		updateGlobals(inputArray[0].op, inputArray[0].l, inputArray[0].m);
		fprintf(outputFile, "%d\t%d\t%d\t", PC, BP, SP);

		// Print the stack.
		int barRequired = stackBar;
        int j;
		for (j = 1; j < SP + 1; j++){
				fprintf(outputFile, "%d ", stack[j]);
				if ((j) % incSize == 0 && barRequired){
					fprintf(outputFile, "| ");
					barRequired--;
				}
			}
		fprintf(outputFile, "\n");
		// Run the program until the stack pointer resets back to 0.
		do{
			fprintf(outputFile, "%d\t%s\t%d\t%d\t", inputArray[PC].line, getBasicOPCode(inputArray[PC].op), inputArray[PC].l, inputArray[PC].m);

			updateGlobals(inputArray[PC].op, inputArray[PC].l, inputArray[PC].m);

			fprintf(outputFile, "%d\t%d\t%d\t", PC, BP, SP);

			int barRequired = stackBar;
			for (j = 1; j < SP + 1; j++){
				fprintf(outputFile, "%d ", stack[j]);
				if ((j) % incSize == 0 && barRequired){
					fprintf(outputFile, "| ");
					barRequired--;
				}
			}
			fprintf(outputFile, "\n");

		} while (SP);
	}

	fclose(inputFile);
	fclose(outputFile);
}


int main() {
    
    struct instruction inputArray[MAX_CODE_LENGTH];
    
    getBasicAssembly(inputArray);
    
    getAdvancedAssembly(inputArray);
    
    return 0;
}

void nextPC(){
	PC++;
}

int BASE(int L, int base){
	int b1; //find base L levels down
	b1 = base;
	while (L > 0)
	{
		b1 = stack[b1 + 1];
		L--;
	}
	return b1;
}

void LIT(int L, int M){
	SP++;
	stack[SP] = M;
	nextPC();
}

void LOD(int L, int M){
	SP++;
	int base = BASE(L, BP);
	stack[SP] = stack[base + M];
	nextPC();
}

void STO(int L, int M){
	int base = BASE(L, BP);
	stack[base + M] = stack[SP];
	SP--;
	nextPC();
}

void INIT(){
    int i;
	for (i = 0; i < MAX_STACK_HEIGHT; i++){
		stack[i] = 0;
	}
}

void CAL(int L, int M){
	nextPC();
	int base = BASE(L, BP);
	stack[SP + 1] = 0;
	stack[SP + 2] = base;
	stack[SP + 3] = BP;
	stack[SP + 4] = PC;
	BP = SP + 1;
	PC = M;
	stackBar++;
}

void INC(int L, int M){
	SP = SP + M;
	incSize = M;
	nextPC();
}

void JMP(int L, int M){
	PC = M;
}

void JPC(int L, int M){
	if (stack[SP] == 0){
		PC = M;
	}
	SP--;
	nextPC();
}

void SIO(int L, int M){

	switch (M) {
	case 1:
		printf("\n%d\n", stack[SP]);
		SP--;
		nextPC();
		return;
	case 2:
		SP++;
		scanf("%d", &stack[SP]);
		nextPC();
		return;
	case 3:
		SP = 0;
		return;
	}
}

void OPR(int L, int M){

	switch (M) {
	case 0:
		SP = BP - 1;
		PC = stack[SP + 4];
		BP = stack[SP + 3];
		stackBar--;
		return;
	case 1:
		stack[SP] = -stack[SP];
		nextPC();
		return;
	case 2:
		SP--;
		stack[SP] = stack[SP] + stack[SP + 1];
		nextPC();
		return;
	case 3:
		SP--;
		stack[SP] = stack[SP] - stack[SP + 1];
		nextPC();
		return;
	case 4:
		SP--;
		stack[SP] = stack[SP] * stack[SP];
		nextPC();
		return;
	case 5:
		stack[SP] = stack[SP] / stack[SP];
		nextPC();
		return;
	case 6:
		stack[SP] = stack[SP] % 2;
		nextPC();
		return;
	case 7:
		SP--;
		stack[SP] = stack[SP] % stack[SP + 1];
		nextPC();
		return;
	case 8:
		SP--;
		stack[SP] = (stack[SP] == stack[SP + 1]);
		nextPC();
		return;
	case 9:
		SP--;
		stack[SP] = (stack[SP] != stack[SP + 1]);
		nextPC();
		return;
	case 10:
		SP--;
		stack[SP] = (stack[SP] < stack[SP + 1]);
		nextPC();
		return;
	case 11:
		SP--;
		stack[SP] = (stack[SP] <= stack[SP + 1]);
		return;
	case 12:
		SP--;
		stack[SP] = (stack[SP] > stack[SP + 1]);
		nextPC();
		return;
	case 13:
		SP--;
		stack[SP] = (stack[SP] >= stack[SP + 1]);
		nextPC();
		return;
	default:
		return;
	}
}