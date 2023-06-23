/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int isLabelOk(char *label);
void addtoList(char *label, int PC, char *opcode, char *arg0);
void printlabellist();
int checkRegister(char *arg);
char *getBinary(int num);

typedef struct Label
{
	char Labelname[7];
	char value[MAXLINELENGTH];
	int address;
	struct Label *next;
} Label;

Label labellist; // JUST STARTING POINT! NO ITEMS
char *temp;
int main(int argc, char *argv[])
{
	int PC = 0;
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
			arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3)
	{
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
					 argv[0]);
		exit(1);
	}
	inFileString = argv[1];
	outFileString = argv[2];
	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL)
	{
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL)
	{
		printf("error in opening %s\n", outFileString);
		exit(1);
	}
	// compute Label
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
	{
		if (strcmp(label, ""))
		{ // if label exists
			if (isLabelOk(label))
			{
				addtoList(label, PC, opcode, arg0);
			}
			else exit(1);
		}
		PC++;
	}
	rewind(inFilePtr);
	// generate machine code to outfile
	PC = 0;
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
	{
		int code;

		if (!strcmp(opcode, "add"))
		{
			code = (0 << 22);
			if (checkRegister(arg0))
			{
				if (checkRegister(arg1))
				{
					if (checkRegister(arg2))
					{
						code |= (atoi(arg0) << 19);
						code |= (atoi(arg1) << 16);
						code |= atoi(arg2);
					}
					else
						exit(1);
				}
				else
					exit(1);
			}
			else
				exit(1);
			fprintf(outFilePtr, "%d\n", code);
		}
		else if (!strcmp(opcode, "nor"))
		{
			code = (1 << 22);
			if (checkRegister(arg0))
			{
				if (checkRegister(arg1))
				{
					if (checkRegister(arg2))
					{
						code |= (atoi(arg0) << 19);
						code |= (atoi(arg1) << 16);
						code |= atoi(arg2);
					}
					else
						exit(1);
				}
				else
					exit(1);
			}
			else
				exit(1);
			fprintf(outFilePtr, "%d\n", code);
		}
		else if (!strcmp(opcode, "lw"))
		{
			code = (2 << 22);
			if (checkRegister(arg0))
			{
				if (checkRegister(arg1))
				{
					code |= (atoi(arg0) << 19);
					code |= (atoi(arg1) << 16);
				}
				else
					exit(1);
			}
			else
				exit(1);
			// offsetfield
			if (!isNumber(arg2))
			{ // symbolic address
				int address = 0;
				Label *cur = &labellist;
				while (cur != NULL)
				{
					if (!strcmp(cur->Labelname, arg2))
					{
						address = cur->address;
						break;
					}
					else
						cur = cur->next;
				}
				code |= address;
			}
			else
			{ // numeric address
				if (atoi(arg2) > 32767 || atoi(arg2) < -32768)
				{
					exit(1);
				}
				code |= atoi(arg2);
			}
			fprintf(outFilePtr, "%d\n", code);
		}
		else if (!strcmp(opcode, "sw"))
		{
			code = (3 << 22);
			if (checkRegister(arg0))
			{
				if (checkRegister(arg1))
				{
					code |= (atoi(arg0) << 19);
					code |= (atoi(arg1) << 16);
				}
				else
					exit(1);
			}
			else
				exit(1);
			// offsetfield
			if (!isNumber(arg2))
			{ // symbolic address
				int address = 0;
				Label *cur = &labellist;
				while (cur != NULL)
				{
					if (!strcmp(cur->Labelname, arg2))
					{
						address = cur->address;
						break;
					}
					else
						cur = cur->next;
				}
				code |= address;
			}
			else
			{ // numeric address
				if (atoi(arg2) > 32767 || atoi(arg2) < -32768)
				{
					exit(1);
				}
				code |= atoi(arg2);
			}
			fprintf(outFilePtr, "%d\n", code);
		}

		else if (!strcmp(opcode, "beq"))
		{
			code = (4 << 22);
			if (checkRegister(arg0))
			{
				if (checkRegister(arg1))
				{
					code |= (atoi(arg0) << 19);
					code |= (atoi(arg1) << 16);
				}
				else
					exit(1);
			}
			else
				exit(1);
			if (!isNumber(arg2))
			{ // symbolic address
				int address = -1;
				Label *cur = &labellist;
				while (cur != NULL)
				{
					if (!strcmp(cur->Labelname, arg2))
					{
						address = cur->address;
						break;
					}
					else
						cur = cur->next;
				}
				if (address==-1) {
					exit(1);
				}
				address = (address - PC - 1);
				address &= 0xFFFF;
				code |= address;
			}
			else
			{ // numeric address
				if (atoi(arg2) > 32767 || atoi(arg2) < -32768)
				{
					exit(1);
				}
				code |= atoi(arg2);
			}
			fprintf(outFilePtr, "%d\n", code);
		}
		else if (!strcmp(opcode, "jalr"))
		{
			code = (5 << 22);
			if (checkRegister(arg0))
			{
				if (checkRegister(arg1))
				{
					code |= (atoi(arg0) << 19);
					code |= (atoi(arg1) << 16);
				}
				else
					exit(1);
			}
			else
				exit(1);
			fprintf(outFilePtr, "%d\n", code);
		}
		else if (!strcmp(opcode, "halt"))
		{
			code = (6 << 22);
			fprintf(outFilePtr, "%d\n", code);
		}
		else if (!strcmp(opcode, "noop"))
		{
			code = (7 << 22);
			fprintf(outFilePtr, "%d\n", code);
		}
		else if (!strcmp(opcode, ".fill"))
		{
			if (atoi(arg0) != 0)
			{
				int arg = atoll(arg0);
				code = arg;
			}
			else //
			{
				int address = 0;
				Label *cur = &labellist;
				while (cur != NULL)
				{
					if (!strcmp(cur->Labelname, arg0))
					{
						address = cur->address;
						break;
					}
					else
						cur = cur->next;
				}
				code = address;
			}
			fprintf(outFilePtr, "%d\n", code);
		}
		else
		{
			exit(1);
		}
		PC++;
	}

	if (inFilePtr)
	{
		fclose(inFilePtr);
	}
	if (outFilePtr)
	{
		fclose(outFilePtr);
	}
	exit(0);
	return (0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
								 char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;
	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';
	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL)
	{
		/* reached end of file */
		return (0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL)
	{
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}
	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label))
	{
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}
	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
							"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
				 opcode, arg0, arg1, arg2);
	return (1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return ((sscanf(string, "%d", &i)) == 1);
}

