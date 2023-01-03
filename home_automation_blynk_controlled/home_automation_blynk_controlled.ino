/*************************************************************
Title         :   Home automation using blynk
Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through	Blynk app
Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLviwID9iF"
#define BLYNK_DEVICE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "3q_nXL8nnGlOLbwUQp1YoCQ1IaZQGqnC"

// Comment this out to disable prints
//#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "main.h"
#include "temperature_system.h"
#include "ldr.h"
#include "serial_tank.h"


char auth[] = BLYNK_AUTH_TOKEN;
bool heater_sw;
bool inlet_sw,outlet_sw;
unsigned int tank_volume;

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//To turn ON and OFF COOLER based virtual PIN value
BLYNK_WRITE(COOLER_V_PIN)       // This function is called every time the Virtual Pin 0 state changes
{
  //incoming value from pin V0 in the variable
  int value = param.asInt();
  
  //when cooler button is turned ON in blynk app. then ...
  if (value)
  {
    cooler_control(ON); //... turn ON the cooler
    lcd.setCursor(7,0);
    lcd.print("CL_R ON ");
  }
  else
  {
    cooler_control(OFF);
    lcd.setCursor(7,0);
    lcd.print("CL_R OFF");
  }
  
}

//To turn ON and OFF HEATER based virtual PIN value
BLYNK_WRITE(HEATER_V_PIN )      // This function is called every time the Virtual Pin 0 state changes
{
  //incoming value from pin V0 in the variable
  heater_sw = param.asInt();

  //when cooler button is turned ON in blynk app. then ...
  if (heater_sw)
  {
    heater_control(ON);
    lcd.setCursor(7,0);
    lcd.print("HT_R ON ");
  }
  else
  {
    heater_control(OFF);
    lcd.setCursor(7,0);
    lcd.print("HT_R OFF");
  }
}

//To turn ON and OFF inlet vale based virtual PIN value
BLYNK_WRITE(INLET_V_PIN)
{
  inlet_sw = param.asInt();
  //IF inlet valve button is at logic high then turn ON the inlet valve ELSE OFF
  if(inlet_sw)
  {
    enable_inlet();
    //to print the status of valve on the CLCD
    lcd.setCursor(7,1);
    lcd.print("IN_FL_ON");
  }
  else
  {
    disable_inlet();
    //to print the status of valve on the CLCD
    lcd.setCursor(7,1);
    lcd.print("IN_FL_OFF");
  }

}
//To turn ON and OFF outlet value based virtual switch value
BLYNK_WRITE(OUTLET_V_PIN)
{
  outlet_sw = param.asInt();
  //IF outlet valve button is at logic high then turn ON the outlet valve ELSE OFF
  if(outlet_sw)
  {
    enable_outlet();
    //to print the status of valve on the CLCD
    lcd.setCursor(7,1);
    lcd.print("OT_FL_ON");
  }
  else
  {
    disable_outlet();
    //to print the status of valve on the CLCD
    lcd.setCursor(7,1);
    lcd.print("OT_FL_OFF");
  }
}


//To display temperature and water volume as gauge on the Blynk App 

void update_temperature_reading()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(TEMPERATURE_GAUGE, read_temperature());
  Blynk.virtualWrite(WATER_VOL_GAUGE, volume());

}

//To turn off the heater if the temperature raises above 35 deg C
void handle_temp(void)
{
  //compare temp. with 35 and check if heater is ON
  if ((read_temperature() >  float (35))  &&  heater_sw )
  {
    //to turn off the heater
    heater_sw=0;
    heater_control(OFF);

    //print the notification on the CLCD
    lcd.setCursor(7,0);
    lcd.print("HT_R OFF");
  
    //print the notification on the BLYNK
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Temp. is more than 35 degree celsius\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning OFF the HEATER\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "\n");

    //to reflect OFF on the heater button
    Blynk.virtualWrite(HEATER_V_PIN, OFF);
  
  }
}

//To control water volume above 2000ltrs
void handle_tank(void)
{
  //compare the volume of water within 2000 ltr and check the status of the inlet valve
  if(tank_volume < 2000  && (inlet_sw == OFF))
  {
    //enable inlet valve and print the status on CLCD
    enable_inlet();
    //print the status of valve on the CLCD
    lcd.setCursor(7,1);
    lcd.print("IN_FL_ON ");
    inlet_sw = ON;

    //update the inlet button status on the Blynk app as ON
    Blynk.virtualWrite(INLET_V_PIN, ON);

    //print the notification on the BLYNK
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Water volume is less than 2000 L\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning ON the inlet valve\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "\n");
  }

  //check if tank is FULL i.e. 3000 Litre then turn OFF the inlet valve
  if(tank_volume == 3000  && (inlet_sw == ON))
  {
    //disable inlet valve and print the status on CLCD
    disable_inlet();
    //print the status of valve on the CLCD
    lcd.setCursor(7,1);
    lcd.print("IN_FL_OFF");
    inlet_sw=OFF;

    //update  the inlet button status on the Blynk app as ON
    Blynk.virtualWrite(INLET_V_PIN, OFF);

    //print the notification on the BLYNK
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Water tank is FULL\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning OFF the inlet valve\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "\n");
  }
}

void setup(void)
{
  //connectiong blynk to the server
  Blynk.begin(auth);

  /*initialize the lcd*/
  lcd.init();                     
  lcd.backlight(); //turn on backlight
  lcd.clear(); //clear lcd
  lcd.home(); //cursor to home or first position of lcd

  //initializing GARDEN_LIGHT as o/p pin
  init_ldr();

  //init temperature system
  init_temperature_system();

  //prints at 1st row(0), 2nd col(0)
  lcd.setCursor(0,0);
  lcd.print("T=");

  //set cursor to 2nd line to display the volume of water
  lcd.setCursor(0,1);
  lcd.print("V=");

  //init Serial tank
  init_serial_tank();

  //after 0.1sec=100ms, update the value in the app
  timer.setInterval(100L , update_temperature_reading);

}

void loop(void) 
{
  //control the brightness of GARDEN_LIGHT using LDR sensor
  brightness_control();

  //print temperature
  String temperature;
  temperature = String (read_temperature(), 2);

  lcd.setCursor(2,0);
  //display temperature on the CLCD
  lcd.print(temperature);
  
  //display VOLUME on the CLCD
  tank_volume = volume();
  lcd.setCursor(2,1);
  lcd.print(tank_volume);

  //to check the threshold temp and controlling heater
  handle_temp();

  //to monitor the volume of the water and if less than 2000L turn ON the inlet valve
  handle_tank();

  //to run BLYNK application
  Blynk.run();
  timer.run();

}
