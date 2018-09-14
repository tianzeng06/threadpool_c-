#ifndef THREADEXCEPTION_H
#define THREADEXCEPTION_H

#include <string>

class Threadexception
{
    public:
        typedef std::string string;
    public:
        Threadexception(const string &_msg=string(),int errRet=0);
        string what() const;
    private:
        string msg;
};

Threadexception::Threadexception(const string &_msg,int errRet):msg(_msg)
{
    msg+=strerror(errRet);//msg(_msg);
}

std::string Threadexception::what() const
{
    return msg;
}

#endif
