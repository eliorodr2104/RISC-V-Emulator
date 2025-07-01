//
// Created by Eliomar Alejandro Rodriguez Ferrer on 01/07/25.
//

#include <stdlib.h>

#include "args_handler.h"
#include "elf.h"

#include <string.h>
#include <unistd.h>

int load_elf_sections(const char* filepath, options_t* opts) {
    FILE* f = fopen(filepath, "rb");
    if (!f) {
        perror("fopen");
        return -1;
    }

    Elf32_Ehdr ehdr;
    if (fread(&ehdr, sizeof(ehdr), 1, f) != 1) {
        fprintf(stderr, "Errore lettura ELF header\n");
        fclose(f);
        return -1;
    }

    if (ehdr.e_ident[0] != 0x7f || ehdr.e_ident[1] != 'E' ||
        ehdr.e_ident[2] != 'L' || ehdr.e_ident[3] != 'F') {
        fprintf(stderr, "File non è un ELF valido\n");
        fclose(f);
        return -1;
        }

    opts->entry_point = ehdr.e_entry;

    if (fseek(f, ehdr.e_shoff, SEEK_SET) != 0) {
        fprintf(stderr, "Errore seek section headers\n");
        fclose(f);
        return -1;
    }

    Elf32_Shdr* sections = malloc(sizeof(Elf32_Shdr) * ehdr.e_shnum);
    if (!sections) {
        fprintf(stderr, "Errore allocazione memoria sections\n");
        fclose(f);
        return -1;
    }

    if (fread(sections, sizeof(Elf32_Shdr), ehdr.e_shnum, f) != ehdr.e_shnum) {
        fprintf(stderr, "Errore lettura section headers\n");
        free(sections);
        fclose(f);
        return -1;
    }

    // Controllo validità dell'indice string table
    if (ehdr.e_shstrndx >= ehdr.e_shnum) {
        fprintf(stderr, "Indice string table non valido\n");
        free(sections);
        fclose(f);
        return -1;
    }

    const Elf32_Shdr strtab_hdr = sections[ehdr.e_shstrndx];
    char* shstrtab = malloc(strtab_hdr.sh_size);
    if (!shstrtab) {
        fprintf(stderr, "Errore allocazione string table\n");
        free(sections);
        fclose(f);
        return -1;
    }

    if (fseek(f, strtab_hdr.sh_offset, SEEK_SET) != 0) {
        fprintf(stderr, "Errore seek string table\n");
        free(sections);
        free(shstrtab);
        fclose(f);
        return -1;
    }

    if (fread(shstrtab, 1, strtab_hdr.sh_size, f) != strtab_hdr.sh_size) {
        fprintf(stderr, "Errore lettura string table\n");
        free(sections);
        free(shstrtab);
        fclose(f);
        return -1;
    }

    for (int i = 0; i < ehdr.e_shnum; i++) {
        // Controllo bounds del nome sezione
        if (sections[i].sh_name >= strtab_hdr.sh_size) {
            continue; // Salta sezioni con nomi non validi
        }

        const char* name = shstrtab + sections[i].sh_name;

        if (strcmp(name, ".text") == 0 || strcmp(name, ".data") == 0) {

            uint8_t* buf = malloc(sections[i].sh_size);
            if (!buf) {
                fprintf(stderr, "Errore allocazione memoria per sezione %s\n", name);
                continue;
            }

            if (fseek(f, sections[i].sh_offset, SEEK_SET) != 0) {
                fprintf(stderr, "Errore seek sezione %s\n", name);
                free(buf);
                continue;
            }

            if (fread(buf, 1, sections[i].sh_size, f) != sections[i].sh_size) {
                fprintf(stderr, "Errore lettura sezione %s\n", name);
                free(buf);
                continue;
            }

            if (strcmp(name, ".text") == 0) {
                opts->text_data = buf;  // Mantieni come uint8_t*
                opts->text_size = sections[i].sh_size;
                opts->text_vaddr = sections[i].sh_addr;

            } else {
                opts->data_data = buf;  // Mantieni come uint8_t*
                opts->data_size = sections[i].sh_size;
                opts->data_vaddr = sections[i].sh_addr;

            }
        }
    }

    free(sections);
    free(shstrtab);
    unlink(filepath);
    fclose(f);

    return 0;
}
