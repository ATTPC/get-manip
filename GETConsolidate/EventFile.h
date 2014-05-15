//
//  EventFile.h
//  GETConsolidate
//
//  Created by Joshua Bradt on 5/9/14.
//  Copyright (c) 2014 NSCL. All rights reserved.
//

#ifndef __GETConsolidate__EventFile__
#define __GETConsolidate__EventFile__

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "Event.h"
#include "GETExceptions.h"

class EventFile
{
private:
    std::map<unsigned int,unsigned long long> offsetTable; // evt# : offset
    std::map<unsigned int,unsigned long long>::iterator currentEvt;
    
    std::fstream file;
    
    // Magic string for file ID: EVTFILE\0
    
    static const char* magic; // 8 char, null-term
    
    bool isInitialized = false;
    
public:
    EventFile();
    ~EventFile();
    
    EventFile(EventFile& orig) =delete;
    EventFile(EventFile&& orig) =delete;
    
    EventFile& operator=(EventFile& other) =delete;
    EventFile& operator=(EventFile&& other) =delete;
    
    void CloseFile();
    
    // Functions for writing events to file
    void OpenFileForWrite(std::string path);
    void WriteEvent(const Event& event);
    
    // Functions for reading events from a file
    void OpenFileForRead(const std::string path);
    void RebuildIndex();
    Event ReadEvent();
    Event GetNextEvent();
    Event GetPreviousEvent();
    Event GetEventByNumber(const unsigned int evtNumber);
};

#endif /* defined(__GETConsolidate__EventFile__) */
