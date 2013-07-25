/*
 *Copyright (C) 2003-2004 Harold L Hunt II All Rights Reserved.
 *Copyright (C) Colin Harrison 2005-2008
 *
 *Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 *"Software"), to deal in the Software without restriction, including
 *without limitation the rights to use, copy, modify, merge, publish,
 *distribute, sublicense, and/or sell copies of the Software, and to
 *permit persons to whom the Software is furnished to do so, subject to
 *the following conditions:
 *
 *The above copyright notice and this permission notice shall be
 *included in all copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *NONINFRINGEMENT. IN NO EVENT SHALL HAROLD L HUNT II BE LIABLE FOR
 *ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 *CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *Except as contained in this notice, the name of the copyright holder(s)
 *and author(s) shall not be used in advertising or otherwise to promote
 *the sale, use or other dealings in this Software without prior written
 *authorization from the copyright holder(s) and author(s).
 *
 * Authors:	Harold L Hunt II
 *              Colin Harrison
 */

#ifdef HAVE_XWIN_CONFIG_H
#include <xwin-config.h>
#else
#define HAS_WINSOCK 1
#endif
#include <sys/types.h>
#include <signal.h>
#include "winclipboard.h"
#ifdef __CYGWIN__
#include <errno.h>
#endif
#include "misc.h"
#include "winmsg.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif
/*
 * References to external symbols
 */

extern Bool g_fUnicodeClipboard;
extern Bool g_fClipboardStarted;
extern Bool g_fClipboardLaunched;
extern HWND g_hwndClipboard;
extern void *g_pClipboardDisplay;
extern Window g_iClipboardWindow;
extern Bool g_fClipboardPrimary;

/*
 * Global variables
 */

static jmp_buf g_jmpEntry;
static XIOErrorHandler g_winClipboardOldIOErrorHandler;
static pthread_t g_winClipboardProcThread;

Bool g_fUseUnicode = FALSE;

/*
 * Local function prototypes
 */

static int
 winClipboardErrorHandler(Display * pDisplay, XErrorEvent * pErr);

static int
 winClipboardIOErrorHandler(Display * pDisplay);

static void
winClipboardThreadExit(void *arg);
/*
 * Main thread function
 */

