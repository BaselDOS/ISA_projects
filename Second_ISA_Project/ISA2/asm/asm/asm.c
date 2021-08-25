#include <stdio.h>
#include<string.h>
#include <process.h>
#define Max_ROW 500
#define MAX_LABEL_NAME 50
#define IDMEM_SIZE 4096
/*_____________________________________________________Structures_________________________________________________*/
typedef struct Word
{
	int Address;
	int Data;
}Word;
typedef struct Label
{
	char Name[MAX_LABEL_NAME];
	int Address;
}Label;
/*_____________________________________________________Declarations_________________________________________________*/
int Add_Words(Word Words[], int Adress, char* Data, int Words_Ammount);
int Top_Word_Address(Word Words[], int Words_Ammount);
int Get_Word_Data(Word Words[], int Words_Amount, int Address);
void Update_Memory(FILE* fp_dmem, int Words_Ammount, Word Words[]);
int First_Read_Get_Labels_Addresses(FILE* fp_Program, Label Labels[]);
void Decode_Opcode(char* Opcode, char opcode_Hex_Num[]);
char Decode_Regesters(char* Regester);
int Get_Label_Address(Label Labels[], char* Label_Name, int Label_Ammount);
int Check_If_Label(char* Is_Label, int Label_Ammount, Label Labels[]);
void Update_Instruction_Memory(FILE* fp_Imemin,Label Labels[],int Label_Ammount,char* Opcode,char* rd,char* rs,char* rt, char* imm1,char* imm2);
int Second_Read_Asmbler(FILE* fp_Program, FILE* fp_Imemin, Word Words[], int Word_Ammount, Label Labels[], int Label_Ammount);

/*_______________________________________________Definitions_____________________________________________________________*/

