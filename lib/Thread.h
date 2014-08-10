#pragma once

#include <sys/time.h>
#include <pthread.h>

#include <unordered_set>


inline timespec gettimeoffset ( long milliseconds )
{
    timeval tv;
    gettimeofday ( &tv, 0 );

    timespec ts;
    ts.tv_sec = tv.tv_sec + ( milliseconds / 1000L );
    ts.tv_nsec = tv.tv_usec * 1000L + ( milliseconds % 1000L ) * 1000000L;

    return ts;
}


class Mutex
{
    pthread_mutex_t mutex;

public:

    Mutex()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init ( &attr );
        pthread_mutexattr_settype ( &attr, PTHREAD_MUTEX_RECURSIVE );
        pthread_mutex_init ( &mutex, &attr );
    }

    ~Mutex()
    {
        pthread_mutex_destroy ( &mutex );
    }

    void lock()
    {
        pthread_mutex_lock ( &mutex );
    }

    void unlock()
    {
        pthread_mutex_unlock ( &mutex );
    }

    friend class CondVar;
};


class Lock
{
    Mutex& mutex;

public:

    Lock ( Mutex& mutex ) : mutex ( mutex )
    {
        mutex.lock();
    }

    ~Lock()
    {
        mutex.unlock();
    }
};


#define LOCK(MUTEX) Lock lock ## MUTEX ( MUTEX )


class CondVar
{
    pthread_cond_t cond;

public:

    CondVar()
    {
        pthread_cond_init ( &cond, 0 );
    }

    ~CondVar()
    {
        pthread_cond_destroy ( &cond );
    }

    int wait ( Mutex& mutex )
    {
        return pthread_cond_wait ( &cond, & ( mutex.mutex ) );
    }

    int wait ( Mutex& mutex, long timeout )
    {
        timespec ts = gettimeoffset ( timeout );
        return pthread_cond_timedwait ( &cond, & ( mutex.mutex ), &ts );
    }

    void signal()
    {
        pthread_cond_signal ( &cond );
    }

    void broadcast()
    {
        pthread_cond_broadcast ( &cond );
    }
};


class Thread
{
    mutable Mutex mutex;
    bool running = false;
    pthread_t thread;
    static void *func ( void *ptr );

public:

    Thread() {}
    virtual ~Thread() { join(); }

    virtual void start();

    virtual void join();

    void release()
    {
        LOCK ( mutex );
        running = false;
    }

    bool isRunning() const
    {
        LOCK ( mutex );
        return running;
    }

    virtual void run() = 0;
};


#define THREAD(NAME, CONTEXT)                                   \
    class NAME : public Thread {                                \
        CONTEXT& context;                                       \
    public:                                                     \
        NAME ( CONTEXT& context ) : context ( context ) {}      \
        void run();                                             \
    }