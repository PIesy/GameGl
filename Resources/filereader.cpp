#include "filereader.h"
#include <iostream>

void* FileReader::ReadByTemplate(std::string source, Scanner* scanner)
{
    std::fstream file;
    char buff[256];
    buff[255] = '\0';

    file.open(source);

    while(!file.eof())
    {
        file.getline(buff,255);
        scanner->Analyze(buff);
    }
    return scanner->Fill();
}
