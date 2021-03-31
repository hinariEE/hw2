#include "mbed.h"
#include "uLCD_4DGL.h"

DigitalIn up(D3);
DigitalIn down(D6);
DigitalIn enter(D5);
AnalogOut Aout(D7);
AnalogIn Ain(A0);
uLCD_4DGL uLCD(D1, D0, D2);

int idx;
int smaple = 512;
float ADCdata[512];

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
    uLCD.locate(0, pos);
    uLCD.printf("v");
}

void sample_output()
{
    for(int i = 0; i < smaple; i++){
        printf("%f\n", ADCdata[i]);
        ThisThread::sleep_for(5ms);
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
    uLCD.printf("100 Hz");
    uLCD.locate(2, 1);
    uLCD.printf("200 Hz");
    
    int cursor = 0;
    uLCD.locate(0, 0);
    uLCD.printf("v");
    
    int btn_prev = 0;
    int sample_on = 0;
    float i = 0.0f;
    float step;
    float rise_list[] = { 10.0f / 9.0f * 0.00001f * 100.0f * 2.85f,  // 100 Hz
                          10.0f / 9.0f * 0.00001f * 200.0f * 2.84f}; // 200 Hz
    float fall_list[] = {-10.0f / 1.0f * 0.00001f * 100.0f * 2.88f,
                         -10.0f / 1.0f * 0.00001f * 200.0f * 2.88f};
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
                if(cursor != 1){
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
            if(idx == smaple){
                idx = 0;
                sample_on = 0;
                sample_output();
                ThisThread::sleep_for(1ms/10);
            }
        }
        
    }
}