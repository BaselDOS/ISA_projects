#include <stdio.h>
#include <string.h>
#include <process.h>
#define MAX_LABELS 50
#define MAX_ROW 500
#define IMEM 1024
#define DMEM 4096
#define SECTORS 128
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c\n"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

void print_regout(FILE* fp_regout, int R[]);//function that prints the values stored in registers 2 to 15 at the end
void print_regout4trace(FILE* fp_regout, int R[], int pc, char* inst);// function that prints the instruction 
//and program Counter and the values stored in the registers before "applying "the instrustion/
void hwregtrac(FILE* fp_hwr, int cycle, char* type, char* name, int data);// function that prints the values stord in the I-O Registers
void cyclestimepc(FILE* fp_cycletime, int time, int pc);//function that prints the instruction total amount and the total and the amount of cycles. 
void writeleds(FILE* fp_ledo, int leds[8], int cycle);//function that prints the value of the leds to the file 
void printscreenstatus(FILE* fp_screen,int screen[288][352]);//function that prints the value of each pixel in the screen
void printscreenstatusBinary(FILE* fp_screen, int screen[288][352]);
int checkReg(char Reg);//function that decodes which register Reg represent
void checkIOReg(int IORegnim, char regWRname[]);//function that decodes which I-O Register regWRname represent
int isjump(int opcode);// function that check if we have a jump instruction or not
int isInOut(char* opcode);// function that deccode if we are in IN or OUT instruction-----------------------
void ReadOrWrite(char* opcode,char* typeWR);// function that decodes if we are reading writing from I-O register
int copyIMem(FILE* fp_Imem, char Imem[][MAX_ROW]);//function that initializes the Instruction memory array from the Instruction memory in file
int copyDMem(FILE* fp_Dmem, int Dmem[]);//function that initializes the Data memory array from the Data memory in file
int immneed(char rs, char rt, char rd);// function that decodes if we uses $imem or not
int opcodeNoImm(int opcode, int R[], int IOReg[], int rd, int rs, int rt, int Dmem[], int pcc, int screen[][352], char Disk[128][3], int clk[], int clk_count, FILE* fp_ledo, FILE* fp_hwREgtrace);
int opcodeImm(int opcode, int R[], int IOReg[], int rd, int rs, int rt, int Dmem[], int pcc, int imm, int screen[288][352],char Disk[128][3], int clk[], int clk_count, FILE* fp_ledo, FILE* fp_hwREgtrace);
int checkopcode(char opcode[]);//function that decodes which Opcode "opcode" represent
void fill_data(FILE* fp_out, int Dmem_counter, int Dmem[]);// function that print the data memory to the data memory out file
int MaxData(int Dmem[]);//function that takes an array of Data memory and return the maximom place in it with value that is bigger than zero
int CopyCLK(FILE* fp_irq2, int CLK[]);// function that initialize the CLK(irq2) array
void copyDisk(FILE* fp_Diskin, int Disk[]);//function that initializes the Disk array from the Disk in file
void fill_Disk(FILE* fp_out, int Disk[128][3]);// function that print the Disk data to the Diskout file
int irq2Setup(int clk[],int clk_count, int irq);// function that chek if irq2 in on or not


