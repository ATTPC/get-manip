//
//  GETDataFile.cpp
//  GETConsolidate
//
//  Created by Joshua Bradt on 5/9/14.
//  Copyright (c) 2014 NSCL. All rights reserved.
//

#include "GETDataFile.h"

class ioError : public std::exception
{
private:
    const char* reasonString;

public:
    ioError(const char* reason) : reasonString(reason) {};
    virtual const char* what() const throw()
    {
        return reasonString;
    }
};

template<typename outType>
outType GETDataFile::ExtractByteSwappedInt(std::vector<uint8_t>::iterator begin,std::vector<uint8_t>::iterator end)
{
    outType result = 0;
    int n = 0;
    for (auto iter = end-1; iter >= begin; iter--) {
        result |= (*iter)<<(8*n);
        n++;
    }
    return result;
}

GETDataFile::GETDataFile(const boost::filesystem::path& filePath_in)
: filePath(filePath_in)
{
    // Check validity of file path
    if (not (exists(filePath) and is_regular_file(filePath))) {
        throw ioError("Invalid file path.");
    }
    if (filePath.extension() != ".graw") {
        throw ioError("File provided is not of extension .graw.");
    }
    
    // Extract info from path:
    // Expect path: /[data path]/timestamp/mm#/CoBo_AsAd#_[junk].graw
    
    std::string mmnStr = filePath.parent_path().string();
    
    coboId = std::stoi(&mmnStr.back()); // Last character
    
    std::string filename = filePath.filename().string();
    asadId = filename.at(filename.find("AsAd") + 4) - '0'; // this is kludgy
    
    // Open file stream
    
    filestream.open(filePath.string(), std::ios::in|std::ios::binary);
    if (filestream.bad()) {
        throw ioError("Failed to open filestream.");
    }
}

GETDataFile::~GETDataFile()
{
    filestream.close();
}

GETDataFile::GETDataFile(GETDataFile&& orig)
: filePath(orig.filePath), coboId(orig.coboId), asadId(orig.asadId)
{
    auto origPos = orig.filestream.tellg();
    orig.filestream.close();
    
    this->filestream.open(filePath.string(), std::ios::in|std::ios::binary);
    if (this->filestream.good()) {
        filestream.seekg(origPos);
    }
}

GETDataFile& GETDataFile::operator=(GETDataFile&& orig)
{
    if (this->filestream.is_open()) {
        this->filestream.close();
    }
    
    this->filePath = orig.filePath;
    this->coboId = orig.coboId;
    this->asadId = orig.asadId;
    
    auto origPos = orig.filestream.tellg();
    orig.filestream.close();
    
    this->filestream.open(filePath.string(), std::ios::in|std::ios::binary);
    if (this->filestream.good()) {
        filestream.seekg(origPos);
    }
    return *this;
}

std::vector<uint8_t> GETDataFile::GetNextRawFrame()
{
    std::vector<uint8_t> size_raw;
    uint16_t size;
    std::vector<uint8_t> frame_raw;
    
    // Test the input file for validity
    
    if (!filestream.good()) {
        throw ioError("Input file is bad.");
    }
    
    filestream.seekg(1,std::ios::cur); // Skip the metaType char
    
    for (int i = 0; i < 3; i++) {
        char temp;
        filestream.read(&temp, sizeof(uint8_t));
        size_raw.push_back((uint8_t)temp);
    }
    
    size = ExtractByteSwappedInt<uint32_t>(size_raw.begin(), size_raw.end());
    
    if (size == 0) throw ioError("Failed to read frame.");
    
    std::cout << "Found frame of size " << size << std::endl;
    
    filestream.seekg(-4,std::ios::cur); // rewind to start of frame
    
    for (unsigned long i = 0; i < size*64; i++) {
        char temp;
        filestream.read(&temp, sizeof(uint8_t));
        frame_raw.push_back((uint8_t)temp);
    }
    
    return frame_raw;
    // Leaves file pointer at end of frame. This assumes the frame size is correct.
}

uint8_t GETDataFile::GetFileCobo() const
{
    return coboId;
}

uint8_t GETDataFile::GetFileAsad() const
{
    return asadId;
}

bool GETDataFile::eof() const
{
    return filestream.eof();
}