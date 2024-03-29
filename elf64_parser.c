// i/o and string operations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// file handling
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// necessary structures for elf header
#include <elf.h>

#define _ELF_MAGIC "\x7f\x45\x4c\x46"

// structure which holds all the section structures and additional information
typedef struct {
  Elf64_Ehdr* ehdr;
  Elf64_Phdr* phdr;
  Elf64_Shdr* shdr;
  void* strtab;
  void* buffer;
  size_t size;
} elf64_image;

static elf64_image* elf64_parse(void* buffer, size_t size) {
  if (memcmp(buffer, _ELF_MAGIC, 4)) {
    puts("Error: not an ELF binary");
    return 0;
  }
  elf64_image* binary = calloc(1, sizeof(elf64_image));

  // update buffer and size information for elf image structure
  binary->buffer = buffer;
  binary->size = size;

  // cast binary header as elf header
  binary->ehdr = (Elf64_Ehdr *)(buffer);

  // use program header offset in elf header to locate program header
  binary->phdr = (Elf64_Phdr *)(buffer + binary->ehdr->e_phoff);

  // use section header offset in elf header to locate section header
  binary->shdr = (Elf64_Shdr *)(buffer + binary->ehdr->e_shoff);

  // find string table
  binary->strtab = buffer + binary->shdr[binary->ehdr->e_shstrndx].sh_offset;

  return binary;
}

// this function loads file into memory then passes to elf64_parse
static elf64_image* elf64_load_file(const char* path) {
  // stat() system call to find size of file
  struct stat finfo;
  if (stat(path, &finfo) == -1) {
    printf("Error: stat() returned error for file %s\n", path);
    return 0;
  }

  // load file directly into memory via mmap
  void* buffer = mmap(0, finfo.st_size, PROT_READ, MAP_PRIVATE,
    open(path, O_RDONLY), 0);

  // if buffer then return result of elf64_parse
  if (buffer)
    return elf64_parse(buffer, finfo.st_size);
  // if failed, then return 0
  return 0;
}

static void elf64_destroy(elf64_image* elf) {
  if (elf) {
    munmap(elf->buffer, elf->size);
    free(elf);
  }
  return;
}

static void elf64_dump(elf64_image* elf) {
  if (elf) {
    // dump ELF header
    printf("ELF Header:\n\tMagic: ");
    for (int i = 0; i < EI_NIDENT; ++i)
      printf("%#lx ", elf->ehdr->e_ident[i]);

    printf("\n\tType: %d\n", elf->ehdr->e_type);
    printf("\tMachine: %d\n", elf->ehdr->e_machine);
    printf("\tVersion: %d\n", elf->ehdr->e_version);
    printf("\tEntry: %#lx\n", elf->ehdr->e_entry);
    printf("\tProgram Header Offset: %#lx\n", elf->ehdr->e_phoff);
    printf("\tSection Header Offset: %#lx\n", elf->ehdr->e_shoff);
    printf("\tFlags: %d\n", elf->ehdr->e_flags);
    printf("\tELF Header Size: %d\n", elf->ehdr->e_ehsize);
    printf("\tProgram Header Size: %d\n", elf->ehdr->e_phentsize);
    printf("\tProgram Header Length: %d\n", elf->ehdr->e_phnum);
    printf("\tSection Header Size: %d\n", elf->ehdr->e_shentsize);
    printf("\tSection Header Length: %d\n", elf->ehdr->e_shnum);
    printf("\tString Table Index: %#lx\n", elf->ehdr->e_shstrndx);

    puts("\nSection Headers:");
    for (int i = 1; i < elf->ehdr->e_shnum; ++i)
      printf("\t%s \tOffset: %#lx\n",
        elf->strtab + elf->shdr[i].sh_name, elf->shdr[i].sh_offset);

    puts("\nProgram Headers:");
    for (int i = 0; i < elf->ehdr->e_phnum; ++i)
      printf("\tType: %d\tOffset: %d\tAddress: %#lx\n",
        elf->phdr[i].p_type,
        elf->phdr[i].p_offset,
        elf->phdr[i].p_vaddr);
  }
  return;
}

int main(int argc, char** argv) {
  if (argv[1]) {
    elf64_image* binary = elf64_load_file(argv[1]);
    if (binary)
      elf64_dump(binary);
    elf64_destroy(binary);
  } else
    printf("Usage: %s <elf binary>\n", argv[0]);
  return 0;
}
