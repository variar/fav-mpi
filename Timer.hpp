#include <sys/time.h>


class Timer
{
public:
    void Start( )
    {
        gettimeofday( &_start_time, NULL);
    }
    void Stop( )
    {
        gettimeofday( &_end_time, NULL);
    }
    double GetTotalTime( )
    {
        return _end_time.tv_sec - _start_time.tv_sec + 0.000001 * ( _end_time.tv_usec - _start_time.tv_usec);
    }
    double GetElapsedTime( )
    {
        struct timeval curr_time;
        gettimeofday( &curr_time, NULL);
        return curr_time.tv_sec - _start_time.tv_sec + 0.000001 * ( curr_time.tv_usec - _start_time.tv_usec);
    }
    Timer( )
    {
        Start( );
    }
private:
    struct timeval _start_time,
                   _end_time;

};

