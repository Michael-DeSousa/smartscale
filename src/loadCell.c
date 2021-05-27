#include "loadCell.h"

void initialize_scale()
{
    pinMode(SCK_PIN, OUTPUT);
    pinMode(DT_PIN, INPUT);
    digitalWrite(SCK_PIN, LOW);
       
    return;
}

bool scale_isReady()
{
    return ((DT_Read) == LOW);
}

long readScale()
{
    long count = 0;

    while (!scale_isReady())
    {
        //Wait until scale can send data
    }
    
    digitalWrite(SCK_PIN, LOW);
    delayMicroseconds(1);

    for (int i = 0; i < 24; ++i)
    {
       SCK_On;
       delayMicroseconds(1);
       
       count = count << 1;
               
       if(DT_Read)
       {
           count++;
       }
       
       SCK_Off;
       delayMicroseconds(1);

    }
    
    SCK_On;
    delayMicroseconds(1);
    
    SCK_Off;
    delayMicroseconds(1);
   
   //Data is in 2s compliment https://www.electro-tech-online.com/threads/help-convert-24bit-from-adc-to-decimal.145062/
    count ^= 0x800000;

    return count;
}

long getAverage(long reads)
{
   long sum = 0;
    
    for (int i = 0; i < reads; ++i)
    {
        
        sum += readScale();
    }
    
    long average = sum / reads;
    
	//This is to help convert the value scale, we subtract by the value the scale sends when nothing is on it. https://www.youtube.com/watch?v=nGUpzwEa4vg
	average -= 8606192;
	
	if (average < 0)
	{
		average = 0;
	}
    
    return average;
}
