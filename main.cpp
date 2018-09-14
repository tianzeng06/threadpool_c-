#include "threadpool.h"
#include "header.h"

void *mytask(void *arg)
{
    int n=*(int *)arg;
    delete (int *)arg;

    cout<<"thread: "<<(unsigned long)pthread_self()<<" is working on task "<<n<<endl;
    sleep(6);//函数睡眠6秒，也就是让函数占用线程6秒
    cout<<"thread: "<<(unsigned long)pthread_self()<<" end of job.. "<<endl;
    return NULL;
}

int main()
{
    int numberthread;
    cout<<"Please input the MAX number to start thread: ";
    cin>>numberthread;
    Threadpool pool(numberthread);//初始化线程池中线程数量，默认36
    
    int numbertask;
    cout<<"Please input the number to start stask: ";
    cin>>numbertask;
    for(int i=1;i<=numbertask;++i)
    {
        pool.addtask(mytask,new int(i));//把任务加到任务队列
        sleep(1);//每个任务加入到任务队列后停一秒再开始加入下一个任务，
    }

    return 0;
}
