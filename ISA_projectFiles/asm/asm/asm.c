#include <stdio.h>
#include <string.h>
#include <process.h>
#define MAX_LABELS 50
#define MAX_ROW 500
#define MEM_SIZE 4096
typedef struct word
{
	int place;
	int value;



}word;
typedef struct label
{
	char label_name[MAX_LABELS];
	int Address;




}label;
int add_word(word words[], int place, char* value, int word_counter);// Function that add's a word to array of words
void fill_words(FILE* fp_out, int word_counter, word words[]);//function that update's the Data memory with the words(".word" instruction)
char checkReg(char* Reg);//function that decodes which register Reg represent
void checkOpcode(char* opcode, char opcodenum[]);// function that decodes which opcode "opcode" represent
int islabel(char* CheckLabel, int label_counter, label labels[]);//function that check if the word we want to check is a label or not
int GetLabelsAddress(label labels[], char* labelName, int label_counter);// function that takes a label name and return his address if not it returns an error masage 
int GetWordssValue(word words[], int place, int word_counter);//function that take words place and return the value :".word place value" 
int read_labels(FILE * fp_in, label labels[]);// function that reads the file and fill an array with all the labels
void opcodeprint(FILE * fp_out, char* opcode, char* rd, char* rs, char* rt, char* imm, label labels[], int label_counter);// function that prints the inst in hex
int read_asm(FILE * fp_in, FILE* fp_out, word words[], int  word_counter, label labels[], int label_counter);// function that does assembler work
int MaxWordPlace(word words[], int word_count);//function that takes an array of words and return the maximom place of word with value that is bigger than zero