int irq2Setup(int clk[],int clk_count, int irq)
{
	int i =0 ;
	for (i; i <= clk_count; i++)if (clk[i] == irq) return 1; // if the clock cycle that we are in appears in irq2in file then we return 1 and zero else
	return 0;
}
int MaxData(int Dmem[])
{
	int MaxPlace = 0, i = 0;
	for (i; i < DMEM; i++)
	{
		if (Dmem[i] > 0) MaxPlace = i;
		else continue;
	}
	return MaxPlace;
}
int opcodeNoImm(int opcode, int R[], int IOReg[], int rd, int rs, int rt, int Dmem[],int pcc,int screen[][352],char Disk[128][3],int clk[],int clk_count, FILE* fp_ledo,FILE* fp_hwREgtrace)
{
	char IOregister[MAX_LABELS] = "";
	int pc = pcc;
	switch (opcode)
	{
	case(0):
		R[rd] = R[rs] + R[rt];
		break;
	case(1):
		R[rd] = R[rs] - R[rt];
		break;
	case(2):
		R[rd] = R[rs] & R[rt];
		break;
	case(3):
		R[rd] = R[rs] | R[rt];
		break;
	case(4):
		R[rd] = R[rs] ^ R[rt];
		break;
	case(5):
		R[rd] = R[rs] * R[rt];
		break;
	case(6):
		R[rd] = R[rs] << R[rt];
		break;
	case(7):
		R[rd] = R[rs] >> R[rt];
		break;
	case(8):
		R[rd] = R[rs] >> R[rt];
		break;
	case(9):
		if (R[rs] == R[rt]) pc = R[rd] - 1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
		break;
	case(10):
		if (R[rs] != R[rt]) pc = R[rd] - 1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
		break;
	case(11):
		if (R[rs] < R[rt]) pc = R[rd] - 1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
		break;
	case(12):
		if (R[rs] > R[rt]) pc = R[rd] - 1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
		break;
	case(13):
		if (R[rs] <= R[rt]) pc = R[rd] - 1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
		break;
	case(14):
		if (R[rs] >= R[rt]) pc = R[rd] - 1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
		break;
	case(15):
		//jalpc = *pc + 1;//ask
		R[15] = pc + 1;
		pc = R[rd] - 1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
		break;
	case(16):
		R[rd] = Dmem[R[rs] + R[rt]];
		break;
	case(17):
		Dmem[R[rs] + R[rt]] = R[rd];
		int r = Dmem[R[rs] + R[rt]];
		break;

	case(18):
		pc = IOReg[7] - 1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
		break;
	case(19):
		R[rd] = IOReg[R[rs] + R[rt]];
		if (R[rs] + R[rt]!=18)
		{
			R[rd] = IOReg[R[rs] + R[rt]];
		}
		else if (R[rs] + R[rt] == 14)
		{
			if (IOReg[14] == 1)
			{
				Dmem[IOReg[16] = Disk[IOReg[15]]];
				IOReg[14] = 0;
				IOReg[17] = 0;
				IOReg[4] = 1;
			}

		}
		
		checkIOReg(R[rd], IOregister);
		hwregtrac(fp_hwREgtrace, IOReg[8], "READ", IOregister, R[rs] + R[rt]);
		break;
	case(20):
		IOReg[R[rs] + R[rt]]=R[rd];
		if ((R[rs] + R[rt]) == 18)
		{
			screen[IOReg[20]][IOReg[19]] = IOReg[21];
			IOReg[R[rs] + R[rt]] = 0;
		}
		else if(R[rs] + R[rt] ==11)
		{
			if (IOReg[R[rs] + R[rt]] = 1)
			{
				if (IOReg[12] == IOReg[13])
				{
					IOReg[12] = 0;
					IOReg[3] = 1;
				}
				else IOReg[12]++;
			}
		}
		else if(R[rs] + R[rt] == 14)
		{
			if (IOReg[14] == 2)
			{
				Disk[IOReg[15] = Dmem[IOReg[16]]];
				IOReg[14] = 0;
				IOReg[17] = 0;
				IOReg[4] = 1;
			}
			
		}
		else if ((R[rs] + R[rt]) == 9)writeleds(fp_ledo, IOReg[9], IOReg[8]);
		checkIOReg(R[rs] + R[rt], IOregister);
		hwregtrac(fp_hwREgtrace, IOReg[8]+1, "WRITE", IOregister,R[rd]);
		break;
		case(21): 
		exit;
   
	}
	
	if (irq2Setup(clk, clk_count, IOReg[8]))
	{
		IOReg[2] = 1;
		IOReg[5] = 1;
	}

	return pc;
}
int opcodeImm(int opcode, int R[], int IOReg[], int rd, int rs, int rt, int Dmem[], int imm,int pcc,int screen[288][352],char Disk[128][3],int clk[],int clk_count,FILE* fp_ledo, FILE* fp_hwRegtrace)
{
	char IOregister[MAX_LABELS] = "";
	 int pc = pcc;
	if (isjump(opcode))
	{
		switch (opcode)
		{

		case(9):
			if (R[rs] == R[rt]) pc = R[rd]-1; //- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
			break;
		case(10):
			if (R[rs] != R[rt]) pc = R[rd]-1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
			break;
		case(11):
			if (R[rs] < R[rt]) pc = R[rd]-1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
			break;
		case(12):
			if (R[rs] > R[rt]) pc = R[rd]-1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
			break;
		case(13):
			if (R[rs] <= R[rt]) pc = R[rd]-1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
			break;
		case(14):
			if (R[rs] >= R[rt]) pc = R[rd]-1;//- appears here becase we are adding in the main function the pc by one (easier to write the simulator code)
			break;
		case(15):
			R[15] = pc+1;
			pc = R[rd]-1;
			break;
		case(16):
			R[rd] = Dmem[R[rs] + R[rt]];
			break;
		case(17):
			Dmem[R[rs] + R[rt]] = R[rd];
			int r = Dmem[R[rs] + R[rt]];
			break;
		case(19):
			R[rd] = IOReg[R[rs] + R[rt]];
			if (R[rs] + R[rt] != 18)
			{
				R[rd] = IOReg[R[rs] + R[rt]];
			}
			else if (R[rs] + R[rt] == 14)
			{
				if (IOReg[14] == 1)
				{
					Dmem[IOReg[16] = Disk[IOReg[15]]];
					IOReg[14] = 0;
					IOReg[17] = 0;
					IOReg[4] = 1;
				}

			}
			checkIOReg(R[rd], IOregister);
			hwregtrac(fp_hwRegtrace, IOReg[8]+1, "READ", IOregister, R[rs] + R[rt]);
			break;
		case(20):
			IOReg[R[rs] + R[rt]] = R[rd];
			if ((R[rs] + R[rt]) == 18)
			{
				screen[IOReg[20]][IOReg[19]] = IOReg[21];
				IOReg[R[rs] + R[rt]] = 0;
			}
			else if (R[rs] + R[rt] == 11)
			{
				if (IOReg[R[rs] + R[rt]] = 1)
				{
					if (IOReg[12] == IOReg[13])
					{
						IOReg[12] = 0;
						IOReg[3] = 1;
					}
					else IOReg[12]++;
				}
			}
			else if (R[rs] + R[rt] == 14)
			{
				if (IOReg[14] == 2)
				{
					Disk[IOReg[15] = Dmem[IOReg[16]]];
					IOReg[14] = 0;
					IOReg[17] = 0;
					IOReg[4] = 1;
				}
			}
			else if ((R[rs] + R[rt])==9)writeleds(fp_ledo,IOReg[9],IOReg[8]);
			checkIOReg(R[rs] + R[rt], IOregister);
			hwregtrac(fp_hwRegtrace, IOReg[8], "WRITE", IOregister, R[rd]);
			break;
			
		}
		
		if (irq2Setup(clk, clk_count, IOReg[8]))
		{
			IOReg[2] = 1;
			IOReg[5] = 1;
		}
		
		return pc;
	}
	else
	{
		opcodeNoImm(opcode, R, IOReg, rd, rs, rt, Dmem, pcc, screen, Disk, clk, clk_count, fp_ledo, fp_hwRegtrace);
		
	}
}
int CopyCLK(FILE* fp_irq2, int CLK[])
{
	int count = 0, data = 0;
	char word[MAX_ROW] = "";
	while ((fgets(word, MAX_ROW, fp_irq2) != NULL))
	{
		data = (int)strtol(word, NULL, 0);
		CLK[count] = data;
		count++;

	}
	return count - 1;
}
int immneed(char rs, char rt, char rd)
{
	if(rs == 1 || rt == 1 || rd == 1)return 1;
	return 0;
}
void writeleds(FILE* fp_ledo, int leds[8], int cycle)
{
	fprintf(fp_ledo, "%d %08x\n", cycle, leds);
}
void print_regout(FILE* fp_regout, int R[])
{
	for (int i = 2; i < 16; i++)
	{
		fprintf(fp_regout, "%08X\n", (R[i]));
	}
}
void print_regout4trace(FILE* fp_regout, int R[], int pc, char* inst)
{
	fprintf(fp_regout, "%03X %s %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x \n", pc, inst, R[0], R[1], R[2], R[3], R[4], R[5], R[6], R[7], R[8], R[9], R[10], R[11], R[12], R[13], R[14], R[15]);

}
void hwregtrac(FILE* fp_hwr, int cycle, char* type, char* name, int data)
{
	fprintf(fp_hwr, "%d %s %s %08X \n", cycle, type, name, data);
}
void cyclestimepc(FILE* fp_cycletime, int time, int pc)
{
	fprintf(fp_cycletime, "cycles time is:%d\n total amount of instructions is:%d\n", time, pc);
}
void printscreenstatus(FILE* fp_screen,int screen[288][352])
{
	int i = 0, j = 0;
	for (i;i<288;i++)
		for(j;j<352;j++)
			fprintf(fp_screen, "%02x\n", screen[i][j]);
}
void printscreenstatusBinary(FILE* fp_screen, int screen[288][352])
{

	int i = 0, j = 0;
	for (i; i < 288; i++)
		for (j; j < 352; j++)
		{
			fprintf(fp_screen, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY( screen[i][j]));
		}
}
int checkReg(char Reg)
{
	if (Reg=='0') return 0;
	else if (Reg == '1') return 1;
	else if (Reg == '2') return 2;
	else if (Reg == '3') return 3;
	else if (Reg == '4') return 4;
	else if (Reg =='5') return 5;
	else if (Reg == '6') return 6;
	else if (Reg == '7') return 7;
	else if (Reg == '8') return 8;
	else if (Reg == '9') return 9;
	else if (Reg == 'A') return 10;
	else if (Reg == 'B') return 11;
	else if (Reg == 'c') return 12;
	else if (Reg == 'D') return 13;
	else if (Reg == 'E') return 14;
	else if (Reg == 'F') return 15;

}
void checkIOReg(int IORegnim, char regWRname[])
{
	if (IORegnim == 0) strcpy(regWRname, "irq0enable");
	else if (IORegnim == 1) strcpy(regWRname, "irq1enable");
	else if (IORegnim == 2) strcpy(regWRname, "irq2enable");
	else if (IORegnim == 3)  strcpy(regWRname, "irq0status");
	else if (IORegnim == 4) strcpy(regWRname, "irq1status");
	else if (IORegnim == 5) strcpy(regWRname, "irq2status");
	else if (IORegnim == 6) strcpy(regWRname, "irqhandler");
	else if (IORegnim == 7) strcpy(regWRname, "irqreturn");
	else if (IORegnim == 8) strcpy(regWRname, "clks");
	else if (IORegnim == 9) strcpy(regWRname, "leds");
	else if (IORegnim == 10) strcpy(regWRname, "reserved");
	else if (IORegnim == 11) strcpy(regWRname, "timerenable");
	else if (IORegnim == 12) strcpy(regWRname, "timercurrent");
	else if (IORegnim == 13) strcpy(regWRname, "timermax");
	else if (IORegnim == 14) strcpy(regWRname, "diskcmd");
	else if (IORegnim == 15) strcpy(regWRname, "disksector");
	else if (IORegnim == 16) strcpy(regWRname, "diskbuffer");
	else if (IORegnim == 17) strcpy(regWRname, "diskstatus");
	else if (IORegnim == 18) strcpy(regWRname, "monitorcmd");
	else if (IORegnim == 19) strcpy(regWRname, "monitorx");
	else if (IORegnim == 20)strcpy(regWRname, "monitory");
	else if (IORegnim == 21) strcpy(regWRname, "monitordata");

}
int checkopcode(char opcode[])
{
	if (opcode[0]=='0'&&opcode[1] == '0') return 0;
	else if (opcode[0] == '0'&&opcode[1] == '1') return 1;
	else if (opcode[0] == '0'&&opcode[1] == '2') return 2;
	else if (opcode[0] == '0'&&opcode[1] == '3') return 3;
	else if (opcode[0] == '0'&&opcode[1] == '4') return 4;
	else if (opcode[0] == '0'&&opcode[1] == '5') return 5;
	else if (opcode[0] == '0'&&opcode[1] == '6') return 6;
	else if (opcode[0] == '0'&&opcode[1] == '7') return 7;
	else if (opcode[0] == '0'&&opcode[1] == '8') return 8;
	else if (opcode[0] == '0'&&opcode[1] == '9') return 9;
	else if (opcode[0] == '0'&&opcode[1] == 'A') return 10;
	else if (opcode[0] == '0'&&opcode[1] == 'B') return 11;
	else if (opcode[0] == '0'&&opcode[1] == 'C') return 12;
	else if (opcode[0] == '0'&&opcode[1] == 'D') return 13;
	else if (opcode[0] == '0'&&opcode[1] == 'E') return 14;
	else if (opcode[0] == '0'&&opcode[1] == 'F') return 15;
	else if (opcode[0] == '1'&&opcode[1] == '0') return 16;
	else if (opcode[0] == '1'&&opcode[1] == '1') return 17;
	else if (opcode[0] == '1'&&opcode[1] == '2') return 18;
	else if (opcode[0] == '1'&&opcode[1] == '3') return 19;
	else if (opcode[0] == '1'&&opcode[1] == '4') return 20;
	else if (opcode[0] == '1'&&opcode[1] == '5') return 21;



}
int isInOut(char* opcode)
{
	if (strcmp(opcode, "20") == 0 || strcmp(opcode, "19") == 0 ) return 1;
	return 0;
}
int isjump(int opcode)
{
	if (opcode>=9&&opcode<=15)
		return 1;
	else return 0;
}
void ReadOrWrite(char* opcode,char* typeRW)
{
	char* type = " ";
	if (strcmp(opcode, "in") == 0)strcpy(type, "READ");
	else strcpy(type, "WRITE");
	*typeRW = *type;
}
int copyDMem(FILE* fp_Dmem, int Dmem[])
{
	int count = 0, data = 0;
	char word[MAX_ROW] = "";
	while ((fgets(word, MAX_ROW, fp_Dmem) != NULL))
	{
		data = (int)strtol(word, NULL, 16);
		Dmem[count] = data;
		count++;
		
	}
	return count-1;
}
int copyIMem(FILE* fp_Imem, char Imem[][MAX_ROW])
{
	int count = 0;
	char* word [MAX_ROW]={""};
	while ((fgets(word, MAX_ROW, fp_Imem) != NULL))
	{
	
		
		strcpy(Imem[count], word);
		Imem[count][5] = 0;
		count++;

	}
	return count-1;
}
void fill_data(FILE* fp_out, int Dmem_counter, int Dmem[])
{
	int i = 0;
	while (i <= MaxData(Dmem))
	{
		fprintf(fp_out, "%08x\n", Dmem[i]);
		i++;
	}
}
void fill_Disk(FILE* fp_out,int Disk[128][3])
{
	int i = 0;
	while (i < 128)
	{
		fprintf(fp_out, "%03x\n", Disk[i]);
		i++;
	}
}
void copyDisk(FILE* fp_Diskin, int Disk[])
{
	int count = 0, data = 0;
	char word[3] = "";
	while ((fgets(word, 3, fp_Diskin) != NULL)&&count<128)
	{
		data = (int)strtol(word, NULL, 16);
		Disk[count] = data;
		count++;

	}
	
}
int main(int argc, char* argv[])
{
	//initilization
	FILE* fp_imemin;
	FILE* fp_dmemin;
	FILE* fp_diskin;
	FILE* fp_irq2in;
	FILE* fp_dmemout;
	FILE* fp_regout;
	FILE* fp_trace;
	FILE* fp_hwreg;
	FILE* fp_cycle;
	FILE* fp_ledo;
	FILE* fp_moni;
	FILE* fp_diskout;
	FILE* fp_moni_Binary;
	int pc = 0;
	int R[16] = { 0x00000000 };
	int screen[288][352] = { 0 };
	char Imem[IMEM][MAX_ROW] = { "" };
	int ImemCount = 0;
	int Dmem[DMEM] = { 0 };
	int DmemCount = 0;
	int IORegister[22] = { 0x00000000 };

	int opcode = 0;
	int rd = 0;
	int rs = 0;
	int rt = 0;
	int imm = 0;
	int pc_count = 0;
	int CLK[MAX_ROW] = { 0 };
	int ClkCount = 0;
	int irq = 0;
	char opcodenum[2] = "";
	char disk[128][9] = { "" };
	char negativenumber[500] = " ";
	//opening files
	fp_imemin = fopen(argv[1], "r");
	if (fp_imemin == NULL)
	{
		printf("Imemin input file didn't open\n");
	}
	fp_dmemin = fopen(argv[2], "r");
	if (fp_dmemin == NULL)
	{
		printf("Dmemin input file didn't open\n");
	}
	fp_diskin = fopen(argv[3], "r");
	if (fp_diskin == NULL)
	{
		printf("Diskin input file didn't open\n");


	}
	fp_irq2in = fopen(argv[4], "r");
	if (fp_irq2in == NULL)
	{
		printf("Irq2in input file didn't open\n");


	}
	fp_dmemout = fopen(argv[5], "w");
	if (fp_dmemout == NULL)
	{
		printf("Dmemout output file didn't open\n");


	}
	fp_regout = fopen(argv[6], "w");
	if (fp_regout == NULL)
	{
		printf("Regout output file didn't open\n");


	}
	fp_trace = fopen(argv[7], "w");
	if (fp_trace == NULL)
	{
		printf("Trace output file didn't open\n");


	}
	fp_hwreg = fopen(argv[8], "w");
	if (fp_hwreg == NULL)
	{
		printf("Hwregtrace output file didn't open\n");


	}
	fp_cycle = fopen(argv[9], "w");
	if (fp_cycle == NULL)
	{
		printf("Cycles output file didn't open\n");


	}
	fp_ledo = fopen(argv[10], "w");
	if (fp_ledo == NULL)
	{
		printf("Leds output file didn't open\n");


	}
	fp_moni = fopen(argv[11], "w");
	if (fp_moni == NULL)
	{
		printf("Monitor output file didn't open\n");


	}
	fp_moni_Binary = fopen(argv[12], "w");
	if (fp_moni == NULL)
	{
		printf("Monitor.yuv output file didn't open\n");


	}
	fp_diskout = fopen(argv[13], "w");
	if (fp_diskout == NULL)
	{
		printf("Diskout output file didn't open\n");

	}

	//filling memory and irq2 arrays	
	DmemCount = copyDMem(fp_dmemin, Dmem);
	ImemCount= copyIMem(fp_imemin, Imem);
	ClkCount = CopyCLK(fp_irq2in, CLK);
	while (pc<=ImemCount)
	{
		irq = (IORegister[0] && IORegister[3]) || (IORegister[1] && IORegister[4]) || (IORegister[2] && IORegister[5]);//if we have "pseeka"
		if (irq)
		{// jump to irqhandler
			IORegister[7] = pc;
			pc = IORegister[6];
			if (IORegister[0] & IORegister[3])IORegister[3] = 0;
			else if (IORegister[1] & IORegister[4]) IORegister[4] = 0;
			else
			{
				IORegister[5] = 0;
				IORegister[2] = 0;
			}

		}

		imm = 0;
		strncpy(opcodenum, Imem[pc], 2);
		opcode = checkopcode(opcodenum);
		print_regout4trace(fp_trace, R, pc, Imem[pc]);
		pc_count++;
		if (opcode == 21)//if opcode is halt the we have finished
		{
			break;
		}
		else
		{
			rd = checkReg(Imem[pc][2]);
			rs = checkReg(Imem[pc][3]);
			rt = checkReg(Imem[pc][4]);
			if (immneed(rs, rt, rd))
			{
				pc++;	
				if (Imem[pc][0] == 'F')//if we have a negative number we apply the oposit 2"s compliment algorithem
				{
					strcpy(negativenumber, Imem[pc]);
					negativenumber[0] = '0';
					imm = (int)strtol(negativenumber, NULL, 16);
					imm -= 0x0001;
					imm = -(imm ^ 0xFFFF);
				}

				else imm = (int)strtol(Imem[pc], NULL, 16);

				if (rs == 1) R[rs] = imm;
				else if (rt == 1)R[rt] = imm;
				else if (rd == 1)R[rd] = imm;
				{
					pc = opcodeImm(opcode, R, IORegister, rd, rs, rt, Dmem, imm, pc,screen,disk,CLK,ClkCount, fp_ledo,fp_hwreg);
					IORegister[8] += 2;
					
				}
			}
			else
			{
				pc = opcodeNoImm(opcode, R, IORegister, rd, rs, rt, Dmem, pc,screen,disk,CLK,ClkCount, fp_ledo,fp_hwreg);
				IORegister[8]++;
				
			}

		}
	
		pc++;
		
		
	}
	// printing to files that requier printing at the end
	fill_data(fp_dmemout, DmemCount, Dmem);
	print_regout(fp_regout, R);
	printscreenstatus(fp_moni, screen);
	printscreenstatusBinary(fp_moni_Binary, screen);
	cyclestimepc(fp_cycle, IORegister[8], pc_count);
	fill_Disk(fp_diskout, disk);
	//closing files
	fclose(fp_imemin);
	fclose(fp_dmemin);
	fclose(fp_diskin);
	fclose(fp_irq2in);
	fclose(fp_dmemout);
	fclose(fp_regout);
	fclose(fp_trace);
	fclose(fp_hwreg);
	fclose(fp_cycle);
	fclose(fp_ledo);
	fclose(fp_moni);
	fclose(fp_moni_Binary);
	fclose(fp_diskout);
	return 1;

}
