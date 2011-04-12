/** 
 iXKeyLog - Copyright (c) 2011 by Victor Dorneanu
 All rights reserved. 
 
 args.c - Check arguments

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
 IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, IysNDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "ixkeylog.h"
#include "display.h"
#include "args.h"


/**
 * Print help message 
 */
static void args_banner () {
    printf("iXKeyLog - *NIX X11 KeyLogger (c) by %s\n\n", D_IXKEYLOG_AUTHOR);
}


static void args_help() {
    args_banner();
    printf("Usage: ixkeylog [--daemon] [--debug] [-h] [-o output]");
    printf(" [--version] [-x DISPLAY]\n\n");
    
    printf("   --daemon     - Run process as daemon\n");
    printf("   --debug      - Run with debug messages\n");
    printf("   -h           - Print help message\n");
    printf("   -o <output>  - Specify where to dump data (default: stdout)\n");
    printf("   --version    - Print version\n");
    printf("   -x <display> - Specify which DISPLAY should be used ");
    printf("(default: getenv(\"DISPLAY\"))\n");
}


/**
 * Check arguments.
 * 
 * @param argc No. of parameters
 * @param argv The parameters
 * @return own options structure (ixkeylog_opts)
 */
ixkeylog_opts *args_check(int argc, char **argv)
{
    ixkeylog_opts *opts;
    static char *opts_name = NULL;
    int option_index = 0;
    int c;
    
    /* Allocate memory for own opts structure */
    if((opts = malloc(sizeof(struct _opts))) == NULL)
        M_DISPLAY_ERROR;
    
    /* Parse parameters */
    while((c = getopt_long(
            argc, argv, 
            short_options, long_options, 
            &option_index
         )) != 1) 
    {
        /* Detect the end of the options. */
        if (c == -1)
            break;
            
        switch (c) {
            case 0:
                opts_name = (char *)long_options[option_index].name;
                
                if(args_debug_flag)                 /* --debug */
                    opts->debug = 1;
                else if(args_daemonize_flag)        /* --daemon */
                    opts->daemon = 1;
                else if(args_version_flag) {        /* --version */
                    opts->version = 1;
                    args_banner();
                    printf("Version: %s\n", D_IXKEYLOG_VERSION);
                    exit(EXIT_SUCCESS);
                }
                    
                break;
            
            case 'h':
                args_help();
                exit(EXIT_SUCCESS);
                
            case 'o':
                opts->output = optarg;
                break;
                
            case 'x':
                opts->display = optarg;
                
            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                break;
        }
    }
    
    return opts;
}
