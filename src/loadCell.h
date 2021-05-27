#ifndef LOADCELL_H
#define LOADCELL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <wiringPi.h>

//Using Wiring Pi pin numbering                           
#define SCK_PIN 12          

#define SCK_On digitalWrite(SCK_PIN, HIGH)
#define SCK_Off digitalWrite(SCK_PIN, LOW)

//Using Wiring Pi pin numbering            
#define DT_PIN 13

#define DT_Read digitalRead(DT_PIN)

void initialize_scale();

bool scale_isReady();

long readScale();

long getAverage(long reads);

#endif