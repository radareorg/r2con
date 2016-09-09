/*
 ****************************************************************************
 *
 * simulavr - A simulator for the Atmel AVR family of microcontrollers.
 * Copyright (C) 2001, 2002 - 2012   Klaus Rudolph & other
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 ****************************************************************************
 *
 *  $Id$
 */
#define DEFAULT_BAUD 9600
#ifndef _MSC_VER
    // only, if BFD lib is used
#   include "config.h"
#endif

#include "elfio/elfio.hpp"

#include <string>
#include <cstring>
#include <map>
#include <limits>

#include "avrdevice_impl.h"
#include "avrsignature.h"
#include "serialrx.h"
#include "serialtx.h"
#include "simulavr_info.h"

#include "avrreadelf.h"

#ifdef _MSC_VER
// Stolen from http://developers.sun.com/solaris/articles/elf.html

#define EI_NIDENT     16
// Segment types
#define PT_NULL             0
#define PT_LOAD             1
#define PT_NOTE             4
#define PT_SHLIB            5
#define PT_PHDR             6
// Segment flags
#define PF_X                 1
#define PF_W                 2
#define PF_R                 4

typedef uint32_t  Elf32_Addr;
typedef uint16_t  Elf32_Half;
typedef uint32_t  Elf32_Off;
typedef int32_t   Elf32_Sword;
typedef uint32_t  Elf32_Word;

typedef struct {
    unsigned char e_ident[EI_NIDENT];    /* ident bytes */
    Elf32_Half e_type;                   /* file type */
    Elf32_Half e_machine;                /* target machine */
    Elf32_Word e_version;                /* file version */
    Elf32_Addr e_entry;                  /* start address */
    Elf32_Off e_phoff;                   /* phdr file offset */
    Elf32_Off e_shoff;                   /* shdr file offset */
    Elf32_Word e_flags;                  /* file flags */
    Elf32_Half e_ehsize;                 /* sizeof ehdr */
    Elf32_Half e_phentsize;              /* sizeof phdr */
    Elf32_Half e_phnum;                  /* number phdrs */
    Elf32_Half e_shentsize;              /* sizeof shdr */
    Elf32_Half e_shnum;                  /* number shdrs */
    Elf32_Half e_shstrndx;               /* shdr string index */
} Elf32_Ehdr;
// Segment header
typedef struct {
    Elf32_Word p_type;   /* entry type */
    Elf32_Off p_offset;  /* file offset */
    Elf32_Addr p_vaddr;  /* virtual address */
    Elf32_Addr p_paddr;  /* physical address */
    Elf32_Word p_filesz; /* file size */
    Elf32_Word p_memsz;  /* memory size */
    Elf32_Word p_flags;  /* entry flags */
    Elf32_Word p_align;  /* memory/file alignment */
} Elf32_Phdr;

void ELFLoad(AvrDevice * core) {
    FILE * f = fopen(core->GetFname().c_str(), "rb");
    if(f == NULL)
        avr_error("Could not open file: %s", core->GetFname().c_str());

    Elf32_Ehdr header;
    fread(&header, sizeof(header), 1, f);
    if(header.e_ident[0] != 0x7F || header.e_ident[1] != 'E'
        || header.e_ident[2] != 'L' || header.e_ident[3] != 'F')
        avr_error("File '%s' is not an ELF file", core->GetFname().c_str());
    // TODO: fix endianity in header
    if(header.e_machine != 83)
        avr_error("ELF file '%s' is not for Atmel AVR architecture (%d)", core->GetFname().c_str(), header.e_machine);

    for(int i = 0; i < header.e_phnum; i++) {
        fseek(f, header.e_phoff + i * header.e_phentsize, SEEK_SET);
        Elf32_Phdr progHeader;
        fread(&progHeader, sizeof(progHeader), 1, f);
        // TODO: fix endianity
        if(progHeader.p_type != PT_LOAD)
            continue;
        if((progHeader.p_flags & PF_X ) == 0 || (progHeader.p_flags & PF_R) == 0)
            continue;  // must be readable and writable
        if(progHeader.p_vaddr >= 0x80ffff)
            continue;  // not into a Flash
        if(progHeader.p_filesz != progHeader.p_memsz) {
            avr_error("Segment sizes 0x%x and 0x%x in ELF file '%s' must be the same",
                progHeader.p_filesz, progHeader.p_memsz, core->GetFname().c_str());
        }
        unsigned char * tmp = new unsigned char[progHeader.p_filesz];
        fseek(f, progHeader.p_offset, SEEK_SET);
        fread(tmp, progHeader.p_filesz, 1, f);

        core->Flash->WriteMem(tmp, progHeader.p_vaddr, progHeader.p_filesz);
        delete [] tmp;
    }

    fclose(f);
}

