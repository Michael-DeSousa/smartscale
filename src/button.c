#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include "loadCell.c"
#include "ssd1306_i2c.h"

//For better accuracy we use the pull-up resistors on Pi. 0 Means pressed
#define PRESSED 0
#define RELEASED 1

//Using Wiring Pi Pin Numbering
#define PowerButton 0
#define TareButton 1
#define UnitButton 2

//Global Variables

//Power for the whole system. We don't really shut down components, we just block their output
enum Power_States { Off, On };
unsigned char power = Off;

//Char Arrays that hold info for OLED
char weightBuffer[10];

//Raw Weight value from scale
long weight = 0;

//Converted Weight value from scale
long convertedWeight = 0;

//Tare weight offset
long offset = 0;

//Measurement options for scale
enum Unit_Types {Grams, Ounces, Pounds};
unsigned char unitFactor = Grams;

//Global Variables END


enum PowerButton_States { PowerButton_Init, PowerButton_Idle, PowerButton_Release, PowerButton_Press } PowerButton_State;

int PowerButtonTick(int PowerButton_State)
{
    //State Transitions
    switch(PowerButton_State)
    {
        case PowerButton_Init:
        power = Off;
        PowerButton_State = PowerButton_Idle;
        break;
        
        case PowerButton_Idle:
        if (digitalRead(PowerButton) == PRESSED)
        {
            PowerButton_State = PowerButton_Press;
        }
        break;
        
        case PowerButton_Release:
        PowerButton_State = PowerButton_Idle;
        break;
        
        case PowerButton_Press:
        if (digitalRead(PowerButton) == RELEASED)
        {
            PowerButton_State = PowerButton_Release;
        }
        break;

        default:
        break;
    }
    
    //State Actions
    switch(PowerButton_State)
    {
        case PowerButton_Init:
        break;
        
        case PowerButton_Idle:
        break;
        
        case PowerButton_Release:
        power = !power;
        break;
        
        case PowerButton_Press:
        break;
	
        default:
        break;
    }
    
    return PowerButton_State;
}

enum TareButton_States { TareButton_Init, TareButton_Idle, TareButton_Release, TareButton_Press } TareButton_State;

int TareButtonTick(int TareButton_State)
{
    //State Transitions
    switch(TareButton_State)
    {
        case TareButton_Init:
        offset = 0;
        if(power == On)
        {
            TareButton_State = TareButton_Idle;
        }          
        break;
        
        case TareButton_Idle:
        if (power == Off)
        {
            TareButton_State = TareButton_Init;
        }
        else if ((digitalRead(TareButton) == PRESSED) && power == On)
        {
            TareButton_State = TareButton_Press;
        }  
        break;
        
        case TareButton_Release:
        TareButton_State = TareButton_Idle; 
        break;
        
        case TareButton_Press:
        if (digitalRead(TareButton) == RELEASED)
        {
            TareButton_State = TareButton_Release;
        }   
        break;
        
        default:
        break;
    }
    
    //State Actions
    switch(TareButton_State)
    {
        case TareButton_Init:
        break;
        
        case TareButton_Idle:
        break;
        
        case TareButton_Release:
        offset = weight;
        break;
        
        case TareButton_Press:
        break;
        
        default:
        break;
    }
    
    return TareButton_State;
}

enum UnitButton_States { UnitButton_Init, UnitButton_Idle, UnitButton_Release, UnitButton_Press } UnitButton_State;

int UnitButtonTick(int UnitButton_State)
{
    //State Transitions
    switch(UnitButton_State)
    {
        case UnitButton_Init:
        unitFactor = Grams;
        if (power == On)
        {
          UnitButton_State = UnitButton_Idle;
        }        
        break;
        
        case UnitButton_Idle:
        if (power == Off)
        {
            UnitButton_State = UnitButton_Init;
        }
        if ((digitalRead(UnitButton) == PRESSED) && power == On)
        {
            UnitButton_State = UnitButton_Press;
        }                 
        break;
        
        case UnitButton_Release:
        UnitButton_State = UnitButton_Idle;
        break;
        
        case UnitButton_Press:
        if (digitalRead(UnitButton) == RELEASED)
        {
            UnitButton_State = UnitButton_Release;
        }
        break;
        
        default:
        break;
    }
    
    //State Actions
    switch(UnitButton_State)
    {
        case UnitButton_Init:
        break;
        
        case UnitButton_Idle:
        break;
        
        case UnitButton_Release:
        switch(unitFactor)
        {
            case Grams:
	    printf("Ounces: ");
            unitFactor = Ounces;
            break;
            
            case Ounces:
	    printf("Pounds: ");
            unitFactor = Pounds;
            break;
            
            case Pounds:
	    printf("Grams: ");
            unitFactor = Grams;
            break;
        }
        break;
        
        case UnitButton_Press:
        break;
        
        default:
        break;
    }
    
    return UnitButton_State;
}

