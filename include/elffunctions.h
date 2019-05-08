#pragma once
#include <elf.h>

int check_elf_magic(Elf64_Ehdr* hdr){
	return ((hdr->e_ident[EI_MAG0] == ELFMAG0) && (hdr->e_ident[EI_MAG1] == ELFMAG1) &&
			(hdr->e_ident[EI_MAG2] == ELFMAG2) && (hdr->e_ident[EI_MAG3] == ELFMAG3));
}

Elf64_Shdr* get_elf_section_header(Elf64_Ehdr* hdr){
	return (Elf64_Shdr*)((long)hdr + hdr->e_shoff);
}

Elf64_Shdr* get_elf_section(Elf64_Ehdr* hdr, int index){
	return &(get_elf_section_header(hdr)[index]);
}

char* get_elf_string(Elf64_Ehdr* hdr, int section_idx, int string_idx){
	char* string_table_base = (char*)((long)hdr + get_elf_section(hdr, section_idx)->sh_offset);
	return string_table_base + string_idx;
}

Elf64_Sym* get_elf_symbol(Elf64_Ehdr* hdr, Elf64_Shdr* symbol_section, int index){
	return &((Elf64_Sym*)((long)hdr + symbol_section->sh_offset))[index];
}
