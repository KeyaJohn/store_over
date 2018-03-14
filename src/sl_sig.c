#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "sl_sig.h"

//flag: 0-屏蔽信号 1-解除屏蔽
int block_allsig(int flag)
{
    sigset_t    signal_set;

    if (sigemptyset(&signal_set) < 0) {
        return -1;
    }
    if (sigfillset(&signal_set) < 0) {
        return -1;
    }
    if (pthread_sigmask(flag, &signal_set, NULL) != 0) {
        return -1;
    }

    return 0;
}
