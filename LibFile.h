#ifndef __LIBFILE_H_
#define __LIBFILE_H_

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <fstream>

#define FS_DIRECTORY   0x0001

#define LIB_MAXLUMPS    16384

typedef struct {
    uint32_t size;
    uint32_t ofs;
    char name[13];
} libinfo_t;

class LibFile {
    private:
        std::fstream fsStream;
        libinfo_t* fsInfo;
        uint32_t uNumEntries;

        void NewInfoTable(uint32_t num);
        void Read(void* dest, uint32_t size, uint32_t ofs);
        void Seek(int32_t offset, uint32_t flags);
        uint32_t GetOffset(const char* name);
        uint32_t GetOffset(uint32_t num);

        uint8_t* LumpPtrs[LIB_MAXLUMPS];

    public:
        LibFile();
        LibFile(const char* fname);
        ~LibFile();
        uint32_t Open(const char* fname);
        void Close(void);
        uint32_t GetNumForName(const char* name);
        uint32_t GetNumEntries(void);
        uint32_t GetSize(const char* name);
        uint32_t GetSize(uint32_t num);
        uint8_t* GetPtr(const char* name);
        uint8_t* GetPtr(uint32_t num);
        uint8_t* Load(const char* name);
        void WriteLump(uint32_t num, void* data, uint32_t size);
        void Save(const char* fname);
};

#endif