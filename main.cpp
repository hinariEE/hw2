#include "mbed.h"
#include "uLCD_4DGL.h"

DigitalIn up(D3);
DigitalIn down(D6);
DigitalIn enter(D5);
AnalogOut Aout(D7);
AnalogIn Ain(A0);
uLCD_4DGL uLCD(D1, D0, D2);

int idx = 0;
int sample = 1024;
float ADCdata[1024];

void cursor_update(int pos, int select)
{
    if(select == 0)
        uLCD.color(RED);
    else
        uLCD.color(GREEN);
    
    uLCD.locate(0, 0);
    uLCD.printf(" ");
    uLCD.locate(0, 1);
    uLCD.printf(" ");
    uLCD.locate(0, 2);
    uLCD.printf(" ");
    uLCD.locate(0, 3);
    uLCD.printf(" ");
    uLCD.locate(0, pos);
    uLCD.printf("v");
}

void sample_output()
{
    for(int i = 0; i < sample; i++){
        printf("%f\n", ADCdata[i]);
    }
}

int main()
{
    up.mode(PullNone);
    down.mode(PullNone);
    enter.mode(PullNone);
    
    uLCD.color(GREEN);
    uLCD.reset();
    uLCD.locate(2, 0);
    uLCD.printf(" 50 Hz");
    uLCD.locate(2, 1);
    uLCD.printf("100 Hz");
    uLCD.locate(2, 2);
    uLCD.printf("200 Hz");
    uLCD.locate(2, 3);
    uLCD.printf("300 Hz");
    
    int cursor = 0;
    uLCD.locate(0, 0);
    uLCD.printf("v");
    
    int btn_prev = 0;
    int sample_on = 0;
    float i = 0.0f;
    float step;
    float rise_list[] = { 10.0f / 9.0f * 0.0001f *  50.0f * 0.285f,  // 50 Hz
                          10.0f / 9.0f * 0.0001f * 100.0f * 0.285f,  // 100 Hz
                          10.0f / 9.0f * 0.0001f * 200.0f * 0.29f,   // 200 Hz
                          10.0f / 9.0f * 0.0001f * 300.0f * 0.29f};  // 300 Hz
    float fall_list[] = {-10.0f / 1.0f * 0.0001f *  50.0f * 0.285f,
                         -10.0f / 1.0f * 0.0001f * 100.0f * 0.285f,
                         -10.0f / 1.0f * 0.0001f * 200.0f * 0.29f,
                         -10.0f / 1.0f * 0.0001f * 300.0f * 0.29f};
    float rise = rise_list[0];
    float fall = fall_list[0];
    while(1){
        if(up){
            if(btn_prev == 0){
                btn_prev = 1;
                if(cursor != 0){
                    cursor--;
                    cursor_update(cursor, 0);
                }
            }
        }
        if(down){
            if(btn_prev == 0){
                btn_prev = 1;
                if(cursor != 3){
                    cursor++;
                    cursor_update(cursor, 0);
                }
            }
        }
        if(enter){
            if(btn_prev == 0){
                btn_prev = 1;
                rise = rise_list[cursor];
                fall = fall_list[cursor];
                cursor_update(cursor, 1);
                i = 0.0f;
                sample_on = 1;
                idx = 0;
            }
        }
        if((up || down || enter) == 0){
            btn_prev = 0;
        }

        if(i >= 1.0f){
            i = 1.0f;
            step = fall;
        }
        if(i <= 0.0f){
            i = 0.0f;
            step = rise;
        }
        i += step;
        Aout = i;
        ADCdata[idx] = Ain;
            
        if(sample_on){
            idx++;
            if(idx == sample){
                idx = 0;
                sample_on = 0;
                sample_output();
            }
        }
        ThisThread::sleep_for(1ms/10);
    }
}