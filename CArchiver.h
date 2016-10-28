#ifndef CZIPCOMPRESSOR_H
#define CZIPCOMPRESSOR_H

#include <string>

class CArchiver
{
public:
    static void CompressFolder(const std::string& a_rFolder, const std::string& a_rDestination);

    static void ExtractArchive(const std::string& a_rArchive);
};

#endif // CZIPCOMPRESSOR_H
