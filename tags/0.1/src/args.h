/** 
 iXKeyLog - Copyright (c) 2011 by Victor Dorneanu
 All rights reserved. 
 
 args.h - Check arguments

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. The name of the author may not be used to endorse or promote products
    derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __ARGS_H__
#define __ARGS_H__

#include <unistd.h>
#include <getopt.h>


/* Useful variables */
static int args_debug_flag             = 0;
static int args_daemonize_flag         = 0;
static int args_version_flag           = 0;

/* Short options */
static const char *short_options = "ho:x:";


/**
 * Long options for getopts
 */
static const struct option long_options[] =
{
    /* These arguments set flags */
    {"debug",       0,    &args_debug_flag,     1},
    {"daemon",      0,    &args_daemonize_flag, 1},
    {"version",     0,    &args_version_flag,   1},
    
    /* These ones require arguments */
    {"output",  1, 0, 'o'},
    {"display", 1, 0, 'x'},
    {0, 0, 0, 0}
};

/** 
 * Own options structures
 */
typedef struct _opts {
    const char *display;        /** Where to log */
    const char *output;         /** Where to dump logged data */
    int daemon;                 /** Daemonize process? */
    int debug;                  /** Show debug information */
    int version;
} ixkeylog_opts;


/* Function declarations */
ixkeylog_opts *args_check(int, char **);


/* Global stuff */
ixkeylog_opts *g_ixkeylog_opts;


#endif /* args.h */
