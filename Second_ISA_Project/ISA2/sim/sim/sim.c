#include <stdio.h>
#include<string.h>
#include<process.h>
#define Max_ROW 500
#define MAX_LABEL_NAME 50
#define IDMEM 4096
#define SECTORS_AMMOUNT 128
#define SECTOR_SIZE 128
#define DISK_LINES 16384
#define BUFFERSIZE 256


/* ________________________________________________________Declarations______________________________________________________________________________________*/

int Inisialize_Instruction_Memory(FILE* fp_imemin,char Imem[][Max_ROW]);
int Inisialize_Data_Memory(FILE* fp_dmemin, int DataMemory[]);
int Inisialize_Irq2(FILE* fp_irq2in, int irq[]);
void Inisilaize_Disk(FILE *fp_diskin, int Disk[SECTORS_AMMOUNT][SECTOR_SIZE]);
int Decode_Regester(char Reg);
void Decode_IORegester(int IOReg_Check, char IOReg[]);
int Decode_Opcode(char Opcode[]);
void ReadOrWrite_Opcode(char* Opcode, char* ReaadOrWrite);
void UpdateDisk_For_ReadOrWrite(int *Disk_Bussy_Time, int IORegesters[], int Disk[128][128], int Data_Memory[4096]);
int Top_Data_Address(int DataMemory[]);
void update_Irq0(int IORegesters[]);
void Update_Data_Memory(FILE* fp_Dmemout, int DataMemory[]);
void Update_Leds(FILE* fp_leds, int leds[8], int cycle);
void Update_Regout(FILE* fp_Regout, int Regester[]);
void Update_Trace(FILE* fp_Trace, int Regester[], int Pc, char* instruction);
void Update_HwRegTrace(FILE* fp_HwRegTrace, int Cycle, char* ReadOrWrite, char* Name, int Data);
void Update_Cycles(FILE* fp_Cycles, int Cycles_Ammount);
void Update_Monitor(FILE* fp_Monitor, FILE* fp_Monitor_Yuv, int Buffer[256][256]);
void Update_Disk(FILE* fp_Diskout, int Disk[128][128]);
void Update_Display7seg(FILE* fp_Display7seg, int Cycle, int Display7[8]);
int Instruction_Excute(int Opcode, int Regester[], int IORegester[], int rd, int rs, int rt, int DataMemory[], int imm1, int imm2, int Pc, int Buffer[256][256], char Disk[128][128], int Irq2[], int Irq2EnableTimes, FILE* fp_leds, FILE* fp_HwRegtrace, FILE* fp_Display7seg, int* Bussy, int Clk, int Disk_Ammount, FILE* fp_Trace, char* Instruction);

/* ________________________________________________________Definitions______________________________________________________________________________________*/

