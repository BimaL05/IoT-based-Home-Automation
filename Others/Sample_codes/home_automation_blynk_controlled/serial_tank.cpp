#include "serial_tank.h"
#include "Arduino.h"
#include "main.h"

unsigned int volume_value;
unsigned char valueh, valuel;

void init_serial_tank(void) 
{
    Serial.begin(19200);
    Serial.write(0xFF); //sincroniza comunicação
    Serial.write(0xFF);
    Serial.write(0xFF);   
}

unsigned int volume(void)
{
    
}
void enable_inlet(void)
{
    
}  
void disable_inlet(void)
{
    
}  
void enable_outlet(void)
{  
    
}
void disable_outlet(void)
{  
    
}
