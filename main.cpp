#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "LibFile.h"

void ShowHelp(void);

LibFile lib;

std::fstream newfile;
char* indata;

uint32_t len;

uint8_t* lumpptr;
uint32_t lumpnum;

int main(int argc, char* argv[]) {
    printf("Libby for Shadowcaster v1.0 \n");
    printf("============================================================\n");

    if(argc <= 1) {
        ShowHelp();
    }

    else {
        //we want to extract a lump
        if(!strcmp("-x", argv[1])) {
            if(argc < 4) ShowHelp();
            else {
                lib.Open(argv[2]);
                lumpnum = lib.GetNumForName(argv[3]);
                newfile.open(argv[4], std::ios::out | std::ios::binary);
                if(!newfile.is_open()) {
                    printf("can't open input file %s \n", argv[4]);
                    exit(1);
                }
                else {
                    lumpptr = lib.GetPtr(lumpnum);
                    newfile.write((char*) lumpptr, lib.GetSize(lumpnum));
                    newfile.close();
                    lib.Close();
                    printf("wrote file %s (%d bytes)\n", argv[4], lib.GetSize(lumpnum));
                }
                
            }
        }

        //we want to replace a lump
        else if(!strcmp("-r", argv[1])) {
            if(argc < 4) ShowHelp();

            else {
                lib.Open(argv[2]);
                lumpnum = lib.GetNumForName(argv[3]);
                newfile.open(argv[3], std::ios::ate | std::ios::binary);
                len = std::filesystem::file_size(argv[3]);
                newfile.seekg(std::ios::beg);

                indata = (char*) malloc(len);
                newfile.read(indata, len);
                newfile.close();

                lib.WriteLump(lumpnum, (char*) indata, len);
                lib.Save(argv[4]);
                lib.Close();
                printf("wrote file %s (%d bytes)\n", argv[4], lib.GetSize(lumpnum));

            }
        }
    }
    return 0;
}

void ShowHelp(void) {
    printf("Usage: libby [-option] [infile] [lumpname] [outfile]\n");
    printf("============================================================\n");
    printf("                        Options:\n");
    printf("-x : extract a lump from infile, outfile is written\n");
    printf("-r : replace lump in infile with lumpname.ext, write outfile\n");
    printf("\n");
    exit(0);
}