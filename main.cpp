#include "mbed.h"
#include "TextLCD.h"
#include "Dht11.h"
#include "string.h"

//temp
Dht11 sensor(p19);
const char *moisture[] = {"Normal","Low"};
char ch[7];
DigitalOut heatIN(p16); //IN3 1 = OFF, 0 = ON
DigitalOut fanIN(p25); //IN3 1 = OFF, 0 = ON

//light
AnalogIn photocell(p20);
DigitalIn input(p21);
DigitalOut led(p7);
int btn = 0;
float value;

//water
DigitalOut valveIN(p15); //IN2 1 = OFF, 0 = ON
DigitalOut moistureIN(p14); //IN1 1 = OFF, 0 = ON
DigitalIn soil(p12);

//print to serial monitor
//Serial pc(USBTX, USBRX);

//LCD i2C
I2C i2c_lcd(p9,p10);// SDA, SCL
TextLCD_I2C lcd(&i2c_lcd, 0x4E, TextLCD::LCD16x2);// I2C bus, Slave, LCD Type

//tickers
Ticker waterFlip;
Ticker lightFlip;
Ticker tempFlip;

//light function
void lightControl()
{
    lcd.cls();
    lcd.setCursor(TextLCD::CurOff_BlkOff);
    //when button is not pressed and there's low light -> turn on LEDS
    if(value >=80 && btn == 0) {
        led = 1;
        lcd.locate(3,0);
        lcd.printf("Lights ON");
    } else if(btn == 1) {
        led = 1;
        lcd.locate(3,0);
        lcd.printf("Lights ON");
    } else {
        led = 0;
        btn = 0;
        lcd.locate(3,0);
        lcd.printf("Lights OFF");
    }
    wait(1);
}

//water function
void waterControl()
{
    lcd.cls();
    lcd.setCursor(TextLCD::CurOff_BlkOff);
    moistureIN = 0;
    wait(0.2);
    lcd.locate(4,0);
    lcd.printf("Checking");
    lcd.locate(6,1);
    lcd.printf("Water");

    //if soils is dry then open solenoid to let water in
    if(soil == 1) {
        lcd.cls();
        lcd.locate(4,0);
        lcd.printf("Watering");
        valveIN = 0;
        wait(15); //change this value to affect amount of water flow
        valveIN = 1;
        wait(0.2);
        strcpy(ch, moisture[1]);

    }
    wait(0.8);
    moistureIN = 1;
}

//temp function
void tempControl()
{

    lcd.cls();
    lcd.setCursor(TextLCD::CurOff_BlkOff);
    sensor.read();
    lcd.locate(4,0);
    lcd.printf("Checking");
    lcd.locate(6,1);
    lcd.printf("Temp");

    //65 to 80 OK temp
    //less than 65 turn heat element and
    if(sensor.getFahrenheit() <= 55) {
        lcd.cls();
        lcd.locate(4,0);
        lcd.printf("Adjusting");
        lcd.locate(6,1);
        lcd.printf("Temp");
        heatIN = 0;
        fanIN = 0;
        wait(10);
        heatIN = 1;
        fanIN = 1;
    } else if(sensor.getFahrenheit() >= 81) {
        lcd.cls();
        lcd.locate(4,0);
        lcd.printf("Adjusting");
        lcd.locate(6,1);
        lcd.printf("Temp");
        fanIN = 0;
        wait(15);
        fanIN = 1;
    } else {
        lcd.cls();
        lcd.locate(4,0);
        lcd.printf("Temp OK");
    }
    wait(1);

}

int main()
{

    //turning on LCD and setting cursor off
    lcd.setBacklight(TextLCD::LightOn);
    lcd.setCursor(TextLCD::CurOff_BlkOff);

    //call tickers and interrupts every 30 mins
    //not counting wait in the functions
    lightFlip.attach(&lightControl, 30.0);
    waterFlip.attach(&waterControl,30.0);
    tempFlip.attach(&tempControl,30.0);

    //relay INs OFF
    moistureIN = 1;
    valveIN = 1;
    heatIN = 1;
    fanIN = 1;

    //normal starting moisture, I hate C Lang's strings
    strcpy(ch, moisture[0]);

    while(1) {

        lcd.cls();
        sensor.read();

        lcd.locate(0,0);
        lcd.printf("T: %1.0f      H: %d\r\n", sensor.getFahrenheit(), sensor.getHumidity());
        lcd.locate(0,1);
        lcd.printf("M: %s  L: %1.0f\r\n", ch, value);
        wait(0.1);

        value = photocell * 100;

        //button input and btn count
        if(input == 0) {
            led = 1; //LED ON
            wait(0.2);
            btn++;
        }

        //checking if button was pressed a second time to turn off LEDs
        if(btn == 2) {
            led = 0; //LED OFF
            btn = 0;
        }

    }

}