int Inisialize_Instruction_Memory(FILE* fp_imemin, char InstructionMemory[][Max_ROW])// function that inisilaize the instruction memory.
{
	int Instruction_Ammount = 0;
	char* Inst[Max_ROW] = { "" };
	while ((fgets(Inst, Max_ROW, fp_imemin) != NULL))//read a row.
	{
		strcpy(InstructionMemory[Instruction_Ammount], Inst);
		InstructionMemory[Instruction_Ammount][12] = 0;
		Instruction_Ammount++;

	}
	return Instruction_Ammount - 1;
}
int Inisialize_Data_Memory(FILE* fp_dmemin, int DataMemory[])// function that inisilaize the data memory.
{
	int Data_Ammount = 0;
	int Data = 0;
	char DataCopy[Max_ROW] = "";
	while ((fgets(DataCopy, Max_ROW, fp_dmemin) != NULL))
	{
		Data = (int)strtol(DataCopy, NULL, 16);
		DataMemory[Data_Ammount] = Data;
		Data_Ammount++;

	}
	return Data_Ammount - 1;
}
int Inisialize_Irq2(FILE* fp_irq2in, int irq2[])//function that inisilize Irq2in.
{
	int Irq2EnableTimes = 0;
	char Clk[Max_ROW] = "";

	while (fgets(Clk,Max_ROW,fp_irq2in) != NULL)
	{
		irq2[Irq2EnableTimes] = (int)strtol(Clk, NULL, 0);
		Irq2EnableTimes++;
	}
	return Irq2EnableTimes ;
}
void Inisilaize_Disk(FILE *fp_diskin, int Disk[SECTORS_AMMOUNT][SECTOR_SIZE])//function that inisilize the Disk memory.
{
	int i = 0; int j = 0;
	int copy = 0;
	while (!feof(fp_diskin) && (1 == fscanf(fp_diskin, "%X", &copy)))
	{
		if (j == SECTOR_SIZE) {
			j = 0;
			i++;
		}
		Disk[i][j] = (copy & 0xFFFFFFFF);
		j++;
	}
}
int Decode_Regester(char Reg)//function that marks the registers by there Hex number.
{
	if (Reg == '0') return 0;
	else if (Reg == '1') return 1;
	else if (Reg == '2') return 2;
	else if (Reg == '3') return 3;
	else if (Reg == '4') return 4;
	else if (Reg == '5') return 5;
	else if (Reg == '6') return 6;
	else if (Reg == '7') return 7;
	else if (Reg == '8') return 8;
	else if (Reg == '9') return 9;
	else if (Reg == 'A') return 10;
	else if (Reg == 'B') return 11;
	else if (Reg == 'C') return 12;
	else if (Reg == 'D') return 13;
	else if (Reg == 'E') return 14;
	else if (Reg == 'F') return 15;
}
void Decode_IORegester(int IOReg_Check, char IOReg[])// function that finds and returns the Input or Output register name by its number as shwn in the requerment sheet.
{
	
	if (IOReg_Check == 0) strcpy(IOReg, "irq0enable");
	else if (IOReg_Check == 1) strcpy(IOReg, "irq1enable");
	else if (IOReg_Check == 2) strcpy(IOReg, "irq2enable");
	else if (IOReg_Check == 3)  strcpy(IOReg, "irq0status");
	else if (IOReg_Check == 4) strcpy(IOReg, "irq1status");
	else if (IOReg_Check == 5) strcpy(IOReg, "irq2status");
	else if (IOReg_Check == 6) strcpy(IOReg, "irqhandler");
	else if (IOReg_Check == 7) strcpy(IOReg, "irqreturn");
	else if (IOReg_Check == 8) strcpy(IOReg, "clks");
	else if (IOReg_Check == 9) strcpy(IOReg, "leds");
	else if (IOReg_Check == 10) strcpy(IOReg, "display7seg");
	else if (IOReg_Check == 11) strcpy(IOReg, "timerenable");
	else if (IOReg_Check == 12) strcpy(IOReg, "timercurrent");
	else if (IOReg_Check == 13) strcpy(IOReg, "timermax");
	else if (IOReg_Check == 14) strcpy(IOReg, "diskcmd");
	else if (IOReg_Check == 15) strcpy(IOReg, "disksector");
	else if (IOReg_Check == 16) strcpy(IOReg, "diskbuffer");
	else if (IOReg_Check == 17) strcpy(IOReg, "diskstatus");
	else if ((IOReg_Check == 18)|| (IOReg == 19)) strcpy(IOReg, "reserved");
	else if (IOReg_Check == 20) strcpy(IOReg, "monitoraddr");
	else if (IOReg_Check == 21)strcpy(IOReg, "monitordata");
	else if (IOReg_Check == 22) strcpy(IOReg, "monitorcmd");

}
int Decode_Opcode(char Opcode[])//function that marks the opcodes by there Hex number as shown in the requerment sheet.
{
	if (Opcode[0] == '0'&&Opcode[1] == '0') return 0;
	else if (Opcode[0] == '0'&&Opcode[1] == '1') return 1;
	else if (Opcode[0] == '0'&&Opcode[1] == '2') return 2;
	else if (Opcode[0] == '0'&&Opcode[1] == '3') return 3;
	else if (Opcode[0] == '0'&&Opcode[1] == '4') return 4;
	else if (Opcode[0] == '0'&&Opcode[1] == '5') return 5;
	else if (Opcode[0] == '0'&&Opcode[1] == '6') return 6;
	else if (Opcode[0] == '0'&&Opcode[1] == '7') return 7;
	else if (Opcode[0] == '0'&&Opcode[1] == '8') return 8;
	else if (Opcode[0] == '0'&&Opcode[1] == '9') return 9;
	else if (Opcode[0] == '0'&&Opcode[1] == 'A') return 10;
	else if (Opcode[0] == '0'&&Opcode[1] == 'B') return 11;
	else if (Opcode[0] == '0'&&Opcode[1] == 'C') return 12;
	else if (Opcode[0] == '0'&&Opcode[1] == 'D') return 13;
	else if (Opcode[0] == '0'&&Opcode[1] == 'E') return 14;
	else if (Opcode[0] == '0'&&Opcode[1] == 'F') return 15;
	else if (Opcode[0] == '1'&&Opcode[1] == '0') return 16;
	else if (Opcode[0] == '1'&&Opcode[1] == '1') return 17;
	else if (Opcode[0] == '1'&&Opcode[1] == '2') return 18;
	else if (Opcode[0] == '1'&&Opcode[1] == '3') return 19;
}
void ReadOrWrite_Opcode(char* Opcode, char* ReaadOrWrite)// function that decides if we are dealing with a read or write situation.
{
	char* type = " ";
	if (strcmp(Opcode, "in") == 0)strcpy(type, "READ");
	else strcpy(type, "WRITE");
	*ReaadOrWrite = *type;
}
void UpdateDisk_For_ReadOrWrite(int *Disk_Bussy_Time,int IORegesters[],int Disk[128][128],int Data_Memory[4096])// function that update the disk.
{
	if (IORegesters[17] == 0)
	{							// disk is not bussy situation
		*Disk_Bussy_Time = 0;
		if (IORegesters[14] == 0);
		else
		{
			IORegesters[17] = 1;
			if (IORegesters[14] == 2) {						// writing to the disk situation.
				for (int i = 0; i < 128; i++)		// copy sector from Data memory to the disk.
					Disk[IORegesters[15]][i] = Data_Memory[IORegesters[16] + i];
			}
			else if (IORegesters[14] == 1) {				// reading from the disk situation.
				for (int i = 0; i < 128; i++)		// copy sector from disk to data memory. 
					Data_Memory[IORegesters[16] + i] = Disk[IORegesters[15]][i];
			}
		}
	}
	 if (IORegesters[17] == 1) 
	 {					// disk is bussy situation.
		*Disk_Bussy_Time++;// counting the cycles needed to finish the work.
	 }
	if (*Disk_Bussy_Time == 1024)
	{							// finished the work situation.
		*Disk_Bussy_Time = 0;
		IORegesters[17] = 0;							// free diskstatus.
		IORegesters[4] = 1;								// irq1status = 1.
		IORegesters[14] = 0;							// returning diskcmd back to zero.
	}
		
}
int Top_Data_Address(int DataMemory[])//function that findes and return the farest address that contain a non zero data.
{
	int TopAddress = 0, i = 0;
	for (i; i < IDMEM; i++)
	{
		if (DataMemory[i] > 0) TopAddress = i;
		else continue;
	}
	return TopAddress;
}
void update_Irq0(int IORegesters[])// function that updates the timer interupt .
{
	if (IORegesters[11] == 1)
	{								// if timerenable = 1.
		if (IORegesters[12] < IORegesters[13])// if timercurrent < timermax we add one to the current time. 
		{	IORegesters[12]++;						
			if (IORegesters[12] == IORegesters[13])         // if if we reached the maximum time allowed.
			{
				IORegesters[3] = 1;							//	trigger irq0status.
				IORegesters[12] = 0;						//  change timercurrent back to zero.
			}
		}
	}
}
void Update_Data_Memory(FILE* fp_Dmemout, int DataMemory[])//function that updates the data memory.
{
	int i = 0;
	while (i <= Top_Data_Address(DataMemory))
	{
		fprintf(fp_Dmemout, "%08x\n", DataMemory[i]);// 8 hex digit printing.
		i++;
	}
}
void Update_Leds(FILE* fp_leds, int leds[8], int cycle)// function that updates the Leds status.
{
	fprintf(fp_leds, "%d %08x\n", cycle+1, leds);//8 hex digit printing.
}
void Update_Regout(FILE* fp_Regout, int Regester[])//function that updates the Regout file by the values of the regesters.
{
	int i = 3;
	for (i; i < 16; i++)
	{
		fprintf(fp_Regout, "%08X\n", (Regester[i]));
	}
}
void Update_Trace(FILE* fp_Trace, int Regester[], int Pc, char* instruction)// function that updates the trace file.
{
	fprintf(fp_Trace, "%03X %s %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x \n", Pc, instruction, Regester[0], Regester[1], Regester[2], Regester[3], Regester[4], Regester[5], Regester[6], Regester[7], Regester[8], Regester[9], Regester[10], Regester[11], Regester[12], Regester[13], Regester[14], Regester[15]);

}
void Update_HwRegTrace(FILE* fp_HwRegTrace, int Cycle, char* ReadOrWrite, char* Name, int Data)// function that updates Hwregtrace file.
{
	fprintf(fp_HwRegTrace, "%d %s %s %08X \n", Cycle, ReadOrWrite, Name, Data);
}
void Update_Cycles(FILE* fp_Cycles, int Cycles_Ammount)// function that updates the cycle file by tha ammount of cycles needed to finish the jop.
{
	fprintf(fp_Cycles, "Total cycles ammount nedded is: %d [Cycles]\n ", Cycles_Ammount);
}
void Update_Monitor(FILE* fp_Monitor,FILE* fp_Monitor_Yuv ,int Buffer[256][256])// function that updates both monitor files by the screen status .
{
	int i = 0, j = 0;
	for (i; i < 256; i++)
		for (j; j < 256; j++)
			fprintf(fp_Monitor, "%02X\n", Buffer[j][i] *255);
	        fprintf(fp_Monitor_Yuv, "%c", Buffer[j][i]);
}
void Update_Disk(FILE* fp_Diskout,int Disk[128][128])// function that updates the disk .
{
	for (int i = 0; i < 128; i++) 
	{
		for (int j = 0; j < 128; j++) 
		{
			fprintf(fp_Diskout, "%08X\n", Disk[i][j]);
		}
	}
}
void Update_Display7seg(FILE* fp_Display7seg, int Cycle, int Display7[8])// function that updates the display7seg file .
{
	fprintf(fp_Display7seg, "%d %08x\n", Cycle, Display7);
}
int Instruction_Excute(int Opcode, int Regester[], int IORegester[], int rd, int rs, int rt, int DataMemory[], int imm1, int imm2, int Pc, int Buffer[256][256], char Disk[128][128], int Irq2[], int Irq2EnableTimes, FILE* fp_leds, FILE* fp_HwRegtrace, FILE* fp_Display7seg, int* Bussy, int Clk,int Disk_Ammount,FILE* fp_Trace,char* Instruction)// function that excute the current instruction.
{
	int Program_Counter = Pc; 
	char IORegName[Max_ROW]="";
	Update_Trace(fp_Trace, Regester, Program_Counter, Instruction);// update the trace file before excuting the instruction.
	switch (Opcode)
	{
	case(0):
		Regester[rd] = Regester[rs] + Regester[rt];//add case.

		break;
	case(1):
		Regester[rd] = Regester[rs] - Regester[rt];//sub case.
		break;
	case(2):
		Regester[rd] = Regester[rs] & Regester[rt];//and case.
		break;
	case(3):
		Regester[rd] = Regester[rs] | Regester[rt];//or case.
		break;
	case(4):
		Regester[rd] = Regester[rs] << Regester[rt];//sll case.
		break;
	case(5):
		Regester[rd] = Regester[rs] >> Regester[rt];//sra case.
		break;
	case(6):
		Regester[rd] = Regester[rs] >> Regester[rt];//srl case.
		break;
	case(7):
		if (Regester[rs] == Regester[rt]) Program_Counter = Regester[rd]-1;//beq case.
		break;
	case(8):
		if (Regester[rs] != Regester[rt]) Program_Counter = Regester[rd]-1;//bne case.
		break;
	case(9):
		if (Regester[rs] < Regester[rt]) Program_Counter = Regester[rd]-1;//blt case.
		break;
	case(10):
		if (Regester[rs] > Regester[rt]) Program_Counter = Regester[rd]-1;//bgt case.
		break;
	case(11):
		if (Regester[rs] <= Regester[rt]) Program_Counter = Regester[rd]-1;//ble case.
		break;
	case(12):
		if (Regester[rs] >= Regester[rt]) Program_Counter = Regester[rd]-1;//bge case.
		break;
	case(13): //jal case.
		if ((Regester[rd] >= 0) && (Regester[rd] < 4096))// checking if the address is llegal.
		{
			Regester[15] = (Program_Counter + 1) ;
			Program_Counter = Regester[rd]-1;
			
		}
		else 
		{
			printf("jal can't jump to this address\n Please enter a legal address and try again later\n! ");//illegal address situation.
		}
		break;
	case(14)://lw case.
		if ((Regester[rs] + Regester[rt] >= 4096) || (Regester[rs] + Regester[rt] <0))// checking if the address is llegal.
		{
			printf("Illegal address error\nPlease enter a illegal address and try again later\n");//illegal address situation.
		}
		else
		{
			Regester[rd] = DataMemory[Regester[rs] + Regester[rt]];//copying fro the data memory.
		}
		break;
	case(15)://sw case.
		if ((Regester[rs] + Regester[rt] >= 4096) || (Regester[rs] + Regester[rt] < 0))// checking if the address is llegal.
		{
			printf("Illegal address error\nPlease enter a illegal address and try again later\n");//illegal address situation.
		}
		else
		{
			DataMemory[Regester[rs] + Regester[rt]] = Regester[rd] ;//storing to the data memory.
		}
		break;
	case(16)://reti case.
		Program_Counter = IORegester[7]; //setting Program counter to the irqreturn address.
		*Bussy = 0;// updating bussy to zero -finish current interupt.
		break;
	case(17):// in case.
		Regester[rd] = IORegester[Regester[rs] + Regester[rt]];
		Decode_IORegester(Regester[rs] + Regester[rt], IORegName);
		Update_HwRegTrace(fp_HwRegtrace, Clk, "READ", IORegName, IORegester[Regester[rs] + Regester[rt]]);//updating the HWregtrace file.
		if (Regester[rs] + Regester[rt] == 22) // if we are trying to update the monitorcmd check.
			IORegester[22] = 0;//update the monitorcmd to zero.
		break;
	case(18)://out situation.
		if ((Regester[rs] + Regester[rt] >= 0) && (Regester[rs] + Regester[rt] <= 22))	// llegal address check
		{
			int OldIORegester_Info = IORegester[Regester[rs] + Regester[rt]];		// saving old address
			IORegester[Regester[rs] + Regester[rt]] = Regester[rd];
			Decode_IORegester(Regester[rs] + Regester[rt], IORegName);
			Update_HwRegTrace(fp_HwRegtrace, Clk, "WRITE", IORegName, IORegester[Regester[rs] + Regester[rt]]);
			if (Regester[rs] + Regester[rt] == 9)						// if we are trying to update the leds
			{
				if (OldIORegester_Info != IORegester[Regester[rs] + Regester[rt]])		// leds updated check.
				{
					Update_Leds(fp_leds, IORegester[9], Clk);
				}
			}
			if (IORegester[22] == 1)				// if we are trying to update the screen buffer- write pixel to monitor
			{
				Buffer[IORegester[19]][IORegester[20]] = IORegester[21] & 0xFF; // copying the monitordata into frame buufer .
				IORegester[22] = 0;				//finishing jop flag- 
			}
			if (Regester[rs] + Regester[rt] == 10)						// if we are trying to update the display7seg status.
			{
				if (OldIORegester_Info != IORegester[Regester[rs] + Regester[rt]])		// if the display7seg status changed check.
				{
					Update_Display7seg(fp_Display7seg, Clk, IORegester[10]);
				}
			}
			
		}
		else
		{
			printf("Illegal IORegester error\nPlease try again later\n");// illegal input or output register error.
		}
		Regester[rs] = 0;
		Regester[rd] = 0;
		Regester[rd] = 0;
		break;
	case(19):
		Program_Counter = -2;
		break;
	default:
		Program_Counter = -2;
		break;
	}
	
	update_Irq0(IORegester);//updating the irq0 interrupt.
	UpdateDisk_For_ReadOrWrite(&Bussy, IORegester, Disk, DataMemory);//updating the disk for reading or writing
	return Program_Counter;
}

