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

#define VERSION "0.1"

#include <iostream>
#include <fstream>

using namespace std;


class FileLogger {

	public:

    	// If you can´t/dont-want-to use C++11, remove the "class" word after enum
    	enum  e_logType { LOG_ERROR, LOG_WARNING, LOG_INFO };

    	explicit FileLogger (const char *engine_version = VERSION, const char *fname = "default.log");

    	~FileLogger ();

    	friend FileLogger &operator << (FileLogger &logger, const e_logType l_type);

    	friend FileLogger &operator << (FileLogger &logger, string text);

    	friend FileLogger &operator << (FileLogger &logger, const char *text);

    private:

        std::ofstream           myFile;

        unsigned int            numWarnings;
        unsigned int            numErrors;
        unsigned int            numInfos;

};

typedef FileLogger::e_logType LogType;

#endif
