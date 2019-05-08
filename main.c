#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/queue.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include "include/elffunctions.h"

#define DEBUGGING 0

void get_imported_symbols(char* elf_path, char*** function_relocation_symbols, int* function_relocation_symbols_index){
	Elf64_Ehdr hdr;
	Elf64_Shdr* dynamic_section;		// the section that lists the dependencies of the opened ELF file
	Elf64_Shdr* symbol_section; 		// the .dynsym section
	Elf64_Shdr* string_table_section;	// the dynamic string table section
	struct stat st;
	Elf64_Shdr** relocation_sections = malloc(sizeof(Elf64_Shdr*));
	int relocation_section_index = 0;
	FILE* fp = fopen(elf_path, "r");
	
	if (fp == NULL){
		printf("\nError. ELF file not found: %s\n", elf_path);
		return;
	} else {
		printf("Ok.\n");
	}
	
	fread(&hdr, sizeof(hdr), 1, fp);
	printf("Reading ELF header... ");

	//check if argument is an ELF file
	if (!check_elf_magic(&hdr)){
		printf(" ELF Magic number is invalid.\n");
		exit(1);
	} else{
		printf("Ok.\n");
	}

	//checking the ELF machine target instruction set
	if (!(hdr.e_machine == EM_X86_64)){
		printf("ELF file has not x86-64 instruction set.");
		exit(1);
	}

	//load the ELF file into the memory
	fstat(fileno(fp), &st);
	Elf64_Ehdr* elf = malloc(st.st_size);
	fseek(fp, 0L, SEEK_SET);
	fread(elf, sizeof(elf), st.st_size, fp);
	printf("Copied ELF into memory.\n");

	//iterate over all sections and find the relocation section
	for (int i = 0; i < hdr.e_shnum; i++){
		Elf64_Shdr* section = get_elf_section(elf, i);
		if (DEBUGGING){
			printf("Section name: %s\n", get_elf_string(elf,  elf->e_shstrndx, section->sh_name));
			printf("Section name addr: %p\n", get_elf_string(elf, elf->e_shstrndx, section->sh_name));
		}

		if (section->sh_type == SHT_RELA){
			if (DEBUGGING){
				printf("Relocation Section found\n");
			}
			symbol_section = get_elf_section(elf, section->sh_link); // typically .dynsym section
			string_table_section = get_elf_section(elf, symbol_section->sh_link); // the string table for the symbol section
			
			// we only have a low number of relocation sections, therefore realloc() does not 
			// consume too much time
			relocation_sections = realloc(relocation_sections, (relocation_section_index + 1) * sizeof(Elf64_Shdr*));
			relocation_sections[relocation_section_index] = section;
			relocation_section_index++;
			
			if (DEBUGGING){
				printf("%ld\n", section->sh_size / section->sh_entsize);
			}

			// iterate through all relocations of this section
			for (int j = 0; j < section->sh_size / section->sh_entsize; j++){
				Elf64_Rela* rel = &((Elf64_Rela*)((long)elf + section->sh_offset))[j];
				int symbol_string_table_idx = get_elf_symbol(elf, symbol_section, ELF64_R_SYM(rel->r_info))->st_name;
				char* symbol_string = get_elf_string(elf, symbol_section->sh_link, symbol_string_table_idx);
				if (DEBUGGING){
					printf("Relocation symbol index: %ld\n", ELF64_R_SYM(rel->r_info));
					printf("Relocation string table index: %d\n", symbol_string_table_idx);
					printf("Relocation symbol: %s\n", symbol_string);
				}

				// get the relocation type
				if (ELF64_R_TYPE(rel->r_info) == R_X86_64_JUMP_SLOT){
					// don't use trampolines for functions in the dynamic loader
					// these would cause problems when resolving the original trampoline
					// addresses
					if (strstr(symbol_string, "dlsym") || strstr(symbol_string, "_dl")){
						continue;
					}

					if (DEBUGGING){
						printf("Found jump relocation:\n");
						printf("%s\n", symbol_string);
						printf("%d\n", *function_relocation_symbols_index);
					}
					printf("%s\n", symbol_string);
					// search the list if we have found the symbol earlier
					int prev_found = 0;
					for (int k = 0; k < *function_relocation_symbols_index; k++){
						if (strcmp((*function_relocation_symbols)[k], symbol_string) == 0){
							prev_found = 1;
							break;
						}
					}
					// add the symbol to the function_relocation_symbols list
					if (prev_found == 0){
						*function_relocation_symbols = realloc(*function_relocation_symbols, (*function_relocation_symbols_index + 1) * sizeof(char*));
						(*function_relocation_symbols)[*function_relocation_symbols_index] = symbol_string;
						*function_relocation_symbols_index = *function_relocation_symbols_index + 1;
					}
				}
				if (DEBUGGING){
					printf("\n");
				}
			}
		}

		if (section->sh_type == SHT_DYNAMIC){
			dynamic_section = get_elf_section(elf, i);
		}
	}
}

