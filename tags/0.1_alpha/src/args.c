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
 IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <argtable2.h>
#include <string.h>
#include <stdlib.h>
#include "ixkeylog.h"
#include "display.h"


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
    
    /* 
     * Define the allowable command line options, 
     * collecting them in argtable[]
     */
    struct arg_lit *daemon = arg_lit0(
            "D","daemon","- run as daemon in the background");
            
    struct arg_lit *debug = arg_lit0(
            "d","debug","- show debug messages");
            
    struct arg_lit *help = arg_lit0(
            "h","help","- print this help and exit");
            
    struct arg_file *output = arg_file0(
            "o","output","<output>", "- where to log data (default is \"-\")");

    struct arg_lit *vers = arg_lit0(
            "V","version","- print version information and exit");
            
    struct arg_file *display = arg_file0(
            "x","display","<display>", "- display to use (default is env(\"DISPLAY\"))");
            
    struct arg_end *end  = arg_end(20);
    void* argtable[] = {
                debug, 
                daemon, 
                help, 
                vers,  
                output, 
                display,
                end};
                
    int nerrors;

    
    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
    {
        // NULL entries were detected
        fprintf(stderr, "Insufficient memory\n");
        return NULL;
    }
    
    /* Allocate memory for own opts structure */
    if((opts = malloc(sizeof(struct _opts))) == NULL) {
        fprintf(stderr, "Insufficient memory\n");
        return NULL;
    }
    
    
    /* Set default values */
    output->filename[0]="-";

    /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);
    
    /* Check parameters */
    if (help->count > 0)  {
        printf("iXKeyLog - *NIX X11 KeyLogger (c) by %s\n\n", D_IXKEYLOG_AUTHOR);
        printf("Usage: %s", argv[0]);
        arg_print_syntax(stdout,argtable,"\n");
        arg_print_glossary(stdout,argtable,"  %-10s %s\n");
        exit(EXIT_SUCCESS);
    }
    
    if (vers->count > 0) {
        printf("iXKeyLog - *NIX X11 KeyLogger (c) by %s\n", D_IXKEYLOG_AUTHOR);
        printf("Version: %s\n", D_IXKEYLOG_VERSION);
        exit(EXIT_SUCCESS);
    }
    
    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0) {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout, end, argv[0]);
        printf("Try '%s --help' for more information.\n", argv[0]);
        return NULL;
    }
    
    // Fill in opts with collected arguments
    opts->daemon    = daemon->count;
    opts->output    = output->filename[0];
    opts->display   = display->filename[0]; 
    opts->debug     = debug->count;
    
    
    /*** DEBUG ********************************************************/
    if(opts->debug > 0) {
        M_DEBUG_INFO("opts->daemon: %d\n", opts->daemon); 
        M_DEBUG_INFO("opts->output: %s\n", opts->output);
        M_DEBUG_INFO("opts->display: %s\n", opts->display);
        M_DEBUG_INFO("opts->debug: %d\n", opts->debug);
    }
    /******************************************************************/
    
    return opts;
}
