#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct intermediate_table{
	//int instr_no;
	int opcode;
	int parameters[5];

};

int register_code(char *str){
	if (str[1] == 'X'){
		return str[0] - 'A';

	}
	else
		return -1;


}
struct symbol_table{
	char name[20];
	int address;
	int size;
	//struct symbol_table *next;

};
void create_symboltable(struct symbol_table *sym_table_array, int* sym_table_index,char *str,int addr,int size){
	int j;
	++(*sym_table_index);
	for (j = 0; str[j] != '\0'; j++)
		sym_table_array[(*sym_table_index)].name[j] = str[j];
	sym_table_array[(*sym_table_index)].name[j] = '\0';
	sym_table_array[(*sym_table_index)].address = addr;
	sym_table_array[(*sym_table_index)].size = size;

}
struct label_table{
	//char block_name[20];
	int reference;
	int address;
};
void instruction_push(int *instr_stack, int *i,int instr_value){
	++(*i);
	instr_stack[(*i)] = instr_value;
}

int instruction_pop(int* instr_stack, int *i){
	(*i)--;
	return instr_stack[(*i)+1];
}
void labeltable_push(struct label_table *lbltable, int *i,int reference,int addr){
	int j;
	++(*i);
	/*for ( j = 0; j < str[j] != '\0';j++)
	lbltable[(*i)].block_name[j]=str[j];
	lbltable[(*i)].block_name[j] = '\0';
	*/
	lbltable[(*i)].reference = reference;
	lbltable[(*i)].address = addr;
}
int labeltable_pop(struct label_table *lbltable, int *i){
	return lbltable[(*i)--].address;

}
int check_opcode(char *str){

	if (strcmp(str, "MOV") == 0)
		return 1;

	if (strcmp(str, "MOV") == 0)
		return 1;
	if (strcmp(str, "ADD") == 0)
		return 3;
	if (strcmp(str, "SUB") == 0)
		return 4;
	if (strcmp(str, "MUL") == 0)
		return 5;
	if (strcmp(str, "JUMP") == 0)
		return 6;
	if (strcmp(str, "ELSE") == 0)
		return 6;
	if (strcmp(str, "IF") == 0)
		return 7;
	if (strcmp(str, "EQ") == 0)
		return 8;
	if (strcmp(str, "LT") == 0)
		return 9;
	if (strcmp(str, "GT") == 0)
		return 10;
	if (strcmp(str, "LTEQ") == 0)
		return 11;
	if (strcmp(str, "GTEQ") == 0)
		return 12;
	if (strcmp(str, "PRINT") == 0)
		return 13;
	if (strcmp(str, "READ") == 0)
		return 14;
	if (strcmp(str, "ENDIF") == 0)
		return 25;
	if (strcmp(str, "END") == 0)
		return 100;
	return -1;


}

void print_symbol_table(struct symbol_table *sym_table_array, int* i){

	for (int j = 0; j <= (*i); j++){
		printf("\n%s  %d  %d",sym_table_array[j].name,sym_table_array[j].address,sym_table_array[j].size);
	}
}

int compare(char *str){

	if (str[1] == 'X')
		return 1;
	return 0;
}
int find_address_symtable(struct symbol_table* arr, int len, char*str){
	for (int i = 0; i <= len; i++){
		if (strcmp(arr[i].name, str) == 0)
			return arr[i].address;
	}
	return -1;


}

