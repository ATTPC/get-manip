//
//  UserInterface.h
//  get-manip
//
//  Created by Joshua Bradt on 5/25/14.
//  Copyright (c) 2014 NSCL. All rights reserved.
//

#ifndef __get_manip__UserInterface__
#define __get_manip__UserInterface__

#include <iostream>
#include <iomanip>
#include <string>
#include <assert.h>

#define LOG_ERROR std::cout << '\r' << std::string(40,' ') << '\r' << "Error: "
#define LOG_WARNING std::cout << '\r' << std::string(40,' ') << '\r' << "Warning: "

namespace UI {
	class ProgressBar;
    
    enum class LogLevel {
        Error = 0,
        Warning = 1,
        Info = 2,
        Debug = 3
    };
    
    class Logger;
}

class UI::ProgressBar
{
public:
    ProgressBar();
    ProgressBar(int i);
    
	void Write();
	void SetPercent(int pct_in);
	int GetPercent(int pct_in);
    int GetWidth();
    
    static int width;
    
private:
	int pct;
};

class UI::Logger
{
public:
    void SetLogLevel(LogLevel lvl);
    
    friend std::ostream& operator<<(UI::Logger& log, const char* msg);
    
private:
    static LogLevel currentLogLevel;
};



#endif /* defined(__get_manip__UserInterface__) */