unsigned int ELFGetDeviceNameAndSignature(const char *filename, char *devicename) {
    return std::numeric_limits<unsigned int>::max();
}

#endif

void make_default_serial(AvrDevice * core){
	{
                        Net *net = new Net();
                        SerialTxFile *serial =
                          new SerialTxFile("-");
                        serial->SetBaudRate(DEFAULT_BAUD);
                        net->Add(core->GetPin("E0"));
                        net->Add(serial->GetPin("tx"));
	}
	{
                        Net *net = new Net();
                        SerialRxFile *serial =
                          new SerialRxFile("-");
                        serial->SetBaudRate(DEFAULT_BAUD);
                        net->Add(core->GetPin("E1"));
                        net->Add(serial->GetPin("rx"));
	}
	{
                        Net *net = new Net();
                        SerialRxFile *serial =
                          new SerialRxFile("/home/radare/workshop/ex3.4_simulator/serial1.txt");
                        serial->SetBaudRate(DEFAULT_BAUD);
                        net->Add(core->GetPin("D3"));
                        net->Add(serial->GetPin("rx"));
	}
}

#ifndef _MSC_VER
void ELFLoad(AvrDevice * core) {
    ELFIO::elfio reader;

    if(!reader.load(core->GetFname()))
        avr_error("File '%s' not found or isn't a elf object",
                  core->GetFname().c_str());

    if(reader.get_machine() != EM_AVR)
        avr_error("ELF file '%s' is not for Atmel AVR architecture (%d)",
                  core->GetFname().c_str(),
                  reader.get_machine());

    // over all symbols ...
    ELFIO::Elf_Half sec_num = reader.sections.size();

    for(ELFIO::Elf_Half i = 0; i < sec_num; i++) {
        ELFIO::section* psec = reader.sections[i];

        if(psec->get_type() == SHT_SYMTAB) {
            const ELFIO::symbol_section_accessor symbols(reader, psec);

            for(ELFIO::Elf_Xword j = 0; j < symbols.get_symbols_num(); j++) {
                std::string       name;
                ELFIO::Elf64_Addr value = 0;
                ELFIO::Elf_Xword  size = 0;
                unsigned char     bind = 0;
                unsigned char     type = 0;
                ELFIO::Elf_Half   section_index = 0;
                unsigned char     other = 0;

                // read symbol properties
                symbols.get_symbol(j, name, value, size, bind,
                                      type, section_index, other);

                // check zero length names
                if(name.length() == 0)
                    continue;

                // don't list section absolute symbols
                if(section_index == SHN_ABS)
                    continue;

                // don't list local symbols
                if((bind == STB_LOCAL) && (type != STT_NOTYPE))
                    continue;

                if(value < 0x800000) {
                    // range of flash space (.text)
                    std::pair<unsigned int, std::string> p(value >> 1, name);

                    core->Flash->AddSymbol(p);
                } else if(value < 0x810000) {
                    // range of ram (.data)
                    ELFIO::Elf64_Addr offset = value - 0x800000;
                    std::pair<unsigned int, std::string> p(offset, name);

                    core->data->AddSymbol(p);
                } else if(value < 0x820000) {
                    // range of eeprom (.eeprom)
                    ELFIO::Elf64_Addr offset = value - 0x810000;
                    std::pair<unsigned int, std::string> p(offset, name);

                    core->eeprom->AddSymbol(p);
                } else if(value < 0x820400) {
                    /* fuses space starting from 0x820000, do nothing */;
                } else if(value >= 0x830000 && value < 0x830400) {
                    /* lock bits starting from 0x830000, do nothing */;
                } else if(value >= 0x840000 && value < 0x840400) {
                    /* signature space starting from 0x840000, do nothing */;
                } else if(!strncmp("siminfo" , name.c_str(), 7)) {
                    /* SIMINFO symbol, do nothing */
                } else
                    avr_warning("Unknown symbol address range found! (symbol='%s', address=0x%lx)",
                                name.c_str(),
                                value);

            }
        }
        if(psec->get_name() == ".siminfo") {
            /*
             * You wonder why SIMINFO is read here, ignoring symbols?
             * Well, doing things this way is pretty independent from ELF
             * internals, other than finding the .siminfo section start pointer.
             * Accordingly, we can add pretty much anything, as long as the
             * interpretation here matches what's given in simulavr_info.h.
             */
            ELFIO::Elf_Xword filesize = psec->get_size();
            const char *data = psec->get_data();
            const char *data_ptr = data, *data_end = data + filesize;

            while(data_ptr < data_end) {
                char tag = *data_ptr;
                char length = *(data_ptr + 1);
                // Length check already done in ELFGetDeviceNameAndSignature().

                switch(tag) {
                  case SIMINFO_TAG_DEVICE:
                    // Device name. Handled in ELFGetDeviceNameAndSignature().
                    break;
                  case SIMINFO_TAG_CPUFREQUENCY:
                    core->SetClockFreq((SystemClockOffset)1000000000 /
                                       ((siminfo_long_t *)data_ptr)->value);
                    break;
                  case SIMINFO_TAG_SERIAL_IN:
                    avr_message("Connecting file %s as serial in to pin %s at %d baud.",
                                ((siminfo_serial_t *)data_ptr)->filename,
                                ((siminfo_serial_t *)data_ptr)->pin,
                                ((siminfo_serial_t *)data_ptr)->baudrate);
                    {
                        Net *net = new Net();
                        SerialTxFile *serial =
                          new SerialTxFile(((siminfo_serial_t *)data_ptr)->filename);
                        serial->SetBaudRate(((siminfo_serial_t *)data_ptr)->baudrate);
                        net->Add(core->GetPin(((siminfo_serial_t *)data_ptr)->pin));
                        net->Add(serial->GetPin("tx"));
                    }
                    break;
                  case SIMINFO_TAG_SERIAL_OUT:
                    avr_message("Connecting pin %s as serial out to file %s at %d baud.",
                                ((siminfo_serial_t *)data_ptr)->pin,
                                ((siminfo_serial_t *)data_ptr)->filename,
                                ((siminfo_serial_t *)data_ptr)->baudrate);
                    {
                        Net *net = new Net();
                        SerialRxFile *serial =
                          new SerialRxFile(((siminfo_serial_t *)data_ptr)->filename);
                        serial->SetBaudRate(((siminfo_serial_t *)data_ptr)->baudrate);
                        net->Add(core->GetPin(((siminfo_serial_t *)data_ptr)->pin));
                        net->Add(serial->GetPin("rx"));
                    }
                    break;
                  default:
                    avr_warning("Unknown tag in ELF .siminfo section: %hu", tag);
                }
                data_ptr += length;
            }
        }
    }

    make_default_serial(core);

    // load program, data and - if available - eeprom, fuses and signature
    ELFIO::Elf_Half seg_num = reader.segments.size();

    for(ELFIO::Elf_Half i = 0; i < seg_num; i++) {
        ELFIO::segment* pseg = reader.segments[i];

        if(pseg->get_type() == PT_LOAD) {
            ELFIO::Elf_Xword  filesize = pseg->get_file_size();
            ELFIO::Elf64_Addr vma = pseg->get_virtual_address();
            ELFIO::Elf64_Addr pma = pseg->get_physical_address();

            if(filesize == 0)
                continue;

            const unsigned char* data = (const unsigned char*)pseg->get_data();

            if(vma < 0x810000) {
                // read program, space below 0x810000 (.text)
                core->Flash->WriteMem(data, pma, filesize);
            } else if(vma >= 0x810000 && vma < 0x820000) {
                // read eeprom content, if available, space from 0x810000 to 0x820000 (.eeprom)
                unsigned int offset = vma - 0x810000;

                core->eeprom->WriteMem(data, offset, filesize);
            } else if(vma >= 0x820000 && vma < 0x820400) {
                // read fuses, if available, space from 0x820000 to 0x820400
                if(!core->fuses->LoadFuses(data, filesize))
                    avr_error("wrong byte size of fuses");
            } else if(vma >= 0x830000 && vma < 0x830400) {
                // read lock bits, if available, space from 0x830000 to 0x830400
                if(!core->lockbits->LoadLockBits(data, filesize))
                    avr_error("wrong byte size of lock bits");
            } else if(vma >= 0x840000 && vma < 0x840400) {
                // read and check signature, if available, space from 0x840000 to 0x840400
                if(filesize != 3)
                    avr_error("wrong device signature size in elf file, expected=3, given=%lu",
                              filesize);
                else {
                    unsigned int sig = (((data[2] << 8) + data[1]) << 8) + data[0];

                    if(core->GetDeviceSignature() != std::numeric_limits<unsigned int>::max() &&
                       sig != core->GetDeviceSignature())
                        avr_error("wrong device signature, expected=0x%x, given=0x%x",
                                  core->GetDeviceSignature(),
                                  sig);
                }
            }
        }
    }
}

