/**************************************************************************************************
Copyright (c) LaurentGomila (mateandmetal)
https://github.com/LaurentGomila/SFML/wiki/Source%3A-Simple-File-Logger-%28by-mateandmetal%29

Adapted by: Valentin Montmirail
Copyright (c) 2014, SATyr - Valentin Montmirail - Polytech Tours, France

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#ifndef FILELOGGER_H
#define FILELOGGER_H

/** We will change the version number here. because everyclass will include the FileLogger.h */
#define VERSION "0.1"


#include <iostream>
#include <fstream>

using namespace std;

/**
 * This class represents a basic Logger.
 * It allows 3 levels of logs: ERROR, WARNING, INFO.
 * We can easily add new level of log.
 * @author Laurent Gomila
 * @author Valentin Montmirail
 */
class FileLogger {

	public:

    	/** It represents the possible level of log */
    	enum  e_logType { LOG_ERROR, LOG_WARNING, LOG_INFO };

        /** 
         * It represent the constructor of a FileLogger
         * to create a FileLogger we need a version and the name of the file where everything will be written.
         * @param engine_version the number of version of the Logger (the application's version)
         * @param fname the name of the file where the log will be write.
         */
    	explicit FileLogger (const char *engine_version = VERSION, const char *fname = "default.log");

        /**
         * Represents the destructor of the logger, we close properly the file
         * and we free the memory used.
         * It will also write down the number of error, warning and info at the end of the file,
         * just before closing the file descriptor.
         */
    	~FileLogger ();

        /**
         * This is the overwrite of the operator<< for the Level of the Logger.
         * It will basically just write [Level_of_the_Logger] in the file, just before the log itself.
         * @param logger the Logger itself, with his file descriptor.
         * @param l_type the Level of the logger, because we need to write it in the file.
         * @return FileLogger& the reference of the Logger we use, in order to be able to do things like
         *         logger << "1" << "2" << "3" ... 
         */
    	friend FileLogger &operator << (FileLogger &logger, const e_logType l_type);

        /**
         * This is the overwrite of the operator<< for a string.
         * It will basically just write the string itself in the file.
         * @param logger the Logger itself, with his file descriptor.
         * @param text the log itself that we need to write in the file.
         * @return FileLogger& the reference of the Logger we use, in order to be able to do things like
         *         logger << "1" << "2" << "3" ... 
         */
    	friend FileLogger &operator << (FileLogger &logger, string text);

        /**
         * This is the overwrite of the operator<< for a string.
         * It will basically just write the char* itself in the file.
         * @param logger the Logger itself, with his file descriptor.
         * @param text the log itself that we need to write in the file.
         * @return FileLogger& the reference of the Logger we use, in order to be able to do things like
         *         logger << "1" << "2" << "3" ... 
         */
    	friend FileLogger &operator << (FileLogger &logger, const char *text);


    private:

        /** We need a file descriptor to write logs inside. */
        ofstream         myFile;

        /** We need to count the number of warning to write it down at the end of the file. */
        unsigned int     numWarnings;

        /** We need to count the number of error to write it down at the end of the file. */
        unsigned int     numErrors;

        /** We need to count the number of info to write it down at the end of the file. */
        unsigned int     numInfos;


};

/** We rename the enum, to be easily used in all the other class. */
typedef FileLogger::e_logType LogType;

#endif
