/** 
 iXKeyLog - Copyright (c) 2011 by Victor Dorneanu
 All rights reserved. 
 
 main.c 

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

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "signal.h"
#include "display.h"
#include "ixkeylog.h"
#include "args.h"


/* Global stuff */
ixkeylog_opts *opts;
X11LogSess *sess;


/**
 * Main function: Get arguments, do syntax check and then start logging.
 */
int main(int argc, char** argv)
{
    int pid;
    
    // Get arguments 
    if(!(opts = args_check(argc, argv))) {
        fprintf(stderr, "Couldn't parse arguments.\n");
        return -1;
    }
    
    /* Set global structure pointer */
    g_ixkeylog_opts = opts; 
    
    /* Initialize display */
    sess = display_init();
    
    /* Focus on certain window and start logging */
    if (sess == NULL) {
        fprintf(stderr, "Something went wrong. Aborting.\n");
        exit(EXIT_FAILURE);
    }
    
    /* Daemonize process ? */
    if(g_ixkeylog_opts->daemon > 0) {
        if((pid = fork()) < 0)
            M_DISPLAY_ERROR;
            
        /* Exit the parent process */
        if(pid > 0) 
            exit(EXIT_SUCCESS);
    }
    
    // TODO: Implement this properly 
    // Catch signals for proper exit
    // (void) signal(SIGINT, signal_handler);
    
    // Start logging
    while (1) {
        if (sess->focuswin == None)
            display_focus(sess);
        else
            display_handle_event(sess);
    }
     
    return EXIT_SUCCESS;
}
