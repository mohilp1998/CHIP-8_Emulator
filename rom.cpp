#include "include/rom.h"

// Method for disabling DEBUGGING FILE TO FILE Basis
// #define DISABLE_DEBUG_LOGS
#ifdef DISABLE_DEBUG_LOGS
    #define fprintf(myDebugFile, fmt, ...) (0)
#endif

Rom::Rom(std::FILE *debugFile)
{
    //Constructor
    myDebugFile = debugFile;
}

Rom::~Rom()
{
    if (dataBuffer != nullptr)
    {
        std::free(dataBuffer);
        dataBuffer = nullptr;
    }
}

bool Rom::LoadRom(const char *romPath)
{
    // Clean dataBuffer of past entries
    if (dataBuffer != nullptr)
    {
        std::free(dataBuffer);
        dataBuffer = nullptr;
    }

    // Open the file in the read binary mode
    std::FILE *pFile;
    pFile = std::fopen(romPath, "rb");
    if (pFile == nullptr)
    {
        fprintf(myDebugFile,"[E] <rom.cpp>::Error reading ROM\n");
        return false;
    }

    // Check the file size
    std::fseek(pFile, 0, SEEK_END);              // Set ptr to end
    fileSize = std::ftell(pFile);                // Getting the size by end
    std::rewind(pFile);                          // Set the ptr to beginning
    fprintf(myDebugFile,"[I] <rom.cpp>::File Size is %ld\n", fileSize);

    // Allocate buffer of the size
    dataBuffer = (unsigned char*) std::malloc(sizeof(char) * fileSize);
    if (dataBuffer == nullptr)
    {
        fprintf(myDebugFile,"[E] <rom.cpp>::Unable to assign memory\n");
        return false;
    }

    // Write the data into the buffer
    size_t result = std::fread(dataBuffer, 1, fileSize, pFile);
    if (result != fileSize)
    {
        fprintf(myDebugFile,"[E] <rom.cpp>::Error in copying File to buffer\n");
        return false;
    }

    fprintf(myDebugFile,"[I] <rom.cpp>::ROM Loaded Successfully\n");
    // Close the file
    std::fclose(pFile);

    return true;
}

bool Rom::getRomData(std::vector<unsigned char>& memory)
{
    // Check if dataBuffer has data
    if (dataBuffer == nullptr)
    {
        fprintf(myDebugFile,"[E] <rom.cpp>::No data loaded in the Rom\n");
        return false;
    }

    // Copying data to the memory
    if (fileSize > (memory.size()-512))
    {
        fprintf(myDebugFile,"[E] <rom.cpp>::ROM greater than available memory\n");
        return false;
    }

    for (unsigned short i = 0; i < fileSize; i++)
    {
        memory[512+i] = dataBuffer[i];
    }

    fprintf(myDebugFile,"[I] <rom.cpp>::ROM loaded successfully in memory\n");
    return true;
    
}