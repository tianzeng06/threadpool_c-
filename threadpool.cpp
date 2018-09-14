#include "threadpool.h"
//#include <pthread.h>

Threadpool::Threadpool(int _maxthreads,unsigned int _waitseconds)
    :maxthreads(_maxthreads),counter(0),idle(0),waitseconds(_waitseconds),
        quit(false){}

//线程入口，相当于消费者，不断的消费任务（执行任务）
void *thread_routine(void *arg)
{
    pthread_detach(pthread_self());//将子线程设置为分离状态，省去用jion
    cout<<"thread: "<<(unsigned long)pthread_self()<<" is starting."<<endl;

    Threadpool *pool=(Threadpool *)arg;

    //等待任务的到来
    while(true)
    {
        bool timeout=false;
        
        pool->ready.lock();
        //当处于等待的时候，空闲的线程又多了一个
        ++pool->idle;

        //pool->ready中的条件变量三个作用：
        //1.等待任务队列中有任务到来
        //2.等待线程池销毁通知
        //3.确保等待超时的时候，能够将线程池销毁（线程退出）
        while(pool->taskqueue.empty()&&pool->quit==false)
        {
            cout<<"thread: "<<(unsigned long)pthread_self()<<" is waitting..."<<endl;
            //等待waitseconds.wait!!!
            if(pool->ready.timedwait(pool->waitseconds)!=0)
            {
                //如果等待超时
                cout<<"thread: "<<(unsigned long)pthread_self()<<" is wait timeout..."<<endl;
                timeout=true;
                break;//跳出循环，执行到下一个if处
            }
        }

        //条件成熟（等待结束），线程开始执行任务或者是线程销毁，则说明空闲的线程又少了一个
        --pool->idle;

        //状态3.如果等待超时（一般此时任务队列已经空了）
        if(timeout==true&&pool->taskqueue.empty())
        {
            --pool->counter;
            pool->ready.unlock();//解锁跳出循环,直接销毁线程（退出线程）
            break;
        }

        //状态2.如果等待到了线程的销毁通知，且任务都执行完毕了
        if(pool->quit==true&&pool->taskqueue.empty())
        {
            --pool->counter;//如果没有线程了，则给线程发通知告诉线程池，池中没有线程了
            if(pool->counter==0)
                pool->ready.signal();//signal
            pool->ready.unlock();
            break;//解锁跳出循环
        }

        //状态1.如果有任务，执行任务
        if(!(pool->taskqueue.empty()))
        {
            //从队头取出任务进行处理
            Threadpool::task_t *t=pool->taskqueue.front();
            pool->taskqueue.pop();

            //任务需要执行一定的时间，解锁便于其他生产者可以继续生产任务，其他消费者可以消费任务
            pool->ready.unlock();
            
            //处理任务,运行mytask函数
            t->run(t->arg);
            delete t;
        }
    }
    
    //跳出循环后打印退出信息，然后销毁线程
    cout<<"thread: "<<(unsigned long)pthread_self()<<" is exiting..."<<endl;
    pthread_exit(NULL);
}

//添加任务函数，类似于一个生产者，不断将任务生成，挂到任务队列上，等待消费者线程进行消费
void Threadpool::addtask(callback_t run,void *arg)
{
    //1.生成任务并添加到任务队列的队尾
    task_t *newtask=new task_t{run,arg};

    ready.lock();//保护共享变量
    taskqueue.push(newtask);

    //2.让线程开始执行任务
    starttask();
    ready.unlock();//解锁使任务开始执行
}

//线程启动函数
void Threadpool::starttask()
{
    //如果都等待的线程唤醒其中一个
    if(idle>0)
        ready.signal();

    //没有等待的线程，而且当前线程总数没有到大最大值，就创建一个
    //如果达到线程最大总数还想创建线程，改成else
    else if(counter<maxthreads);
    {
        pthread_t tid;
        pthread_create(&tid,NULL,thread_routine,this);//this指向本类对象
        ++counter;
    }
}

Threadpool::~Threadpool()
{
    if(quit==true)
        return;

    ready.lock();
    quit=true;
    if(counter>0)
    {
        //对处于等待状态给他们发送通知，这些处于等待状态的线程，会收到通知直接退出
        if(idle>0)
            ready.broadcast();

        //对于正在执行任务的线程，他们收不到通知，需要执行完任务
        while(counter>0)
            ready.wait();
    }
    ready.unlock();
}

