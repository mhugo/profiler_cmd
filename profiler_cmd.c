// gcc -c profiler_cmd.c -lprofiler -fPIC -DPIC
// ld -shared profiler_cmd.o -o profiler_cmd.so -lprofiler
#include <signal.h>
#include <stdio.h>

#include <gperftools/profiler.h>

struct sigaction sigaction_;
int started_ = 0;

void sig_handler( int sig )
{
    if (sig == SIGUSR1) {
        started_ = 1 - started_;
        if ( started_ ) {
            ProfilerStart( "/tmp/profile.prof" );
        }
        else {
            ProfilerStop();
        }
    }
    else if (sig == SIGUSR2) {
        ProfilerFlush();
        printf("flushed\n");
    }
}

void _init()
{
    printf("CPU profiler helper, send SIGUSR to start/stop profiling, SIGUSR2 to flush\n");

    sigaction_.sa_handler = sig_handler;
    sigemptyset(&sigaction_.sa_mask);
    sigaction_.sa_flags = 0;
    sigaction( SIGUSR1, &sigaction_, NULL );
    sigaction( SIGUSR2, &sigaction_, NULL );

    // don't start before ordered to
    //    ProfilerStop();
}

