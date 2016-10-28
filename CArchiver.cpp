#include "CArchiver.h"
//#include <bzlib.h>
//#include <fstream>
#include <cstdlib>
#include <QDir>
#include <QDebug>
#include <QFileDialog>

void CArchiver::CompressFolder(const std::string &a_rFolder, const std::string& a_rDestination)
{
//    std::ifstream infile;
//    infile.open(a_rFolder.c_str());

//    const char* outFileName = std::string(a_rFolder + ".lm").c_str();
//    FILE* outFile = fopen(outFileName, "wb");
//    int error;
//    const int blockMultiplier = 7;

//    BZFILE* bzFile = BZ2_bzWriteOpen(&error, outFile, blockMultiplier, 0, 0);

//    const int buffsize = 10000;
//    char* buff = new char[buffsize];
//    ssize_t bytesRead;

//    while(infile.read(buff, buffsize))
//    {
//        BZ2_bzWrite(&error, bzFile, buff, buffsize);
//    }

//    BZ2_bzWriteClose(&error, bzFile, 0, NULL, NULL);

//    delete[] buff;

    // or "tar cjf a_rFolder.lm a_rFolder"


    QDir compressedFolder(a_rFolder.c_str());
    std::string folderName = compressedFolder.dirName().toStdString();
    compressedFolder.cdUp();

    system(std::string(std::string("tar -cjf ") +
                       a_rDestination + "/" + folderName + ".lm -C " +
                       compressedFolder.absolutePath().toStdString() + " " + folderName).c_str());
}


void CArchiver::ExtractArchive(const std::__1::string &a_rArchive)
{
    // tar xjf a_rfolder.lm
    system(std::string(std::string("tar xjf ") + a_rArchive + ".lm").c_str());
}