int Add_Words(Word Words[], int Address, char* Data, int Words_Ammount)// function that addes a new word to the list.
{
	int WordCounter = Words_Ammount;
	int value = (int)strtol(Data, NULL, 0);
	int Place = Address;
	Words[WordCounter].Address = Place;
	Words[WordCounter].Data = value;
	WordCounter++;
	return WordCounter;

}
int Top_Word_Address(Word Words[], int Words_Ammount)// function that finds and returns the farest address that includes a Data at memory data.
{
	int Top_Address = 0;
	int i = 0;
	for (i; i <= Words_Ammount; i++)
	{
		if (Words[i].Address > Top_Address)Top_Address = Words[i].Address;
		else continue;
	}
	return Top_Address;
}
int Get_Word_Data(Word Words[], int Words_Amount, int Address)//function that findes and returns a specific word data by taking it's address.
{
	int i = 0;
	for (i; i <= Words_Amount; i++)
	{
		if (Words[i].Address == Address)
			return Words[i].Data;
	}
	return -1;
}
void Update_Memory(FILE* fp_dmem, int Words_Ammount, Word Words[])// function that takes a words list and updates the data memory by it's values.
{
	int i = 0;
	int Top_Word = Top_Word_Address(Words, Words_Ammount);
	for (i; i <= Top_Word; i++)
	{
		if (Get_Word_Data(Words, Words_Ammount, i) != -1)
			fprintf(fp_dmem, "%08x\n", Get_Word_Data(Words, Words_Ammount, i));
		else
		{
			fprintf(fp_dmem, "00000000\n");
		}

	}
}
void Decode_Opcode(char* Opcode, char opcode_Hex_Num[])//function that decodes the Hex number of the opcode we have as told at the requerments sheet.
{
	if (strcmp(Opcode, "add") == 0)
		strcpy(opcode_Hex_Num, "00");
	else if (strcmp(Opcode, "sub") == 0)
		strcpy(opcode_Hex_Num, "01");
	else if (strcmp(Opcode, "and") == 0)
		strcpy(opcode_Hex_Num, "02");
	else if (strcmp(Opcode, "or") == 0)
		strcpy(opcode_Hex_Num, "03");
	else if (strcmp(Opcode, "sll") == 0)
		strcpy(opcode_Hex_Num, "04");
	else if (strcmp(Opcode, "sre") == 0)
		strcpy(opcode_Hex_Num, "05");
	else if (strcmp(Opcode, "srl") == 0)
		strcpy(opcode_Hex_Num, "06");
	else if (strcmp(Opcode, "beq") == 0)
		strcpy(opcode_Hex_Num, "07");
	else if (strcmp(Opcode, "bne") == 0)
		strcpy(opcode_Hex_Num, "08");
	else if (strcmp(Opcode, "blt") == 0)
		strcpy(opcode_Hex_Num, "09");
	else if (strcmp(Opcode, "bgt") == 0)
		strcpy(opcode_Hex_Num, "0A");
	else if (strcmp(Opcode, "ble") == 0)
		strcpy(opcode_Hex_Num, "0B");
	else if (strcmp(Opcode, "bge") == 0)
		strcpy(opcode_Hex_Num, "0C");
	else if (strcmp(Opcode, "jal") == 0)
		strcpy(opcode_Hex_Num, "0D");
	else if (strcmp(Opcode, "lw") == 0)
		strcpy(opcode_Hex_Num, "0E");
	else if (strcmp(Opcode, "sw") == 0)
		strcpy(opcode_Hex_Num, "0F");
	else if (strcmp(Opcode, "reti") == 0)
		strcpy(opcode_Hex_Num, "10");
	else if (strcmp(Opcode, "in") == 0)
		strcpy(opcode_Hex_Num, "11");
	else if (strcmp(Opcode, "out") == 0)
		strcpy(opcode_Hex_Num, "12");
	else if (strcmp(Opcode, "halt") == 0)
		strcpy(opcode_Hex_Num, "13");
	else strcpy(opcode_Hex_Num, "Not an opcode");
}
char Decode_Regesters(char* Regester)//function that decodes the Hex number of the regester we have as told at the requerments sheet.
{
	if (strcmp(Regester, "$zero") == 0) return '0';
	else if (strcmp(Regester, "$imm1") == 0) return '1';
	else if (strcmp(Regester, "$imm2") == 0) return '2';
	else if (strcmp(Regester, "$v0") == 0) return '3';
	else if (strcmp(Regester, "$a0") == 0) return '4';
	else if (strcmp(Regester, "$a1") == 0) return '5';
	else if (strcmp(Regester, "$a2") == 0) return '6';
	else if (strcmp(Regester, "$t0") == 0) return '7';
	else if (strcmp(Regester, "$t1") == 0) return '8';
	else if (strcmp(Regester, "$t2") == 0) return '9';
	else if (strcmp(Regester, "$s0") == 0) return 'A';
	else if (strcmp(Regester, "$s1") == 0) return 'B';
	else if (strcmp(Regester, "$s2") == 0) return 'C';
	else if (strcmp(Regester, "$gp") == 0) return 'D';
	else if (strcmp(Regester, "$sp") == 0) return 'E';
	else if (strcmp(Regester, "$ra") == 0) return 'F';
}
int First_Read_Get_Labels_Addresses(FILE* fp_Program, Label Labels[])// a function that apply the first asmbler read.
{
	//inisualizations
	int Label_Ammount = 0;
	char Row[Max_ROW] = "";
	int i = 1;
	char Label[MAX_LABEL_NAME] = "";
	char Offset[Max_ROW] = "";
	char Opcode_Hex_value[Max_ROW] = "";
	while((fgets(Row,Max_ROW,fp_Program)!=NULL)&&!feof(fp_Program))//file reading while it's not empty.
	{
	sscanf(Row, "%s %s ", Label, Offset);//reading a row.
	if (strcmp(Label, ".word") == 0)//.word situation.
	{
		continue;
	}
	Decode_Opcode(Label, Opcode_Hex_value);
	if (strcmp(Opcode_Hex_value, "Not an opcode")!=0)//opcode situation.
	{
		i++;
		continue;
	}
	else// Label situation.
	{
		Label[strlen(Label) -1] = 0;
		strcpy(Labels[Label_Ammount].Name, Label);
		Labels[Label_Ammount].Address = i-1;
		Label_Ammount++;
		//i++;
	}
	}
	return Label_Ammount;
}
int Get_Label_Address(Label Labels[], char* Label_Name, int Label_Ammount)// a function that gets the labels address by it's name.
{
	int i = 0;
	for (i; i < Label_Ammount; i++)
	{
		if (strcmp(Label_Name, Labels[i].Name) == 0)
		{
			return Labels[i].Address;
		}
	}
	return -1;
}
int Check_If_Label(char* Is_Label, int Label_Ammount, Label Labels[])// a function that decode if we are dealing with a label or not.
{
	int i = 0;
	for (i; i < Label_Ammount; i++)
	{
		if (strcmp(Is_Label, Labels[i].Name) == 0)
			return 1;
	}
	return -1;
}
void Update_Instruction_Memory(FILE* fp_Imemin, Label Labels[], int Label_Ammount, char* Opcode, char* rd, char* rs, char* rt, char* imm1, char* imm2)// a function that updates the instruction memory after we finish the whole work.
{
	//Inisializations.
	char rd_Hex_Value, rs_Hex_Value, rt_Hex_Value;
	char Opcode_Hex_Value[MAX_LABEL_NAME] = " ";
	int imm1_Decemal = 0;
	int imm2_Decemal = 0;
	char reserved = '0';
	rd_Hex_Value = Decode_Regesters(rd);
	rs_Hex_Value = Decode_Regesters(rs);
	rt_Hex_Value = Decode_Regesters(rt);
	Decode_Opcode(Opcode, Opcode_Hex_Value);
	if (rd_Hex_Value == '1' || rs_Hex_Value == '1' || rt_Hex_Value == '1')// If imm1 use is noticed.
	{
		if ((Check_If_Label(imm1, Label_Ammount, Labels))==-1)// Immediate number situation.
		{
			if (imm1[1] == 'x')imm1_Decemal = (int)strtol(imm1, NULL, 16);
			else imm1_Decemal = (int)strtol(imm1,NULL,10);
			if (imm1_Decemal < 0)// Negative number situation .
			{
				imm1_Decemal = imm1_Decemal & 0x0000FFF;//Taking the 3LSB digits as told at the requerment's sheet.
			}
		}
		else
		{
			imm1_Decemal = Get_Label_Address(Labels, imm1, Label_Ammount);// label situation - we return it's address.
		}
	}
	if (rd_Hex_Value == '2' || rs_Hex_Value == '2' || rt_Hex_Value == '2')// If imm2 use is noticed.
	{
		if ((Check_If_Label(imm2, Label_Ammount, Labels))==-1)// Immediate number situation.
		{
			imm2_Decemal = (int)strtol(imm2, NULL, 10);
			if (imm2_Decemal < 0)// Negative number situation .
			{
				imm2_Decemal = imm2_Decemal & 0x0000FFF;//Taking the 3LSB digits as told at the requerment's sheet.
			}
		}
		else
		{
			imm2_Decemal = Get_Label_Address(Labels, imm2, Label_Ammount);// label situation - we return it's address
		}
	}
	
	fprintf(fp_Imemin, "%s%c%c%c%c%03x%03x\n", Opcode_Hex_Value, rd_Hex_Value, rs_Hex_Value, rt_Hex_Value, reserved, imm1_Decemal, imm2_Decemal);//Output file (instruction memory) printing.

}
int Second_Read_Asmbler(FILE* fp_Program, FILE* fp_Imemin, Word Words[], int Word_Ammount, Label Labels[], int Label_Ammount)
{
	char* Opcode[Max_ROW] ;
	char* rd, *rs, *rt, *imm1, *imm2;
	int WordsAmmount = 0;
	char Row[Max_ROW] = " ";
	char Opcode_Hex_Value[MAX_LABEL_NAME] = "";
	while ((fgets(Row, Max_ROW, fp_Program) != NULL))
	{
		sscanf(Row, "%s", Opcode);
		if (Opcode == EOF)continue; // if row is empty
		if (strcmp(Opcode, ".word") == 0) //Word situation
		{
			strtok(Row, "\t  ");
			char* Address = strtok(NULL, " \t");
			char* Data = strtok(NULL, " \t\n");
			if(Address[1]=='x')WordsAmmount = Add_Words(Words, (int)strtol(Address, NULL, 16), Data, WordsAmmount);
			else WordsAmmount = Add_Words(Words, (int)strtol(Address, NULL, 0), Data, WordsAmmount);
		}
		else
		{
			Decode_Opcode(Opcode, Opcode_Hex_Value);//Opcode situation
			if (strcmp(Opcode_Hex_Value, "Not an opcode") != 0)
			{
				strtok(Row, "\t  ");
				rd = strtok(NULL, " \t,");
				rs = strtok(NULL, " ,\t\n");
				rt = strtok(NULL, " ,\t\n");
				imm1 = strtok(NULL, " ,\t\n");
				imm2 = strtok(NULL, " ,\t\n");
				Update_Instruction_Memory(fp_Imemin, Labels, Label_Ammount, Opcode, rd, rs, rt, imm1, imm2);
			}
			else //Label situation
			{
				char* Opcode_helper = " ";
				strtok(Row, ":\t  ");
				Opcode_helper = strtok(NULL, "\t , ");
				Decode_Opcode(Opcode_helper, Opcode_Hex_Value);
				if (strcmp(Opcode_helper, "\n") == 0)continue;
				strtok(Row, "\t  ");
				rd = strtok(NULL, " \t,");
				rs = strtok(NULL, " ,\t\n");
				rt = strtok(NULL, " ,\t\n");
				imm1 = strtok(NULL, " ,\t\n");
				imm2 = strtok(NULL, " ,\t\n");
				Update_Instruction_Memory(fp_Imemin, Labels, Label_Ammount, Opcode_Hex_Value, rd, rs, rt, imm1, imm2);

			}
		}
	}
	return WordsAmmount;
}
//The second read function is a littel long because we want the main function to be as short and compact as posible
/*____________________________________________Main Function________________________________________________________________*/
int main(int argc, char* argv[])
{   // Inisializations:
	FILE* fp_Program;
	FILE* fp_Imemin;
	FILE* fp_Dmemin;
	//File's opening:
	fp_Program = fopen(argv[1], "r");
	if (fp_Program == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the input file\n Please try againg Later\n Thank you and good bye\n");
	}
	fp_Imemin = fopen(argv[2], "w");
	if (fp_Imemin == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Instruction memory file\n Please try againg Later\n Thank you and good bye\n");
	}
	fp_Dmemin = fopen(argv[3], "w");
	if (fp_Dmemin == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the Data memory file\n Please try againg Later\n Thank you and good bye\n");
	}
	if (argc != 4)
	{
		printf("Please enter the right command arguments and try again\n");
	}

	// Inisializations-continue:
	struct Word Words[Max_ROW] = { 0 };
	struct Label Labels[MAX_LABEL_NAME]={0};
	int Labels_Ammount=0;
	int Words_Ammount = 0;
	Labels_Ammount=First_Read_Get_Labels_Addresses(fp_Program,Labels);
	fclose(fp_Program);
	fp_Program = fopen(argv[1], "r");
	if (fp_Program == NULL)
	{
		printf("Somthing went wrong ,we couldn't open the input file\n Please try againg Later\n Thank you and good bye\n");
	}
	Words_Ammount=Second_Read_Asmbler(fp_Program,fp_Imemin,Words,Words_Ammount,Labels,Labels_Ammount);
	fclose(fp_Program);
	Update_Memory(fp_Dmemin, Words_Ammount, Words);
	fclose(fp_Imemin);
	fclose(fp_Dmemin);
	return;

	
}