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

#include "../Headers/FileLogger.h"

	// ctor (remove parameters if you don´t need them)
    FileLogger::FileLogger (const char *engine_version, const char *fname) {
    	
    	numWarnings = 0;
        numInfos = 0;
        numErrors = 0;
           
        myFile.open (fname);

        // Write the first lines
        if (myFile.is_open()) {
            myFile << "PFE - SATyr, version " << engine_version << std::endl;
            myFile << std::endl;
        } // if
    }


	// dtor
    FileLogger::~FileLogger () {

        if (myFile.is_open()) {

            myFile << std::endl << std::endl;

            // Report number of errors, warnings, infos.
            myFile << numWarnings << " warnings" << std::endl;
            myFile << numErrors   << " errors"   << std::endl;
            myFile << numInfos    << " infos"    << std::endl;

            myFile.close();

        } // if
    }


    // Overload << operator using log type
    FileLogger &operator << (FileLogger &logger, const FileLogger::e_logType l_type) {

        switch (l_type) {

            case FileLogger::e_logType::LOG_ERROR:
                logger.myFile << "[ERROR]: ";
                ++logger.numErrors;
                break;

            case FileLogger::e_logType::LOG_WARNING:
                logger.myFile << "[WARNING]: ";
                ++logger.numWarnings;
                break;

            case FileLogger::e_logType::LOG_INFO:
                logger.myFile << "[INFO]: ";
                ++logger.numInfos;
                break;

            default:
                logger.myFile << "[UNKNOWN]: ";
                break;

        } // sw

		return logger;
    }


    // Overload << operator using C style strings
    // No need for std::string objects here
    FileLogger &operator << (FileLogger &logger, const char *text) {

        logger.myFile << text << std::endl;
        return logger;
    }

    // Overload << operator using real string
    FileLogger &operator << (FileLogger &logger, string text) {

        logger.myFile << text << std::endl;
        return logger;
    }