void create_trampoline_prolog(FILE* fp_asm, char*** function_relocation_symbols, int i){
	// the data for this function
	fprintf(fp_asm, "\t.section\t.rodata\n");
	fprintf(fp_asm, ".LC%d:\n", i + 50);
	fprintf(fp_asm, "\t.string \"%s\"\n", (*function_relocation_symbols)[i]);

	// the code for this function
	fprintf(fp_asm, "\t.text\n");
	fprintf(fp_asm, "\t.globl\t%s\n", (*function_relocation_symbols)[i]);
	fprintf(fp_asm, "\t.type\t%s\t, @function\n", (*function_relocation_symbols)[i]);
	fprintf(fp_asm, "%s:\n", (*function_relocation_symbols)[i]);
	fprintf(fp_asm, ".LFB%d:\n", i + 50);
}

void create_trampoline_call(FILE* fp_asm, char*** function_relocation_symbols, int i){
	fprintf(fp_asm, "	movq	trampoline_original_addr@GOTPCREL(%%rip), %%r11\n");
	//fprintf(fp_asm, "	movq	(%%r11), %%r11\n");
	fprintf(fp_asm, "	movq	%d(%%r11), %%r11\n", (i * 8));
	fprintf(fp_asm, "	jmp	*%%r11\n");
}

int main(int argc, char* argv[], char* envp[]){
	if (argc < 2){
		printf("Pass an target ELF file as parameter!\n");
		exit(1);
	}

	//the strings of the functions that have been imported
	char** function_relocation_symbols = malloc(sizeof(char*));
	int function_relocation_symbols_index = 0;

	get_imported_symbols(argv[1], &function_relocation_symbols, &function_relocation_symbols_index);

	FILE* fp_symbols = fopen("build/symbols", "w");
	printf("\nELF imports the following %d functions:\n", function_relocation_symbols_index);
	for (int i = 0; i < function_relocation_symbols_index; i++){
		printf("%s\n", function_relocation_symbols[i]);
		fprintf(fp_symbols, "%s\n", function_relocation_symbols[i]);
	}
	fclose(fp_symbols);

	// create the assembly code
	FILE* fp_asm = fopen("build/asm.s", "w");
	if (fp_asm == NULL){
		printf("Could not create shared library assembly file");
	}
	
	// copy the contents of the template file into the lib asm file
	FILE* fp_template = fopen("./asm/startup.s", "r");
	char c = fgetc(fp_template);
	while (c != EOF){
		fputc(c, fp_asm);
		c = fgetc(fp_template);
	}

	for (int i = 0; i < function_relocation_symbols_index; i++){
		create_trampoline_prolog(fp_asm, &function_relocation_symbols, i);

		create_trampoline_call(fp_asm, &function_relocation_symbols, i);	

		printf("Imported trampoline asm for function symbol: %s\n", function_relocation_symbols[i]);
	}

	fprintf(fp_asm, "	.section\t.note.GNU-stack,\"\",@progbits\n");

	printf("\nFinished...\n");
	return 0;
}