int MaxWordPlace(word words[], int word_count)
{
	int MaxPlace=0, i = 0;
	for (i; i <= word_count; i++)
	{
		if (words[i].place > MaxPlace) MaxPlace = words[i].place;
		else continue;
	}
	return MaxPlace;
}
int add_word(word words[], int place, char* value, int WordCounter)
{
	int wc = WordCounter;
	int imm = (int)strtol(value, NULL, 0);
	int placei = place;
	words[wc].place = placei;
	words[wc].value = imm;
	wc++;
	return wc;

}
void fill_words(FILE* fp_out, int word_counter, word words[])
{
	int j = 0;
	int maxw = MaxWordPlace(words, word_counter);
	while (j <= maxw)
	{
		
		if (GetWordssValue(words, j, word_counter) != -1)// I print data memory until the last place in words array that its value bigger than zero
		{
			
			fprintf(fp_out, "%08x\n", GetWordssValue(words, j, word_counter));//8 digit hex print 
			

		}
		else {
			fprintf(fp_out, "00000000\n");
			
		}
		j++;
	}
}
char checkReg(char* Reg)
{
	if (strcmp(Reg, "$zero") == 0)return'0';
	else if (strcmp(Reg, "$imm") == 0) return'1';
	else if (strcmp(Reg, "$v0") == 0) return '2';
	else if (strcmp(Reg, "$a0") == 0) return '3';
	else if (strcmp(Reg, "$a1") == 0) return  '4';
	else if (strcmp(Reg, "$t0") == 0) return  '5';
	else if (strcmp(Reg, "$t1") == 0) return '6';
	else if (strcmp(Reg, "$t2") == 0) return '7';
	else if (strcmp(Reg, "$t3") == 0) return  '8';
	else if (strcmp(Reg, "$s0") == 0) return  '9';
	else if (strcmp(Reg, "$s1") == 0) return 'A';
	else if (strcmp(Reg, "$s2") == 0) return 'B';
	else if (strcmp(Reg, "$gp") == 0) return 'C';
	else if (strcmp(Reg, "$sp") == 0) return 'D';
	else if (strcmp(Reg, "$fp") == 0) return 'E';
	else if (strcmp(Reg, "$ra") == 0) return 'F';

}
void checkOpcode(char* opcode, char opcodenum[])
{
	if (strcmp(opcode, "add") == 0) strcpy(opcodenum, "00");
	else if (strcmp(opcode, "sub") == 0) strcpy(opcodenum, "01");
	else if (strcmp(opcode, "and") == 0) strcpy(opcodenum, "02");
	else if (strcmp(opcode, "or") == 0)  strcpy(opcodenum, "03");
	else if (strcmp(opcode, "xor") == 0) strcpy(opcodenum, "04");
	else if (strcmp(opcode, "mul") == 0) strcpy(opcodenum, "05");
	else if (strcmp(opcode, "sll") == 0) strcpy(opcodenum, "06");
	else if (strcmp(opcode, "sra") == 0) strcpy(opcodenum, "07");
	else if (strcmp(opcode, "srl") == 0) strcpy(opcodenum, "08");
	else if (strcmp(opcode, "beq") == 0) strcpy(opcodenum, "09");
	else if (strcmp(opcode, "bne") == 0) strcpy(opcodenum, "0A");
	else if (strcmp(opcode, "blt") == 0) strcpy(opcodenum, "0B");
	else if (strcmp(opcode, "bgt") == 0) strcpy(opcodenum, "0C");
	else if (strcmp(opcode, "ble") == 0) strcpy(opcodenum, "0D");
	else if (strcmp(opcode, "bge") == 0) strcpy(opcodenum, "0E");
	else if (strcmp(opcode, "jal") == 0) strcpy(opcodenum, "0F");
	else if (strcmp(opcode, "lw") == 0) strcpy(opcodenum, "10");
	else if (strcmp(opcode, "sw") == 0) strcpy(opcodenum, "11");
	else if (strcmp(opcode, "reti") == 0) strcpy(opcodenum, "12");
	else if (strcmp(opcode, "in") == 0) strcpy(opcodenum, "13");
	else if (strcmp(opcode, "out") == 0)strcpy(opcodenum, "14");
	else if (strcmp(opcode, "halt") == 0) strcpy(opcodenum, "15");
	else strcpy(opcodenum, "soory buddy it's not an opcode");
}
int islabel(char* CheckLabel, int label_counter, label labels[])
{
	for (int j = 0; j < label_counter; j++)
	{
		if (strcmp(CheckLabel, labels[j].label_name) == 0)
		{
			return 1;
		}

	}
	return 0;
}
int GetLabelsAddress(label labels[], char* labelName, int label_counter)
{
	int add = 0;
	for (int j = 0; j < label_counter; j++)
	{
		if (strcmp(labelName, labels[j].label_name) == 0)
		{
			add = labels[j].Address;
			return add;
		}

	}
	return -1;
}
int GetWordssValue(word words[], int place, int word_counter)
{
	int j = 0;
	for (j; j <= word_counter; j++)
	{
		if (place== words[j].place)
		{
			return (int)words[j].value;
		}
		
	}
	return -1;
}
int read_labels(FILE * fp_in, label labels[])
{
	int label_counter = 0;
	char line[MAX_ROW];
	int i = 1;
	char label[MAX_LABELS];
	char rd[MAX_LABELS]="";
	char rs[MAX_LABELS] = "";
	char rt[MAX_LABELS] = "";
	char opcodenum[MAX_LABELS] = "";
	char rss = ' ', rtt = ' ', rdd = ' ';
	while ((fgets(line, MAX_ROW, fp_in) != NULL) && !feof(fp_in))
	{
		sscanf(line, "%s %s %s %s ", label,rd, rs,rt);
		if (strcmp(label, ".word") == 0)
		{
			i++;
			continue;
		}
		rd[strlen(rd) - 1] = 0;//remove ","
		rs[strlen(rs) - 1] = 0;//remove ","
		rt[strlen(rt) - 1] = 0;//remove ","
		checkOpcode(label, opcodenum);
		if ((strcmp(opcodenum, "soory buddy it's not an opcode") == 0) && strcmp(label, ".word") != 0)//label situation
		{
			label[strlen(label) - 1] = 0;//remove :
			strcpy(labels[label_counter].label_name, label);
			labels[label_counter].Address = i;
			label_counter++;
			i++;
		}
		else if (strcmp(label, ".word") == 0)//word situation
		{
			i++;
		}
		else if (strcmp(opcodenum, "soory buddy it's not an opcode") != 0)//opcode situation
		{
			if (checkReg(rs) == '1' || checkReg(rt) == '1' || checkReg(rd) == '1') i += 2;
		}
		else i++;
	}
	return label_counter;
}
void opcodeprint(FILE* fp_out, char* opcode, char* rd, char* rs, char* rt, char* imm, label labels[], int label_counter)
{
	char rdnum, rsnum, rtnum;
	char opcodenum[MAX_LABELS] = "";
	int immset = 0;
	rsnum = checkReg(rs);
	rtnum = checkReg(rt);
	rdnum = checkReg(rd);
	checkOpcode(opcode, opcodenum);
	fprintf(fp_out, "%s%c%c%c\n", opcodenum, rdnum, rsnum, rtnum);// we always print the instruction statment 
	if (rdnum == '1' || rsnum == '1' || rtnum == '1')// if we use $imm register
	{
		if (!(islabel(imm, label_counter, labels)))//if $imm is not a label then we print it in a new line
		{
			immset = (int)strtol(imm, NULL, 10);
			if (immset < 0)//because we have to print just 5 digits we take the 5 most MSB bits 
			{
				immset = immset & 0x00FFFFF;
				fprintf(fp_out, "%01X\n", immset);
			}
			else fprintf(fp_out, "%05X\n", immset);

			
		}
		else// if $imm is a label we print the label's address
		{
			fprintf(fp_out, "%05X\n", GetLabelsAddress(labels,imm,label_counter));

		}

			}

}
int read_asm(FILE * fp_in, FILE* fp_out, word words[], int  word_counter, label labels[], int label_counter)
{
	int pcc = 0;
	char* opcode[MAX_ROW];
	int wc = 0;
	int WordCounter =0;
	char line[MAX_ROW];
	char* rd, *rs, *rt, *imm;
	char opcodenum[MAX_LABELS] = "";
	while ((fgets(line, MAX_ROW, fp_in) != NULL))// we scan lines from the file while it is not empty
	{
		sscanf(line, "%s", opcode);
		if (opcode == EOF)continue;  //if line is empty
		// check if line is .word line
		if (strcmp(opcode, ".word") == 0)
		{
			strtok(line, "\t  ");
			char* place = strtok(NULL, " \t");
			char* value = strtok(NULL, " \t\n");
			WordCounter= add_word(words, (int)strtol(place, NULL, 0), value, WordCounter);
			
		}
		else
		{
			//if line starts with opcode
			checkOpcode(opcode, opcodenum);
			if (strcmp(opcodenum, "soory buddy it's not an opcode") != 0)
			{
				strtok(line, "\t , ");
				rd = strtok(NULL, " ,\t");
				rs = strtok(NULL, " ,\t\n");
				rt = strtok(NULL, " ,\t\n");
				imm = strtok(NULL, " ,\t\n");
				opcodeprint(fp_out, opcode, rd, rs, rt, imm, labels, label_counter);
				
			}
			else //line start with label
			{
				strtok(line, ":\t  ");
				char* opcode_new = strtok(NULL, "\t , ");
				checkOpcode(opcode_new, opcodenum);
				if (strcmp(opcode_new, "\n") == 0)
				{
					pcc++;
					continue;
				}
				rd = strtok(NULL, " ,\t");
				rs = strtok(NULL, " ,\t\n");
				rt = strtok(NULL, " ,\t\n");
				imm = strtok(NULL, " ,\t\n");
				opcodeprint(fp_out, opcodenum, rd, rs, rt, imm, labels, label_counter);
				
			}
			
		}
		
	}
	
	return WordCounter;
}
// in read_asm function we have more than 50 lines because its the most importan function in the code and we want
//to make the main short as much as we can because if we get an error its easly to find and fix and more compact 
//and easy to understand 
int main(int argc, char* argv[])
{
	FILE* fp_in;
	FILE* fp_od;
	FILE* fp_oi;
	fp_in = fopen(argv[1], "r");
	if (fp_in == NULL)
	{
		printf("input file didn't open\n");


	}
	fp_oi = fopen(argv[2], "w");
	if (fp_oi == NULL)
	{
		printf("output file didn't open\n");


	}
	fp_od = fopen(argv[3], "w");
	if (fp_od == NULL)
	{
		printf("output file didn't open\n");


	}
	struct word words[MAX_ROW] = { 0 };
	struct label labels[MAX_ROW] = { 0 };
	int label_counter = 0;
	int word_counter = 0;

	
	if (argc != 4) {
		printf("Error in input arguments");
		return -1;
	}
	label_counter = read_labels(fp_in, labels);
	fclose(fp_in);
	fp_in = fopen(argv[1], "r");
	word_counter= read_asm(fp_in, fp_oi, words, word_counter, labels, label_counter);
	fclose(fp_in);
	fill_words(fp_od, word_counter, words);
	fclose(fp_oi);
	fclose(fp_od);
	return 1;
}