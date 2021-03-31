/**
 * Copyright (c) 2020, Timothy Stack
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of Timothy Stack nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef lnav_time_util_hh
#define lnav_time_util_hh

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <inttypes.h>
#include <string.h>

namespace lnav {

using time64_t = uint64_t;

}

struct tm *secs2tm(lnav::time64_t tim, struct tm *res);
/**
 * Convert the time stored in a 'tm' struct into epoch time.
 *
 * @param t The 'tm' structure to convert to epoch time.
 * @return The given time in seconds since the epoch.
 */
time_t tm2sec(const struct tm *t);
void secs2wday(const struct timeval &tv, struct tm *res);

inline
time_t convert_log_time_to_local(time_t value) {
    struct tm tm;

    localtime_r(&value, &tm);
#ifdef HAVE_STRUCT_TM_TM_ZONE
    tm.tm_zone = NULL;
#endif
    tm.tm_isdst = 0;
    return tm2sec(&tm);
}

constexpr lnav::time64_t MAX_TIME_T = 4000000000LL;

enum exttm_bits_t {
    ETB_YEAR_SET,
    ETB_MONTH_SET,
    ETB_DAY_SET,
    ETB_MACHINE_ORIENTED,
    ETB_EPOCH_TIME,
};

enum exttm_flags_t {
    ETF_YEAR_SET = (1UL << ETB_YEAR_SET),
    ETF_MONTH_SET = (1UL << ETB_MONTH_SET),
    ETF_DAY_SET = (1UL << ETB_DAY_SET),
    ETF_MACHINE_ORIENTED = (1UL << ETB_MACHINE_ORIENTED),
    ETF_EPOCH_TIME = (1UL << ETB_EPOCH_TIME),
};

struct exttm {
    struct tm et_tm;
    int32_t et_nsec;
    unsigned int et_flags;
    long et_gmtoff;

    bool operator==(const exttm &other) const {
        return memcmp(this, &other, sizeof(exttm)) == 0;
    };

    struct timeval to_timeval() const;
};

inline
bool operator<(const struct timeval &left, time_t right) {
    return left.tv_sec < right;
}

inline
bool operator<(time_t left, const struct timeval &right) {
    return left < right.tv_sec;
}

inline
bool operator<(const struct timeval &left, const struct timeval &right) {
    return left.tv_sec < right.tv_sec ||
           ((left.tv_sec == right.tv_sec) && (left.tv_usec < right.tv_usec));
}

inline
bool operator!=(const struct timeval &left, const struct timeval &right) {
    return left.tv_sec != right.tv_sec ||
           left.tv_usec != right.tv_usec;
}

typedef int64_t mstime_t;

inline mstime_t getmstime() {
    struct timeval tv;

    gettimeofday(&tv, nullptr);

    return tv.tv_sec * 1000ULL + tv.tv_usec / 1000ULL;
}

inline time_t day_num(time_t ti)
{
    return ti / (24 * 60 * 60);
}

inline time_t hour_num(time_t ti)
{
    return ti / (60 * 60);
}

#endif
