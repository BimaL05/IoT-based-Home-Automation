/*************************************************************
Title       : CLCD
Description : To configure the CLCD
 *************************************************************/

/*To use i2c protocol and uilt in LCD libraray*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* set the LCD address to 0x27 for a 16 chars and 2 line display*/
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup(void)
{ 
    /*initialize the lcd*/
    lcd.init();                     
    lcd.backlight();
    lcd.clear();
    lcd.home();
    
    /*To display string*/
    lcd.setCursor(0, 0);
    lcd.print("Home automation");
    delay(1000);   
}

void loop(void) 
{
       
}
