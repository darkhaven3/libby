#include <cstdio>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include "LibFile.h"

LibFile::LibFile() {

}

LibFile::LibFile(const char* fname) {
    Open(fname);
}

LibFile::~LibFile() {

}

uint32_t LibFile::Open(const char* fname) {
    fsStream.open(fname, std::ios::in | std::ios::binary);
    if(fsStream.is_open()) {
        fsStream.seekg(-2, std::ios::end);
        fsStream.read((char*) &uNumEntries, 2);
        NewInfoTable(uNumEntries);
        Seek(0, FS_DIRECTORY);
        
        for(int i=0; i < uNumEntries; i++) {
            fsStream.read((char*) &fsInfo[i], 21);
        }

        for(int i=0; i < uNumEntries; i++) {
            if(fsInfo[i].size > 0) LumpPtrs[i] = (uint8_t*) malloc(fsInfo[i].size);
        }

        //now that the info table is prepared, read all the lumps at once
        for(int i=0; i < uNumEntries; i++) {
            fsStream.seekg(fsInfo[i].ofs, std::ios::beg);
            if(fsInfo[i].size > 0) fsStream.read((char*) LumpPtrs[i], fsInfo[i].size);
        }

        fsStream.close();
        return uNumEntries;
    }

    else return -1;
}

void LibFile::Close(void) {

}

void LibFile::Read(void* dest, uint32_t size, uint32_t ofs) {

}

uint32_t LibFile::GetNumForName(const char* name) {
    for(int i = uNumEntries; i > -1; i--) {
        if(!strcmp(name, fsInfo[i].name)) return i;
    }
    return -1;
}

uint32_t LibFile::GetNumEntries(void) {
    return uNumEntries;
}

uint32_t LibFile::GetSize(const char* name) {
    uint32_t num = GetNumForName(name);
    return GetSize(num);
}

uint32_t LibFile::GetSize(uint32_t num) {
    return fsInfo[num].size;
}

uint32_t LibFile::GetOffset(const char* name) {
    uint32_t num = GetNumForName(name);
    return GetOffset(num);
}

uint32_t LibFile::GetOffset(uint32_t num) {
    return fsInfo[num].ofs;
}

uint8_t* LibFile::GetPtr(const char* name) {
    uint32_t num = GetNumForName(name);
    return GetPtr(num);
}

uint8_t* LibFile::GetPtr(uint32_t num) {
    if(num > uNumEntries) {
        printf("%d is greater than numentries (%d)\n", num, uNumEntries);
        exit(1);
    }

    return LumpPtrs[num];
}

void LibFile::NewInfoTable(uint32_t num) {
    libinfo_t* ptr = (libinfo_t*) malloc(num * sizeof(libinfo_t));

    if(ptr == nullptr) {
        printf("LibFile: couldn't allocate a new infotable (size %d bytes)\n", (num * sizeof(libinfo_t)));
        exit(1);
    }
    else {
        fsInfo = ptr;
    }
}

uint8_t* LibFile::Load(const char* name) {
    uint32_t num = GetNumForName(name);

    return nullptr;
}

void LibFile::Seek(int32_t offset, uint32_t flags) {
    if(!fsStream.is_open()) {
        printf("Seek: this file isn't open\n");
        exit(1);
    }
    else {
        if(flags & FS_DIRECTORY) {
            offset += (uNumEntries * 21 + 2) * -1;
            fsStream.seekg(offset, std::ios::end);
        }

        else {
            fsStream.seekg(offset, std::ios::beg);
        }
    }
}

void LibFile::WriteLump(uint32_t num, void* data, uint32_t size) {
    if(num > uNumEntries) {
        printf("%d is greater than numentries (%d)\n", num, uNumEntries);
        exit(1);
    }
    else {
        free(LumpPtrs[num]);
        LumpPtrs[num] = (uint8_t*)malloc(size);
        fsInfo[num].size = size;
        fsInfo[num].ofs = 0;    //offset will be calculated on file save

        memcpy(LumpPtrs[num], data, size);
    }
}

void LibFile::Save(const char* fname) {
    std::fstream FS;
    uint32_t newofs;

    FS.open(fname, std::ios::out | std::ios::binary);
    if(!FS.is_open()) {
        printf("Couldn't open file to save! \n");
        exit(1);
    }
    else {
        newofs = 0;
        for(int i=0;i < uNumEntries; i++) {
            FS.write((char*) LumpPtrs[i], fsInfo[i].size);
            fsInfo[i].ofs = newofs;
            newofs += fsInfo[i].size;
        }

        //write the info table
        for(int j=0;j < uNumEntries;j++) {
            FS.write((char*) &(fsInfo[j].size), 4);
            FS.write((char*) &(fsInfo[j].ofs), 4);
            FS.write((char*) &(fsInfo[j].name), 13);
        }

        FS.write((char*) &uNumEntries, 2);

        FS.close();
    }
}