void *
winClipboardProc(void *pvNotUsed)
{
    Atom atomClipboard;
    int iReturn;
    HWND hwnd = NULL;
    int iConnectionNumber = 0;

#ifdef HAS_DEVWINDOWS
    int fdMessageQueue = 0;
#else
    struct timeval tvTimeout;
#endif
    fd_set fdsRead;
    int iMaxDescriptor;
    Display *pDisplay = NULL;
    Window iWindow = None;
    int iRetries;
    Bool fUseUnicode;
    char szDisplay[512];
    int iSelectError;

    pthread_cleanup_push(&winClipboardThreadExit, NULL);

    winDebug ("winClipboardProc - Hello\n");

    /* Do we use Unicode clipboard? */
    fUseUnicode = g_fUnicodeClipboard;

    /* Save the Unicode support flag in a global */
    g_fUseUnicode = fUseUnicode;

    /* Create Windows messaging window */
    hwnd = winClipboardCreateMessagingWindow ();
  
    /* Save copy of HWND in screen privates */
    g_hwndClipboard = hwnd;

    /* Set error handler */
    XSetErrorHandler(winClipboardErrorHandler);
    g_winClipboardProcThread = pthread_self();
    g_winClipboardOldIOErrorHandler =
        XSetIOErrorHandler(winClipboardIOErrorHandler);

    /* Set jump point for Error exits */
    iReturn = setjmp(g_jmpEntry);

    /* Check if we should continue operations */
    if (iReturn != WIN_JMP_ERROR_IO && iReturn != WIN_JMP_OKAY) {
        /* setjmp returned an unknown value, exit */
        ErrorF("winClipboardProc - setjmp returned: %d exiting\n", iReturn);
        goto thread_errorexit;
    }
    else if (iReturn == WIN_JMP_ERROR_IO) {
        /* TODO: Cleanup the Win32 window and free any allocated memory */
        ErrorF("winClipboardProc - setjmp returned for IO Error Handler.\n");
    }

    /* Use our generated cookie for authentication */
    winSetAuthorization();

    /* Initialize retry count */
    iRetries = 0;

    /* Setup the display connection string x */
    /*
     * NOTE: Always connect to screen 0 since we require that screen
     * numbers start at 0 and increase without gaps.  We only need
     * to connect to one screen on the display to get events
     * for all screens on the display.  That is why there is only
     * one clipboard client thread.
     */
    winGetDisplayName(szDisplay,0);

    /* Print the display connection string */
    winDebug ("winClipboardProc - DISPLAY=%s\n", szDisplay);

    /* Open the X display */
    do {
        pDisplay = XOpenDisplay(szDisplay);
        if (pDisplay == NULL) {
            ErrorF("winClipboardProc - Could not open display, "
                   "try: %d, sleeping: %d\n", iRetries + 1, WIN_CONNECT_DELAY);
            ++iRetries;
            sleep(WIN_CONNECT_DELAY);
            continue;
        }
        else
            break;
    }
    while (pDisplay == NULL && iRetries < WIN_CONNECT_RETRIES);

    /* Make sure that the display opened */
    if (pDisplay == NULL) {
        ErrorF("winClipboardProc - Failed opening the display, giving up\n");
        goto thread_errorexit;
    }

    /* Save the display in the screen privates */
    g_pClipboardDisplay = pDisplay;

    winDebug ("winClipboardProc - XOpenDisplay () returned and "
           "successfully opened the display.\n");

    /* Get our connection number */
    iConnectionNumber = ConnectionNumber(pDisplay);

    winDebug("Clipboard is using socket %d\n",iConnectionNumber);

#ifdef HAS_DEVWINDOWS
    /* Open a file descriptor for the windows message queue */
    fdMessageQueue = open (WIN_MSG_QUEUE_FNAME, _O_RDONLY);
    if (fdMessageQueue == -1) {
        ErrorF("winClipboardProc - Failed opening %s\n", WIN_MSG_QUEUE_FNAME);
        goto thread_errorexit;
    }

    /* Find max of our file descriptors */
    iMaxDescriptor = max(fdMessageQueue, iConnectionNumber) + 1;
#else
    iMaxDescriptor = iConnectionNumber + 1;
#endif

    /* Create atom */
    atomClipboard = XInternAtom(pDisplay, "CLIPBOARD", False);
    XInternAtom (pDisplay, WIN_LOCAL_PROPERTY, False);
    XInternAtom (pDisplay, "UTF8_STRING", False);
    XInternAtom (pDisplay, "COMPOUND_TEXT", False);
    XInternAtom (pDisplay, "TARGETS", False);

    /* Create a messaging window */
    iWindow = XCreateSimpleWindow(pDisplay,
                                  DefaultRootWindow(pDisplay),
                                  1, 1,
                                  500, 500,
                                  0,
                                  BlackPixel(pDisplay, 0),
                                  BlackPixel(pDisplay, 0));
    if (iWindow == 0) {
        ErrorF("winClipboardProc - Could not create an X window.\n");
        goto thread_errorexit;
    }

    XStoreName(pDisplay, iWindow, "xwinclip");

    /* Select event types to watch */
    if (XSelectInput(pDisplay, iWindow, PropertyChangeMask) == BadWindow)
        ErrorF("winClipboardProc - XSelectInput generated BadWindow "
               "on messaging window\n");

    /* Save the window in the screen privates */
    g_iClipboardWindow = iWindow;

    /* Assert ownership of selections if Win32 clipboard is owned */
    if (NULL != GetClipboardOwner()) {
        if (g_fClipboardPrimary)
        {
            /* PRIMARY */
            winDebug("winClipboardProc - asserted ownership.\n");
            iReturn = XSetSelectionOwner (pDisplay, XA_PRIMARY,
                                          iWindow, CurrentTime);
            if (iReturn == BadAtom || iReturn == BadWindow /*||
                XGetSelectionOwner (pDisplay, XA_PRIMARY) != iWindow*/)
            {
                ErrorF ("winClipboardProc - Could not set PRIMARY owner\n");
                goto thread_errorexit;
            }
        }

        /* CLIPBOARD */
        iReturn = XSetSelectionOwner(pDisplay, atomClipboard,
                                     iWindow, CurrentTime);
        if (iReturn == BadAtom || iReturn == BadWindow /*||
            XGetSelectionOwner (pDisplay, atomClipboard) != iWindow*/)
        {
            ErrorF ("winClipboardProc - Could not set CLIPBOARD owner\n");
            goto thread_errorexit;
        }
    }

    /* Pre-flush X events */
    /* 
     * NOTE: Apparently you'll freeze if you don't do this,
     *       because there may be events in local data structures
     *       already.
     */
    //winClipboardFlushXEvents(hwnd, iWindow, pDisplay, fUseUnicode);

    /* Pre-flush Windows messages */
    winDebug ("Start flushing \n");
    if (!winClipboardFlushWindowsMessageQueue(hwnd))
    {
      ErrorF ("winClipboardFlushWindowsMessageQueue - returned 0\n");
      goto thread_errorexit;
    }

    winDebug ("winClipboardProc - Started\n");
    /* Signal that the clipboard client has started */
    g_fClipboardStarted = TRUE;

    /* Loop for X events */
    while (1) {
        /* Setup the file descriptor set */
        /*
         * NOTE: You have to do this before every call to select
         *       because select modifies the mask to indicate
         *       which descriptors are ready.
         */
        FD_ZERO(&fdsRead);
        FD_SET(iConnectionNumber, &fdsRead);
#ifdef HAS_DEVWINDOWS
        FD_SET(fdMessageQueue, &fdsRead);
#else
        tvTimeout.tv_sec = 0;
        tvTimeout.tv_usec = 100;
#endif

        /* Wait for a Windows event or an X event */
        iReturn = select(iMaxDescriptor,        /* Highest fds number */
                         &fdsRead,      /* Read mask */
                         NULL,  /* No write mask */
                         NULL,  /* No exception mask */
#ifdef HAS_DEVWINDOWS
                         NULL   /* No timeout */
#else
                         &tvTimeout     /* Set timeout */
#endif
            );

#ifndef HAS_WINSOCK
        iSelectError = errno;
#else
        iSelectError = WSAGetLastError();
#endif

        if (iReturn < 0) {
#ifndef HAS_WINSOCK
            if (iSelectError == EINTR)
#else
            if (iSelectError == WSAEINTR)
#endif
                continue;

            ErrorF("winClipboardProc - Call to select () failed: %d.  "
                   "Bailing.\n", iReturn);
            break;
        }

        /* Branch on which descriptor became active */
//        if (FD_ISSET (iConnectionNumber, &fdsRead)) {
//            Also do it when no read since winClipboardFlushXEvents
//            is sending the output.
            /* Process X events */
            winClipboardFlushXEvents(hwnd, iWindow, pDisplay, fUseUnicode, FALSE);
//        }

#ifdef HAS_DEVWINDOWS
        /* Check for Windows event ready */
        if (FD_ISSET(fdMessageQueue, &fdsRead))
#else
        if (1)
#endif
        {
            /* Process Windows messages */
            if (!winClipboardFlushWindowsMessageQueue(hwnd)) {
                ErrorF("winClipboardProc - "
                       "winClipboardFlushWindowsMessageQueue trapped "
                       "WM_QUIT message, exiting main loop.\n");
                break;
            }
        }
    }

    /* Close our X window */
    if (pDisplay && iWindow) {
        iReturn = XDestroyWindow(pDisplay, iWindow);
        if (iReturn == BadWindow)
            ErrorF("winClipboardProc - XDestroyWindow returned BadWindow.\n");
#ifdef WINDBG
        else
            winDebug("winClipboardProc - XDestroyWindow succeeded.\n");
#endif
    }

#ifdef HAS_DEVWINDOWS
    /* Close our Win32 message handle */
    if (fdMessageQueue)
        close(fdMessageQueue);
#endif

#if 0
    /*
     * FIXME: XCloseDisplay hangs if we call it, as of 2004/03/26.  The
     * XSync and XSelectInput calls did not help.
     */

    /* Discard any remaining events */
    XSync(pDisplay, TRUE);

    /* Select event types to watch */
    XSelectInput(pDisplay, DefaultRootWindow(pDisplay), None);

    /* Close our X display */
    if (pDisplay) {
        XCloseDisplay(pDisplay);
    }
#endif

  goto commonexit;

thread_errorexit:
    if (g_pClipboardDisplay && g_iClipboardWindow)
    {
        iReturn = XDestroyWindow (g_pClipboardDisplay, g_iClipboardWindow);
        if (iReturn == BadWindow)
            ErrorF ("winClipboardProc - XDestroyWindow returned BadWindow.\n");
#ifdef WINDBG
        else
            winDebug ("winClipboardProc - XDestroyWindow succeeded.\n");
#endif
    }
    winDebug ("Clipboard thread died.\n");

commonexit:
    g_iClipboardWindow = None;
    g_pClipboardDisplay = NULL;
    g_fClipboardLaunched = FALSE;
    g_fClipboardStarted = FALSE;

    pthread_cleanup_pop(0);

    return NULL;
}

