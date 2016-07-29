/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.28                          *
*        Compiled Jan 30 2015, 16:41:06                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "menuDLG.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "FRAMEWIN.h"
#include "options.h"
#include "string.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x02)
#define ID_BUTTON_0    (GUI_ID_USER + 0x04)
#define ID_BUTTON_1    (GUI_ID_USER + 0x05)
#define ID_BUTTON_2    (GUI_ID_USER + 0x06)
#define ID_BUTTON_3    (GUI_ID_USER + 0x07)
#define ID_BUTTON_4    (GUI_ID_USER + 0x08)
#define ID_BUTTON_5    (GUI_ID_USER + 0x09)
#define ID_BUTTON_CATEGORY    (GUI_ID_USER + 0x0A)

#define N_MENU_BUTTONS 6


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "menu", ID_FRAMEWIN_0, 390, -21, 110, 300, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TRACE", ID_BUTTON_CATEGORY, 0, 5, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Detector:\nMAX HOLD", ID_BUTTON_0, 0, 40, 80, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2", ID_BUTTON_1, 0, 80, 80, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3", ID_BUTTON_2, 0, 120, 80, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4", ID_BUTTON_3, 0, 160, 80, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5", ID_BUTTON_4, 0, 200, 80, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6", ID_BUTTON_5, 0, 240, 80, 35, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

void option_press( option_t *o ) {
    if( o->type == OPTION_TYPE_SELECTION ) {
        selection_option_t *so = o->data;
        for( int i = 0; i != so->nSelections; ++i ) {
            if(so->selections[i].id == so->value) {
                so->value = so->selections[(i+1)%so->nSelections].id;
                return;
            }
        }
    } else if( o->type == OPTION_TYPE_FREQUENCY ) {
        GUI_MessageBox("NOT YET EY!", "frequency alter", 0);
    } else {
        // Nothing, unknown option type
    }
}

void print_scientific( double f, char *units, char *dest ) {
    char *postfix = "";
    if ( 1e3 < f && f < 1e6 ) {
        f /= 1e3;
        postfix = "k";
    } else if (1e6 <= f && f < 1e9 ) {
        f /= 1e6;
        postfix = "M";
    } else if( 1e9 <= f && f < 1e12 ) {
        f /= 1e9;
        postfix = "G";
    } else {
        // Nothing
    }
    long long section_1 = (long long)f;
    long long section_2 = (long long)fabs(f*1000000);
    char section_2_str[32] = "";
    if( section_2 >= 1 ) {
        snprintf(section_2_str, 32, ".%lld", section_2);
    }
    snprintf(dest, 64, "%lld%s %s%s", section_1, section_2_str, postfix, units);
}

void option_to_string( option_t *o, char *buf ) {
    char value[64] = "";
    if( o->type == OPTION_TYPE_SELECTION ) {
        selection_option_t *so = o->data;
        for( int i = 0; i != so->nSelections; ++i ) {
            if(so->selections[i].id == so->value) {
                strcpy(value, so->selections[i].name);
            }
        }
    } else if( o->type == OPTION_TYPE_FREQUENCY ) {
        double f = *(double*)o->data;
        print_scientific(f, "Hz", value);
    } else {
        // Nothing, unknown option type
    }
    snprintf(buf, 64, "%s:\n%s", o->name, value);
}

void refreshMenu(WM_HWIN hCategoryButton, WM_HWIN *hButtons, option_t **button_options, int currentCategory) {
    asm volatile ("cpsid i" : : : "memory");

    options_refresh();

    category_t *this_category = &get_option_menu()->categories[currentCategory];

    BUTTON_SetText( hCategoryButton, this_category->name );

    // Clear all buttons first
    for(int i = 0; i != N_MENU_BUTTONS; ++i) {
        BUTTON_SetText(hButtons[i], "");
        button_options[i] = 0;
    }

    int currentButton = 0;
    char buf[64];
    for(int i = 0; i != this_category->nOptions; ++i) {
        option_t *this_option = &this_category->options[i];
        if(this_option->enable) {
            option_to_string(this_option, buf);
            BUTTON_SetText(hButtons[currentButton], buf);
            button_options[currentButton] = this_option;
            ++currentButton;
        }
    }

    asm volatile ("cpsie i" : : : "memory");
}

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int     NCode;
    int     Id;

    // USER START (Optionally insert additional variables)
    // USER END

    static WM_HWIN hCategoryButton;
    static WM_HWIN hButtons[6];
    static option_t *button_options[6];
    static int currentCategory = 0;

    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG:
            hItem = WM_GetDialogItem(pMsg->hWin, ID_FRAMEWIN_0);


            hCategoryButton = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CATEGORY);
            BUTTON_SetFont(hCategoryButton, GUI_FONT_20B_ASCII);

            hButtons[0] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            hButtons[1] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
            hButtons[2] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
            hButtons[3] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
            hButtons[4] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
            hButtons[5] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);

            for( int i = 0; i !=  N_MENU_BUTTONS; ++i ) {
                BUTTON_SetFont(hButtons[i], GUI_FONT_13B_ASCII);
            }

            refreshMenu(hCategoryButton, hButtons, button_options, currentCategory);

            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            printf("WM_NOTIFY: ID:%i, NCODE:%i\n", Id, NCode);
            switch(Id) {
                case ID_BUTTON_CATEGORY: // Notifications sent by '1'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        ++currentCategory;
                        currentCategory %= get_option_menu()->nCategories;
                    }
                    break;
                case ID_BUTTON_0: // Notifications sent by '1'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( button_options[0] ) {
                            option_press( button_options[0] );
                        }
                    }
                    break;
                case ID_BUTTON_1: // Notifications sent by '2'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( button_options[1] ) {
                            option_press( button_options[1] );
                        }
                    }
                    break;
                case ID_BUTTON_2: // Notifications sent by '3'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( button_options[2] ) {
                            option_press( button_options[2] );
                        }
                    }
                    break;
                case ID_BUTTON_3: // Notifications sent by '4'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( button_options[3] ) {
                            option_press( button_options[3] );
                        }
                    }
                    break;
                case ID_BUTTON_4: // Notifications sent by '5'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( button_options[4] ) {
                            option_press( button_options[4] );
                        }
                    }
                    break;
                case ID_BUTTON_5: // Notifications sent by '6'
                    if( NCode == WM_NOTIFICATION_RELEASED ) {
                        if( button_options[5] ) {
                            option_press( button_options[5] );
                        }
                    }
                    break;
                    // USER START (Optionally insert additional code for further Ids)
                    // USER END
            }
            refreshMenu(hCategoryButton, hButtons, button_options, currentCategory);
            break;
            // USER START (Optionally insert additional message handling)
            // USER END
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       Createset_frequency
*/
WM_HWIN Createmenu(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