int isLabelOk(char *label)
{
	if (strlen(label) > 6)
	{
		return 0;
	}
	Label *cur = &labellist;
	while (cur!= NULL)
	{
		if (!strcmp(cur->Labelname, label))
		{
			return 0;
		}
		else
		{
			cur = cur->next;
		}
	}
	return 1;
}

void addtoList(char *label, int PC, char *opcode, char *arg0)
{
	Label *newLabel = (Label *)malloc(sizeof(Label));
	Label *cur = &labellist;
	while (cur->next != NULL)
	{
		cur = cur->next;
	}
	cur->next = newLabel;
	newLabel->address = PC;
	strcpy(newLabel->Labelname, label);

	if (!strcmp(opcode, ".fill"))
	{ // equal returns 0
		if (atoll(arg0) != 0)
		{
			long long arg = atoll(arg0);
			if (arg > 2147483647 || arg < -2147483648)
			{
				exit(1);
			}
		}
		else
			strcpy(newLabel->value, arg0);
	}
}

// for debugging
void printlabellist()
{
	Label *cur = &labellist;
	cur = cur->next;
	while (cur != NULL)
	{
		printf("label name is %s, liencnt is %d, value is %s\n", cur->Labelname, cur->address, cur->value);
		cur = cur->next;
	}
}

int checkRegister(char *arg)
{
	if (atoi(arg) >= 0 && atoi(arg) <= 7)
	{
		if ((atof(arg) - atoi(arg)) == 0)
		{
			return 1;
		}
	}
	return 0;
}

char *getBinary(int num)
{
	switch (num)
	{
	case 0:
		temp = "000\0";
		break;
	case 1:
		temp = "001\0";
		break;
	case 2:
		temp = "010\0";
		break;
	case 3:
		temp = "011\0";
		break;
	case 4:
		temp = "100\0";
		break;
	case 5:
		temp = "101\0";
		break;
	case 6:
		temp = "110\0";
		break;
	case 7:
		temp = "111\0";
		break;
	}
	return temp;
}

char *returnBinary(int num)
{
	temp = "\0";
	while (1)
	{
		if (num / 2 == 0)
		{
			num % 2;
			break;
		}
	}
}