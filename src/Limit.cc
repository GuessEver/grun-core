//
// Created by guessever on 4/30/17.
//

#include "Limit.h"
#include <stdio.h>
#include <sys/resource.h>
#include <zconf.h>
#include "Grun.h"

/**
 * initialize data
 * @param t
 * @param m
 * @param o
 */
Limit::Limit(unsigned t, unsigned m, unsigned o) {
    this->time_limit = t;
    this->memory_limit = m;
    this->output_limit = o;
}

#define SET_LIMIT(LIMIT, R) \
    if (setrlimit(LIMIT, R)) { \
        LOG("set " #LIMIT " error"); \
        return SUCCESS; \
    }
/**
 * setrlimit
 * @return SUCCESS | ERROR
 */
int Limit::set() {
    LOG("[limit] time = %d MS", this->time_limit);
    LOG("[limit] memory = %d KB", this->memory_limit);
    LOG("[limit] output = %d KB", this->output_limit);

    rlimit r;

    // set time limit
    r.rlim_cur = (this->time_limit + 1500) / 1000;
    r.rlim_max = r.rlim_cur + 1;
    SET_LIMIT(RLIMIT_CPU, &r);
    alarm(0);
    alarm(r.rlim_max + 1);

    // set memory limit
    // !!! REMEMBER TO CONVERT `KB` TO `B` HERE !!!
    r.rlim_cur = (this->memory_limit + 1024) * 1024;
    r.rlim_max = r.rlim_cur + 1024 * 1024;
    SET_LIMIT(RLIMIT_DATA, &r);
    r.rlim_cur *= 2;
    r.rlim_max = r.rlim_cur + 1024 * 1024;
    SET_LIMIT(RLIMIT_AS, &r);

    // set stack limit
    // !!! REMEMBER TO CONVERT `KB` TO `B` HERE !!!
    r.rlim_cur = (this->memory_limit + 1024) * 1024;
    r.rlim_max = r.rlim_cur + 1024 * 1024;
    SET_LIMIT(RLIMIT_STACK, &r);

    // set output limit
    // !!! REMEMBER TO CONVERT `KB` TO `B` HERE !!!
    r.rlim_cur = (this->output_limit + 1024) * 1024;
    r.rlim_max = r.rlim_cur + 1024 * 1024;
    SET_LIMIT(RLIMIT_FSIZE, &r);

    return SUCCESS;
}

