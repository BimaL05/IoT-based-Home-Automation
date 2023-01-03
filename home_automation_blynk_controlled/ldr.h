#ifndef LDR_H
#define LDR_H


#define LDR_SENSOR       A1 //LDR-pin A1
#define GARDEN_LIGHT     3  //LED-pin 3



void init_ldr(void);
void brightness_control(void);

#endif
