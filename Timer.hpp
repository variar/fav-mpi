#include <sys/time.h>

class Timer
{
public:
    Timer()
    {
        Start();
    }

    void Start()
    {
        gettimeofday(&_start_time, NULL);
    }
    
    void Stop()
    {
        gettimeofday(&_end_time, NULL);
    }
    
    double GetTotalTime()
    {
        return _end_time.tv_sec - _start_time.tv_sec + 0.000001 * (_end_time.tv_usec - _start_time.tv_usec);
    }
    
    double GetElapsedTime()
    {
        struct timeval curr_time;
        gettimeofday( &curr_time, NULL);
        return curr_time.tv_sec - _start_time.tv_sec + 0.000001 * (curr_time.tv_usec - _start_time.tv_usec);
    }

private:
    struct timeval _start_time;
    struct timeval _end_time;

};