unsigned int ELFGetDeviceNameAndSignature(const char *filename, char *devicename) {
    unsigned int signature = 0, sig_cli = 0, sig_elf = 0, sig_siminfo = 0;
    char siminfo_name[128];
    ELFIO::elfio reader;

    if(!reader.load(filename))
        avr_error("File '%s' not found or isn't a elf object", filename);

    if(reader.get_machine() != EM_AVR)
        avr_error("ELF file '%s' is not for Atmel AVR architecture (%d)",
                  filename,
                  reader.get_machine());

    // Search command line for signature.
    if(strcmp(devicename, "unknown")) {
        std::map<std::string, unsigned int>::iterator cur =
            AvrNameToSignatureMap.find(devicename);
        if(cur != AvrNameToSignatureMap.end()) {
            sig_cli = cur->second;
        } else {
            avr_warning("signature for device '%s' not found", devicename);
        }
    }

    // Search ELF binary for signature.
    ELFIO::Elf_Half seg_num = reader.segments.size();

    for(ELFIO::Elf_Half i = 0; i < seg_num; i++) {
        ELFIO::segment* pseg = reader.segments[i];

        if(pseg->get_type() == PT_LOAD) {
            ELFIO::Elf_Xword  filesize = pseg->get_file_size();
            ELFIO::Elf64_Addr vma = pseg->get_virtual_address();

            if(filesize == 0)
                continue;

            if(vma >= 0x840000 && vma < 0x840400) {
                // read and check signature, if available, space from 0x840000 to 0x840400
                if(filesize != 3)
                    avr_error("wrong device signature size in elf file, "
                              "expected=3, given=%lu", filesize);
                else {
                    const unsigned char* data = (const unsigned char*)pseg->get_data();

                    sig_elf = (((data[2] << 8) + data[1]) << 8) + data[0];

                    std::map<unsigned int, std::string>::iterator cur =
                        AvrSignatureToNameMap.find(sig_elf);
                    if(cur == AvrSignatureToNameMap.end()) {
                        avr_warning("unknown signature in ELF file: 0x%x",
                                    sig_elf);
                        sig_elf = 0;
                    }
                    break;
                }
            }
        }
    }

    // Search SIMINFO for device name.
    for(ELFIO::Elf_Half i = 0; i < seg_num; i++) {
        ELFIO::section* psec = reader.sections[i];

        if(psec->get_name() == ".siminfo") {
            ELFIO::Elf_Xword filesize = psec->get_size();
            const char *data = psec->get_data();
            const char *data_ptr = data, *data_end = data + filesize;

            while(data_ptr < data_end) {
                char tag = *data_ptr;
                char length = *(data_ptr + 1);
                if(length == 0)
                    avr_error("Field of zero length in .siminfo"
                              "section in ELF file found.");
                if(tag == SIMINFO_TAG_DEVICE) {
                    strncpy(siminfo_name,
                            ((siminfo_string_t *)data_ptr)->string, 128);
                    siminfo_name[127] = '\0'; // safety

                    std::map<std::string, unsigned int>::iterator cur =
                        AvrNameToSignatureMap.find(siminfo_name);
                    if(cur != AvrNameToSignatureMap.end()) {
                        sig_siminfo = cur->second;
                    }
                    else {
                        avr_warning("signature for device '%s' not found",
                                    siminfo_name);
                    }
                    break;
                }
                // Everything else handled in ELFLoad().
                data_ptr += length;
            }
        }
    }

    // Now we have searched all possible sources, take the one with the highest
    // precedence: command line over ELF over SIMINFO.
    if(sig_cli) {
        signature = sig_cli;
        if(sig_elf && sig_elf != sig_cli) {
            avr_warning("Command line device name '%s' taking precedence over "
                        "device signature 0x%x in ELF file.",
                        devicename, sig_elf);
        }
        if(sig_siminfo && sig_siminfo != sig_cli) {
            avr_warning("Command line device name '%s' taking precedence over "
                        "SIMINFO device name '%s'.", devicename, siminfo_name);
        }
    }
    else if(sig_elf) {
        signature = sig_elf;
        if(sig_siminfo && sig_siminfo != sig_elf) {
            avr_warning("Device signature 0x%x in ELF file taking precedence "
                        "over SIMINFO device name '%s'.",
                        sig_elf, siminfo_name);
        }
    }
    else if(sig_siminfo) {
        signature = sig_siminfo;
    }

    // Done.
    std::map<unsigned int, std::string>::iterator cur =
        AvrSignatureToNameMap.find(signature);
    if(cur != AvrSignatureToNameMap.end()) {
        strncpy(devicename, cur->second.c_str(), 1024);
    }
    avr_message("Device name is %s, signature 0x%x.", devicename, signature);

    return signature;
}

#endif

// EOF
