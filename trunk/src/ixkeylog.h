/** 
 iXKeyLog - Copyright (c) 2011 by Victor Dorneanu
 All rights reserved. 
 
 ixkeylog.h

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

#ifndef __IXKEYLOG_H__
#define __IXKEYLOG_H__

#include "display.h"

#define D_IXKEYLOG_VERSION    "0.1"
#define D_IXKEYLOG_AUTHOR     "Victor Dorneanu"


/**
 * Useful macros
 */
#define M_DISPLAY_ERROR fprintf(stderr, ">> Error: %s\n", strerror(errno))
#define M_DEBUG_INFO(format, args...) \
        fprintf(stdout, "[DEBUG] (" __FILE__ ") " format, args);
        
#define M_ERROR(e)      { M_DISPLAY_ERROR; exit(e); }


#endif /* ixkeylog.h */
