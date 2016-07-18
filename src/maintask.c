#include "GUI.h"
#include <math.h>
#include <stdlib.h>
#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_ts.h"
#include "spectrogram.h"
#include "set_frequencyDLG.h"

short points[400];

long frameCounter = 0;
short fps = 0;
short up = 0;

void BSP_Pointer_Update(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    fps = frameCounter;
    frameCounter = 0;
    ++up;
}

void MainTask(void) {

    BSP_TS_Init(480, 272);

    spectrogram_t *s = spectrogram_default();

    GUI_HWIN freq_dialog = Createset_frequency();
    WM_ShowWindow( freq_dialog );
    WM_SetFocus( freq_dialog );

    while(1) {

        spectrogram_fake_data(s);

        GUI_MULTIBUF_Begin();
        GUI_Clear();

        GUI_SetFont(&GUI_Font8_ASCII);
        char buf[32];
        snprintf(buf, 32, "%dF/%dU", fps, up);
        GUI_SetTextAlign( GUI_TA_RIGHT | GUI_TA_BOTTOM);
        GUI_DispStringAt( buf, LCD_GetXSize(), LCD_GetYSize());

        spectrogram_draw(s);

        GUI_SetFont(&GUI_Font8x15B_ASCII);
        GUI_SetTextAlign( GUI_TA_LEFT | GUI_TA_BOTTOM);
        GUI_DispStringAt( "START: 10.00 MHz", 10, LCD_GetYSize()-16);
        GUI_DispStringAt( "STOP:  2.000 GHz", 10, LCD_GetYSize());
        GUI_DispStringAt( "RBW: 300.0 KHz", 158, LCD_GetYSize()-16);
        GUI_DispStringAt( "ST:  10.00 mS", 158, LCD_GetYSize());
        GUI_DispStringAt( "MIX. MODE: 1+", 290, LCD_GetYSize()-16);
        GUI_DispStringAt( "0.0 - 2.0 GHz", 290, LCD_GetYSize());

        BSP_Pointer_Update();
        GUI_Exec();

        GUI_MULTIBUF_End();
        ++frameCounter;
    }
}

void BSP_Pointer_Update(void) {
    GUI_PID_STATE TS_State;
    static TS_StateTypeDef prev_state;
    TS_StateTypeDef  ts;
    uint16_t xDiff, yDiff;
    BSP_TS_GetState(&ts);
    TS_State.Pressed = ts.touchDetected;

    xDiff = (prev_state.touchX[0] > ts.touchX[0]) ?
            (prev_state.touchX[0] - ts.touchX[0]) :
            (ts.touchX[0] - prev_state.touchX[0]);

    yDiff = (prev_state.touchY[0] > ts.touchY[0]) ?
            (prev_state.touchY[0] - ts.touchY[0]) :
            (ts.touchY[0] - prev_state.touchY[0]);

    if((prev_state.touchDetected != ts.touchDetected )||
            (xDiff > 3 )||
            (yDiff > 3))
    {
        prev_state.touchDetected = ts.touchDetected;

        if((ts.touchX[0] != 0) &&  (ts.touchY[0] != 0)) 
        {
            prev_state.touchX[0] = ts.touchX[0];
            prev_state.touchY[0] = ts.touchY[0];
        }

        TS_State.Layer = 0;
        TS_State.x = prev_state.touchX[0];
        TS_State.y = prev_state.touchY[0];

        GUI_TOUCH_StoreStateEx(&TS_State);
    }
}
