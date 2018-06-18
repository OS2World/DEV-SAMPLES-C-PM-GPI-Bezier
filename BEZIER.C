

/*-------------------------------------------------------------------
   BEZIER.C -- GPI Bezier Spline Demonstration Program
               (c) 1989, Charles Petzold and Ziff Communications Co.
               PC Magazine, Volume 8, Number 19
  -------------------------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

MRESULT EXPENTRY ClientWndProc (HWND, USHORT, MPARAM, MPARAM) ;

int main (void)
     {
     static CHAR  szClientClass[] = "Bezier" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU  |
                                 FCF_SIZEBORDER    | FCF_MINMAX   |
                                 FCF_SHELLPOSITION | FCF_TASKLIST ;
     HAB          hab ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, szClientClass, ClientWndProc, CS_SIZEREDRAW, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass, NULL,
                                     0L, NULL, 0, &hwndClient) ;

     while (WinGetMsg (hab, &qmsg, NULL, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

VOID DrawSpline (HPS hps, POINTL *pptlEnd1, POINTL *pptlEnd2,
                          POINTL *pptlCtl1, POINTL *pptlCtl2)
     {
     POINTL aptl[4] ;

               // Set 4 point structures

     aptl[0] = *pptlEnd1 ;
     aptl[1] = *pptlCtl1 ;
     aptl[2] = *pptlCtl2 ;
     aptl[3] = *pptlEnd2 ;

               // Draw dotted straight lines

     GpiSetLineType (hps, LINETYPE_ALTERNATE) ;
     GpiMove (hps, aptl + 0) ;
     GpiLine (hps, aptl + 1) ;
     GpiMove (hps, aptl + 3) ;
     GpiLine (hps, aptl + 2) ;

               // Draw spline

     GpiSetLineType (hps, LINETYPE_SOLID) ;
     GpiMove (hps, aptl) ;
     GpiPolySpline (hps, 3L, aptl + 1) ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
     {
     static BOOL   fButton1Down, fButton2Down ;
     static POINTL ptlEnd1, ptlEnd2, ptlCtl1, ptlCtl2 ;
     HPS           hps ;
     SHORT         cxClient, cyClient ;

     switch (msg)
          {
          case WM_SIZE:
               cxClient = SHORT1FROMMP (mp2) ;
               cyClient = SHORT2FROMMP (mp2) ;

               ptlEnd1.x = cxClient / 3 ;
               ptlEnd1.y = cyClient / 2 ;

               ptlEnd2.x = 2 * cxClient / 3 ;
               ptlEnd2.y = cyClient / 2 ;

               ptlCtl1.x = cxClient / 2 ;
               ptlCtl1.y = 3 * cyClient / 4 ;

               ptlCtl2.x = cxClient / 2 ;
               ptlCtl2.y = cyClient / 4 ;
               return 0 ;

          case WM_BUTTON1DOWN:
               if (!fButton2Down)
                    {
                    WinSetCapture (HWND_DESKTOP, hwnd) ;
                    fButton1Down = TRUE ;
                    }
               else
                    WinAlarm (HWND_DESKTOP, WA_ERROR) ;

               break ;

          case WM_BUTTON2DOWN:
               if (!fButton1Down)
                    {
                    WinSetCapture (HWND_DESKTOP, hwnd) ;
                    fButton2Down = TRUE ;
                    }
               else
                    WinAlarm (HWND_DESKTOP, WA_ERROR) ;

               break ;

          case WM_BUTTON1UP:
               if (fButton1Down)
                    {
                    WinSetCapture (HWND_DESKTOP, NULL) ;
                    fButton1Down = FALSE ;
                    }
               break ;

          case WM_BUTTON2UP:
               if (fButton2Down)
                    {
                    WinSetCapture (HWND_DESKTOP, NULL) ;
                    fButton2Down = FALSE ;
                    }
               break ;

          case WM_MOUSEMOVE:
               if (!fButton1Down && !fButton2Down)
                    break ;

               hps = WinGetPS (hwnd) ;
               GpiSetColor (hps, CLR_BACKGROUND) ;
               DrawSpline (hps, &ptlEnd1, &ptlEnd2, &ptlCtl1, &ptlCtl2) ;

               if (fButton1Down)
                    {
                    ptlCtl1.x = MOUSEMSG(&msg)->x ;
                    ptlCtl1.y = MOUSEMSG(&msg)->y ;
                    }
               else
                    {
                    ptlCtl2.x = MOUSEMSG(&msg)->x ;
                    ptlCtl2.y = MOUSEMSG(&msg)->y ;
                    }

               GpiSetColor (hps, CLR_NEUTRAL) ;
               DrawSpline (hps, &ptlEnd1, &ptlEnd2, &ptlCtl1, &ptlCtl2) ;
               WinReleasePS (hps) ;
               break ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULL, NULL) ;
               GpiErase (hps) ;

               DrawSpline (hps, &ptlEnd1, &ptlEnd2, &ptlCtl1, &ptlCtl2) ;

               WinEndPaint (hps) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }

