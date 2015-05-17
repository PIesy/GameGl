#include "filereader.h"
#include <iostream>

bool FileReader::ReadByTemplate(std::string source, Scanner& scanner)
{
    std::fstream file;
    char buff[4096];
    buff[4095] = '\0';

    scanner.Clear();
    file.open(source);
    if(!file.is_open())
        return false;

    while(!file.eof())
    {
        file.getline(buff,4095);
        scanner.Analyze(buff);
    }
    return true;
}
