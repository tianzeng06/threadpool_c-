#ifndef HEADER_H
#define HEADER_H

#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <limits.h>

#include <queue>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

using namespace std;

inline void err_exit(const string &msg)
{
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

inline void err_quit(const string &msg)
{
    cerr<<msg<<endl;
    exit(EXIT_FAILURE);
}

inline void err_thread(const string &msg,int retno)
{
    cerr<<msg<<": "<<strerror(retno)<<endl;
    exit(EXIT_FAILURE);
}

inline void err_check(const string &msg,int retno)
{
    if(retno!=0)
        err_thread(msg,retno);
}

#endif //HEADER_H