/*
 * winClipboardErrorHandler - Our application specific error handler
 */

static int
winClipboardErrorHandler(Display * pDisplay, XErrorEvent * pErr)
{
    char pszErrorMsg[100];

    XGetErrorText(pDisplay, pErr->error_code, pszErrorMsg, sizeof(pszErrorMsg));
    ErrorF("winClipboardErrorHandler - ERROR: \n\t%s\n"
          "  errorCode %d\n"
          "  serial %lu\n"
          "  resourceID 0x%x\n"
          "  majorCode %d\n"
          "  minorCode %d\n"
          , pszErrorMsg
          , pErr->error_code
          , pErr->serial
          , pErr->resourceid
          , pErr->request_code
          , pErr->minor_code);
    return 0;
}

/*
 * winClipboardIOErrorHandler - Our application specific IO error handler
 */

static int
winClipboardIOErrorHandler(Display * pDisplay)
{
    ErrorF("winClipboardIOErrorHandler!\n");

    if (pthread_equal(pthread_self(), g_winClipboardProcThread)) {
        /* Restart at the main entry point */
        longjmp(g_jmpEntry, WIN_JMP_ERROR_IO);
    }

    if (g_winClipboardOldIOErrorHandler)
        g_winClipboardOldIOErrorHandler(pDisplay);

    return 0;
}

/*
 * winClipboardThreadExit - Thread exit handler
 */

static void
winClipboardThreadExit(void *arg)
{
  /* clipboard thread has exited, stop server as well */
  AbortDDX(EXIT_ERR_ABORT);
  TerminateProcess(GetCurrentProcess(),1);
}
