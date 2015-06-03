/*
 * elf.h
 *
 *  Created on: Nov 8, 2013
 *      Author: liyixiao
 */

/**
 * Structures about ELF format taken from the System V Application Binary Interface.
 */

#pragma once

typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef int32_t  Elf32_Sword;

#define EI_NIDENT 16

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half    e_type;
    Elf32_Half    e_machine;
    Elf32_Word    e_version;
    Elf32_Addr    e_entry;
    Elf32_Off     e_phoff;
    Elf32_Off     e_shoff;
    Elf32_Word    e_flags;
    Elf32_Half    e_ehsize;
    Elf32_Half    e_phentsize;
    Elf32_Half    e_phnum;
    Elf32_Half    e_shentsize;
    Elf32_Half    e_shnum;
    Elf32_Half    e_shstrndx;
} Elf32_Ehdr;

typedef struct {
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off  sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
} Elf32_Shdr;

typedef struct {
    Elf32_Word      st_name;
    Elf32_Addr      st_value;
    Elf32_Word      st_size;
    unsigned char   st_info;
    unsigned char   st_other;
    Elf32_Half      st_shndx;
} Elf32_Sym;

typedef struct {
    Elf32_Word  p_type;
    Elf32_Off   p_offset;
    Elf32_Addr  p_vaddr;
    Elf32_Addr  p_paddr;
    Elf32_Word  p_filesz;
    Elf32_Word  p_memsz;
    Elf32_Word  p_flags;
    Elf32_Word  p_align;
} Elf32_Phdr;

typedef struct {
    Elf32_Addr  r_offset;
    Elf32_Word  r_info;
} Elf32_Rel;

typedef struct {
    Elf32_Addr  r_offset;
    Elf32_Word  r_info;
    Elf32_Sword r_addend;
} Elf32_Rela;

typedef unsigned char Elf32_R_Type;

enum SpecialSection {
    SECTION_BSS,
    SECTION_DATA,
    SECTION_GOT,
    SECTION_RODATA,
    SECTION_TEXT,
    SECTION_UNKNOWN
};

enum SectionType {
    SHT_NULL     = 0,
    SHT_PROGBITS = 1,
    SHT_SYMTAB   = 2,
    SHT_STRTAB   = 3,
    SHT_RELA     = 4,
    SHT_HASH     = 5,
    SHT_DYNAMIC  = 6,
    SHT_NOTE     = 7,
    SHT_NOBITS   = 8,
    SHT_REL      = 9,
};

enum SegmentType {
    PT_NULL = 0,
    PT_LOAD = 1
};

enum SegmentPermission {
    PF_X = 0x1,
    PF_W = 0x2,
    PF_R = 0x4
};

enum SymbolType {
	STT_OBJECT = 1,
    STT_FUNC   = 2,
};

#define ELF32_R_SYM(i)  ((i)>>8)
#define ELF32_R_TYPE(i)   ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t))

#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf)

static inline
const Elf32_Shdr* ELF32_SHDR(const Elf32_Ehdr *ehdr, uint32_t idx) {
    assert(idx < ehdr->e_shnum);
    return (const Elf32_Shdr*)((const uint8_t *)ehdr + ehdr->e_shoff + ehdr->e_shentsize * idx);
}

static inline
const Elf32_Phdr* ELF32_PHDR(const Elf32_Ehdr *ehdr, uint32_t idx) {
    assert(idx < ehdr->e_phnum);
    return (const Elf32_Phdr*)((const uint8_t *)ehdr + ehdr->e_phoff + ehdr->e_phentsize * idx);
}

static inline
const Elf32_Rel* ELF32_REL(const Elf32_Ehdr *ehdr, const Elf32_Shdr *shdr, uint32_t idx) {
    assert(idx < shdr->sh_size / shdr->sh_entsize);
    return (const Elf32_Rel*)((const uint8_t *)ehdr + shdr->sh_offset + shdr->sh_entsize * idx);
}

static inline
const Elf32_Rela* ELF32_RELA(const Elf32_Ehdr *ehdr, const Elf32_Shdr *shdr, uint32_t idx) {
    assert(idx < shdr->sh_size / shdr->sh_entsize);
    return (const Elf32_Rela*)((const uint8_t *)ehdr + shdr->sh_offset + shdr->sh_entsize * idx);
}

static inline
const Elf32_Sym* ELF32_SYM(const Elf32_Ehdr *ehdr, const Elf32_Shdr *shdr, uint32_t idx) {
    assert(idx < shdr->sh_size / shdr->sh_entsize);
    return (const Elf32_Sym*)((const uint8_t *)ehdr + shdr->sh_offset + shdr->sh_entsize * idx);
}

static inline
const char* ELF32_SH_NAME(const Elf32_Ehdr *ehdr, Elf32_Word sh_name) {
    const Elf32_Shdr* shstrtab = ELF32_SHDR(ehdr, ehdr->e_shstrndx);
    assert(sh_name < shstrtab->sh_size);
    return (const char*)((const uint8_t *)ehdr + shstrtab->sh_offset + sh_name);
}

static inline
const char* ELF32_STR_NAME(const Elf32_Ehdr *ehdr, const Elf32_Shdr* strtab, Elf32_Word name) {
    assert(name < strtab->sh_size);
    return (const char*)((const uint8_t *)ehdr + strtab->sh_offset + name);
}