void construct_intermediate_table(struct intermediate_table* arr, int* i,int opcode,int *values,int par_len){

	arr[(*i)].opcode = opcode;
	for (int j = 0; j < par_len; j++)
		arr[(*i)].parameters[j] = values[j];

	(*i)++;
}
void print_intermediate_table(struct intermediate_table *arr, int len){

	for (int i = 0; i < len; i++){
		printf("\n%d  ", arr[i].opcode+1);
		for (int j = 0; j < 5; j++)
			printf("%d ", arr[i].parameters[j]);
	}
}
void main(){
	struct intermediate_table intermediate_arr[50];
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 5; j++)
			intermediate_arr[i].parameters[j] = 0;
	}
	int memory[40] = { 0 },i,j,SIZE=1,flag,flag2,sum,opcode=0;
	char temp1_str[40],temp2_str[40],temp3_str[40],sample_str[40];
	int symboltable_index = -1,memory_index=0,val_index,intermediate_arr_index=0;
	int values[5],change_instr,star_value;
	int jump_arr[26];
	int instruction_stack[30],instruction_stack_index=-1,label_table_stack_index=-1;
	//struct label_table *label_table_stack[10] =(struct label_table*)malloc(20 * sizeof(struct label_table));

	char if_oper[6], if_op1[6], if_op2[6], dummy1[15], dummy2[15];

	struct symbol_table *symbol_table_array = (struct symbol_table*)malloc(10 *sizeof(struct symbol_table));
	struct label_table *label_table_stack = (struct label_table*)malloc(20 * sizeof(struct label_table));

	FILE *fp = fopen("compiler_inp.txt", "r");
	fscanf(fp, "%[^\n]\n", temp1_str);

	
	while (temp1_str[0] != 'S'){
		i = 0;
		flag = 0;
		if (temp1_str[0] == 'D'){
			i = i + 5;
			j = 0;
			while (temp1_str[i] != '\0'){
				if (temp1_str[i] == '['){
					flag = i;
					break;
				}
				temp2_str[j++] = temp1_str[i++];
			}
			temp2_str[j] = '\0';
			if (flag == 0)
			{
				create_symboltable(symbol_table_array, &symboltable_index, temp2_str, memory_index, 1);
				memory_index++;
			}
			else{
				sum = 0;
				i = flag + 1;
				while (temp1_str[i] != ']'){
					sum = sum * 10 + temp1_str[i] - '0';
					i++;
				}
				create_symboltable(symbol_table_array, &symboltable_index, temp2_str, memory_index, sum);
				memory_index = memory_index + sum;

			}
		}
		else{
			i = i + 6;
			j = 0;
			while (temp1_str[i] != ' ' && temp1_str[i]!='='){

				temp2_str[j++] = temp1_str[i++];
			}
			temp2_str[j] = '\0';
			sum = 0;
			i=i+3;
			while (temp1_str[i] != '\0'){
				sum = sum * 10 + temp1_str[i] - '0';
				i++;
			}

			create_symboltable(symbol_table_array, &symboltable_index, temp2_str, memory_index, sum);
			memory_index++;
		}
		fscanf(fp, "%[^\n]\n", temp1_str);


	}

	while (!feof(fp)){
		fscanf(fp, "%[^\n]\n", temp1_str);
		sscanf(temp1_str, "%s %[^*]", temp2_str, temp3_str);//[^*]
		printf("\nsplitted strings %s %s", temp2_str, temp3_str);
		opcode = check_opcode(temp2_str);
		if (opcode == 100)
			break;
		else if (opcode == 7)
		{
			val_index = 0;
			sscanf(temp1_str, "%s %s %s %s %s",dummy1, if_op1,if_oper,if_op2,dummy2);
			printf("\n===%s %s %s", if_op1, if_oper, if_op2);
				if (register_code(if_op1) == -1){
					values[val_index++] = find_address_symtable(symbol_table_array, symboltable_index, if_op1);
				}
				else
					values[val_index++] = register_code(if_op1);

				
				if (register_code(if_op2) == -1){
					values[val_index++] = find_address_symtable(symbol_table_array, symboltable_index, if_op2);
				}
				else
					values[val_index++] = register_code(if_op2);

				
					values[val_index++] = check_opcode(if_oper);


				values[val_index++] = -1;
				instruction_push(instruction_stack, &instruction_stack_index, intermediate_arr_index);
				printf("\n%d  %d", instruction_stack[instruction_stack_index],intermediate_arr_index);
				construct_intermediate_table(intermediate_arr, &(intermediate_arr_index), opcode, values, val_index);
				printf("\n\n intermediate table");
				print_intermediate_table(intermediate_arr, intermediate_arr_index);



		}
		else if (opcode == 6 && strcmp(temp2_str,"ELSE")==0){
			val_index = 0;
			values[val_index++] = -1;
			instruction_push(instruction_stack, &instruction_stack_index, intermediate_arr_index);
			construct_intermediate_table(intermediate_arr, &(intermediate_arr_index), opcode, values, val_index);
			labeltable_push(label_table_stack, &label_table_stack_index, 2, intermediate_arr_index);



		}
		else if (opcode == 25){
			int k;
			labeltable_push(label_table_stack, &label_table_stack_index, -2, intermediate_arr_index);
			//if_else_evaluation(intermediate_arr, intermediate_arr_index, instruction_stack, instruction_stack_index, label_table_stack, label_table_stack_index);
			if (label_table_stack_index - 1 == -1 || label_table_stack[label_table_stack_index - 1].reference != 2){
				change_instr = instruction_pop(instruction_stack, &instruction_stack_index);
				star_value = labeltable_pop(label_table_stack, &label_table_stack_index);
				k = 0;
				while (intermediate_arr[change_instr].parameters[k] != -1)
					k++;

					intermediate_arr[change_instr].parameters[k] = star_value;
			}
			else{
			
				change_instr = instruction_pop(instruction_stack, &instruction_stack_index);
				star_value = labeltable_pop(label_table_stack, &label_table_stack_index);
				k = 0;
				while (intermediate_arr[change_instr].parameters[k] != -1)
					k++;

				intermediate_arr[change_instr].parameters[k] = star_value;
				change_instr = instruction_pop(instruction_stack, &instruction_stack_index);
				star_value = labeltable_pop(label_table_stack, &label_table_stack_index);
				k = 0;
				while (intermediate_arr[change_instr].parameters[k] != -1)
					k++;

					intermediate_arr[change_instr].parameters[k] = star_value;



			}
		}

		else if (opcode == 6){
			val_index = 0;
			values[val_index++]=jump_arr[temp3_str[0] - '0'];
			

			construct_intermediate_table(intermediate_arr, &(intermediate_arr_index), opcode, values, val_index);




		}
		else if (opcode == -1){
			jump_arr[temp1_str[0] - '0'] = intermediate_arr_index;
		}
		else{
			
			val_index = 0;
			i = 0;
			while (temp3_str[i] != '\0'){
				j = 0;
				while (temp3_str[i] == ' ')
					i++;
				flag2 = 0;
				while (temp3_str[i] != ','&& temp3_str[i] != '\0'){
					if (temp3_str[i] == '['){
						flag2 = i;
						
					}
					sample_str[j++] = temp3_str[i++];
				}
				
				sample_str[j] = '\0';
				if (register_code(sample_str) == -1){
					if (flag2 == 0)
						values[val_index++] = find_address_symtable(symbol_table_array, symboltable_index, sample_str);
					else{
						int i_temp= flag2 + 1;
						sum = 0;
						while (temp3_str[i_temp] != ']'){
							sum = sum * 10 + temp3_str[i_temp] - '0';
							i_temp++;
						}
						sample_str[1] = '\0';
						values[val_index++] = find_address_symtable(symbol_table_array, symboltable_index, sample_str) + sum;


					}

				}
				else
					values[val_index++] = register_code(sample_str);
				if (temp3_str[i] != '\0')
					i++;

			}

			construct_intermediate_table(intermediate_arr, &(intermediate_arr_index), opcode, values, val_index);
			printf("\n\n intermediate table");
			print_intermediate_table(intermediate_arr, intermediate_arr_index);
		}
	}
	printf("\n\n intermediate table");
	print_intermediate_table(intermediate_arr, intermediate_arr_index);
	/*
	while (!feof(fp)){
		fscanf(fp, "%[^\n]\n", temp1_str);
		//sscanf("temp1_str", "%s %s %s", var1, var2, var3);
		i = 0;
		while (temp1_str[i] != ' '){
			i++;
		}
		i++;
		j = 0;
		while (temp1_str[i] != '\0'){
			temp2_str[j++] = temp1_str[i++];
		}
		temp2_str[j] = '\0';
		create_symboltable(symbol_table_array, &symboltable_index, temp2_str, memory_index, SIZE);
		printf("\n===%d", symboltable_index);
		memory_index++;


		/*sum = 0;
		while (temp_str[i] != '\0'){
		sum = sum * 10 + temp_str[i] - '0';
		i++;
		}
		*/
	printf("\n symbol table");
	print_symbol_table(symbol_table_array, &symboltable_index);



}