enum Scale_States { Scale_Init, Scale_Idle, Scale_On} Scale_State;

int ScaleTick(int Scale_State)
{   
    switch(Scale_State)
    {
        case Scale_Init:
        initialize_scale();
        Scale_State = Scale_Idle;          
        break;
        
        case Scale_Idle:
        if (power == On)
        {
            Scale_State = Scale_On;
        }
        break;
        
        case Scale_On:
        if (power == Off)
        {
            Scale_State = Scale_Idle;
        }
        break;
        
        default:
        break;
    }
    
    switch(Scale_State)
    {
        case Scale_Init:
        break;
        
        case Scale_Idle:
        weight = 0;
        break;
        
        case Scale_On:
        weight = getAverage(10);
        
        //These magic numbers are calibration factors. https://www.youtube.com/watch?v=nGUpzwEa4vg        
        switch(unitFactor)
        {
            case Grams:
            convertedWeight = weight/374;
            convertedWeight -= (offset/374);
            break;
            
            case Ounces:
            convertedWeight = weight/10595;
            convertedWeight -= (offset/10595);
            break;
            
            case Pounds:
            convertedWeight = weight/169644;
            convertedWeight -= (offset/169644);
            break; 
            
            default:
            break;           
        }
        //printf("%ld\n", convertedWeight);       
        break;
        
        default:
        break;
    }
    
    return Scale_State;
}

enum Oled_States {Oled_Init, Oled_Off, Oled_On} Oled_State;

int OledTick(int Oled_State)
{
	char gramsBuffer[20] = "Grams: ";
	char ouncesBuffer[20] = "Ounces: ";
	char poundsBuffer[20] = "Pounds: ";
	switch(Oled_State)
	{
		case Oled_Init:
		ssd1306_clearDisplay();
		delay(100);
		Oled_State = Oled_Off;
		break;

		case Oled_Off:
		if (power == Off)
		{
		   Oled_State = Oled_Off;
		}
		else if (power == On)
		{
		   Oled_State = Oled_On;
		}
		break;

		case Oled_On:
		if (power == Off)
		{
		   Oled_State = Oled_Off;
		}
		else if (power == On)
		{
		   Oled_State = Oled_On;
		}
		break;

		default:
		break;
	}

	switch (Oled_State)
	{
		case Oled_Init:
		break;

		case Oled_Off:
		ssd1306_clearDisplay();
		break;

		case Oled_On:
		sprintf(weightBuffer, "%ld", convertedWeight);
		switch(unitFactor)
		{
		   case Grams:
		   strcat(gramsBuffer, weightBuffer);
		   ssd1306_clearDisplay();
		   ssd1306_drawString(gramsBuffer);
		   ssd1306_display();
		   printf("%s\n", gramsBuffer);
		   break;

		   case Ounces:
		   strcat(ouncesBuffer, weightBuffer);
		   ssd1306_clearDisplay();
		   ssd1306_drawString(ouncesBuffer);
		   ssd1306_display();
		   printf("%s\n", ouncesBuffer);
		   break;

		   case Pounds:
		   strcat(poundsBuffer, weightBuffer);
		   ssd1306_clearDisplay();
		   ssd1306_drawString(poundsBuffer);
		   ssd1306_display();
		   printf("%s\n", poundsBuffer);
		   break;

		   default:
		   break;
		}
		break;

		default:
		break;
	}

	return Oled_State;
}

int main(void)
{
	printf("TEST\n");
	if (wiringPiSetup() < 0)
	{
		printf("Wiring Pi Setup Failed!\n");
		return 1 ;
	}
	
	//Power Button Setup
	pinMode(PowerButton, INPUT);
	pullUpDnControl(PowerButton, PUD_UP);
	unsigned char power_state = PowerButton_Init;
	printf("Power Button Initialized!\n");
 
 	pinMode(TareButton, INPUT);
	pullUpDnControl(TareButton, PUD_UP);
	unsigned char tare_state = TareButton_Init;
	printf("Tare Button Initialized!\n");
 
  pinMode(UnitButton, INPUT);
	pullUpDnControl(UnitButton, PUD_UP);
	unsigned char unit_state = UnitButton_Init;
	printf("Unit Button Initialized!\n");
 
	unsigned char scale_state = Scale_Init;
	printf("Scale Initialized!\n");
	
	unsigned char oled_state = Oled_Init;
	char* text = "SSD1306 Initialized!";
	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	ssd1306_display();
	ssd1306_clearDisplay();
	delay(1000);

	ssd1306_drawString(text);
	ssd1306_display();
	delay(1000);
	
    while (1) 
    {
       power_state = PowerButtonTick(power_state);
       tare_state = TareButtonTick(tare_state);
       unit_state = UnitButtonTick(unit_state);
       scale_state = ScaleTick(scale_state);
       oled_state = OledTick(oled_state);
       delay(100);
    }

   
    return 0;
}