/*_____________________________________________________________Main Function__________________________________________________________________________*/

int main(int argc, char* argv[])
{
	//inisilizations
	FILE* fp_Imemin;
	FILE* fp_Dmemin;
	FILE* fp_Irq2in;
	FILE* fp_Diskin;
	FILE* fp_Dmemout;
	FILE* fp_Regout;
	FILE* fp_Trace;
	FILE* fp_Hwregtrace;
	FILE* fp_Cycles;
	FILE* fp_Leds;
	FILE* fp_Display7seg;
	FILE* fp_Diskout;
	FILE* fp_Monitor;
	FILE* fp_MonitorBinary;

	char InstructionsMemory[Max_ROW][Max_ROW] = { "" };
	int Instructions_Ammount = 0;
	int Data_Memory[IDMEM] = { 0 };
	int Data_Ammount = 0, Disk_Bussy = 0;
	int Irq2[Max_ROW] = { 0 };
	int Irq2_EnableTimes = 0, Irq_all = 0, Current_Irq2=0,IrqStep=0;
	int Disk_Content[SECTORS_AMMOUNT][SECTOR_SIZE] = { 0 };
	int Disk_Ammount = 0;
	int Bussy = 0;
	int Clock = 0;
	int Buffer[256][256] = { 0 };
	int Regesters[16] = { 0 };
	int IORegesters[12] = { 0 };
	char OpcodeNumber[2] = "";
	int Program_counter = 0,Clk=0,rd=0,rs=0,rt=0,Opcode=0;
	// parameters to help us to deal with the immediats numbers we have.
	char Number_Sign = "";
	char imm11[3] = " ", imm22[3] = " ";
	char imm1[2] = " ", imm2[2] = " ";
	int imm1_Dec = 0, imm2_Dec = 0;
	//opening files.
	fp_Imemin = fopen(argv[1], "r");
	if (fp_Imemin == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Instruction memory input file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Dmemin = fopen(argv[2], "r");
	if (fp_Dmemin == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Data memory input file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Diskin = fopen(argv[3], "r");
	if (fp_Diskin == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Disk input file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Irq2in = fopen(argv[4], "r");
	if (fp_Irq2in == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Irq2in input file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Dmemout = fopen(argv[5], "w");
	if (fp_Dmemout == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Data Memory output file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Regout = fopen(argv[6], "w");
	if (fp_Regout == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Regout output file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Trace = fopen(argv[7], "w");
	if (fp_Trace == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the fp_Trace output file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Hwregtrace = fopen(argv[8], "w");
	if (fp_Hwregtrace == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Hwregtrace output file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Cycles = fopen(argv[9], "w");
	if (fp_Cycles == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Cycles output file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Leds = fopen(argv[10], "w");
	if (fp_Leds == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Leds output file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Display7seg = fopen(argv[11], "w");
	if (fp_Display7seg == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Display7seg output file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Diskout = fopen(argv[12], "w");
	if (fp_Diskout == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Diskout output file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_Monitor = fopen(argv[13], "w");
	if (fp_Monitor == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Monitor.txt output file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	fp_MonitorBinary = fopen(argv[14], "w");
	if (fp_MonitorBinary == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Monitor.yuv output file\n Please try againg Later\n Thank you and good bye\n");//opening file error.
	}
	// inisilization of the instruction memory,data memory, disk and irq2 interupts.
	Instructions_Ammount = Inisialize_Instruction_Memory(fp_Imemin, InstructionsMemory);
	Data_Ammount = Inisialize_Data_Memory(fp_Dmemin, Data_Memory);
	Irq2_EnableTimes = Inisialize_Irq2(fp_Irq2in, Irq2);
	Inisilaize_Disk(fp_Diskin, Disk_Content);
	Current_Irq2 = Irq2[IrqStep];
	//decoding each instruction.
	while (Program_counter != -1)
	{
	
		//splitting the instruction to: opcode(first 2 digits), rd(3 digit),rs(4'th digit),rt(5'th digit),imm1(6'th digit to 8'th digit).
		strncpy(OpcodeNumber, InstructionsMemory[Program_counter], 2);
		Opcode =Decode_Opcode(OpcodeNumber);
		rd = Decode_Regester(InstructionsMemory[Program_counter][2]);
		rs = Decode_Regester(InstructionsMemory[Program_counter][3]);
		rt = Decode_Regester(InstructionsMemory[Program_counter][4]);
		Number_Sign = InstructionsMemory[Program_counter][6];
		imm1[0] = InstructionsMemory[Program_counter][7];
		imm1[1] = InstructionsMemory[Program_counter][8];
		imm11[0] = InstructionsMemory[Program_counter][6];
		imm11[1] = InstructionsMemory[Program_counter][7];
		imm11[2] = InstructionsMemory[Program_counter][8];
		// negative immediate number check.
		if (Number_Sign == 'f')//if we have a negative number we apply the oposit 2"s compliment algorithem
		{
			imm1_Dec = (int)strtol(imm1, NULL, 16);
			imm1_Dec -= 0x0001;
			imm1_Dec = -(imm1_Dec ^ 0xFF);
			
		}
		else
		{
			sscanf(imm1, "%03x", &imm1_Dec);
		}
		// updating rd,rs or rt if one of them represent imm1.
		if (rs == 1) Regesters[rs] = imm1_Dec;
		else if (rt == 1)Regesters[rt] = imm1_Dec;
		else if (rd == 1)Regesters[rd] = imm1_Dec;
		//taking out imm2(9'th digit to 11'th digit) from the instruction.
		Number_Sign = InstructionsMemory[Program_counter][9];
		imm2[0] = InstructionsMemory[Program_counter][10];
		imm2[1] = InstructionsMemory[Program_counter][11];
		imm22[0] = InstructionsMemory[Program_counter][9];
		imm22[1] = InstructionsMemory[Program_counter][10];
		imm22[2] = InstructionsMemory[Program_counter][11];
		// negative immediate number check.
		if ((Number_Sign == 'f'))//if we have a negative number we apply the oposit 2"s compliment algorithem
		{
			imm2_Dec = (int)strtol(imm2, NULL, 16);
			imm2_Dec -= 0x0001;
			imm2_Dec = -(imm2_Dec ^ 0xFF);
		}
		else
		{
			sscanf(imm2, "%03x", &imm2_Dec);
		}
		// updating rd,rs or rt if one of them represent imm2.
		if (rs == 2) Regesters[rs] = imm2_Dec;
		else if (rt == 2)Regesters[rt] = imm2_Dec;
		else if (rd == 2)Regesters[rd] = imm2_Dec;
		// excuting the instruction usng Instruct_Excute function and update the Program counter if needed.
		Program_counter = Instruction_Excute(Opcode, Regesters, IORegesters, rd, rs, rt, Data_Memory, imm1_Dec, imm2_Dec, Program_counter, Buffer, Disk_Content, Irq2, Irq2_EnableTimes, fp_Leds, fp_Hwregtrace, fp_Display7seg, &Bussy, Clock, Disk_Ammount,fp_Trace, InstructionsMemory[Program_counter]);
		// innterupt check.
		if ((Clock == Current_Irq2) | (Clock - 1 == Current_Irq2) | (Clock + 1 == Current_Irq2))
		{
			IORegesters[5] = 1;
			IrqStep++;
			Current_Irq2 = Irq2[IrqStep];
		}
		Irq_all = ((IORegesters[0] & IORegesters[3]) | (IORegesters[1] & IORegesters[4]) | (IORegesters[2] & IORegesters[5]));
		if (Irq_all == 1)
		{
			if (Bussy == 0)
			{
				IORegesters[7] = Program_counter;
				Program_counter = IORegesters[6]-1;
				Bussy = 1;
			}
			IORegesters[5] = 0;
		}
		if (IORegesters[8] < 0xFFFFFFFF)IORegesters[8]++;
		if (IORegesters[8] == 0xFFFFFFFF)IORegesters[8] = 0;
		Clock++;
		Program_counter++;
		
	}
	Update_Data_Memory(fp_Dmemout, Data_Memory);//update the data memory file after finishing the jop.
	Update_Regout(fp_Regout, Regesters);//update the regout file after finishing the jop.
	Update_Cycles(fp_Cycles, Clock);//update the cycles file after finishing the jop.
	Update_Disk(fp_Diskout, Disk_Content);//update the disk memory file after finishing the jop.
	Update_Monitor(fp_Monitor, fp_MonitorBinary, Buffer);//update the monitor files after finishing the jop.
	//closing the files.
	fclose(fp_Imemin);
	fclose(fp_Dmemin);
	fclose(fp_Diskin);
	fclose(fp_Irq2in);
	fclose(fp_Dmemout);
	fclose(fp_Regout);
	fclose(fp_Trace);
	fclose(fp_Hwregtrace);
	fclose(fp_Cycles);
	fclose(fp_Leds);
	fclose(fp_Display7seg);
	fclose(fp_Diskout);
	fclose(fp_Monitor);
	fclose(fp_MonitorBinary);
	return 1;
}