# SmartScale - A "smart" food scale!

![Smartscale](https://user-images.githubusercontent.com/22509729/119906851-e738d280-bf03-11eb-83fb-0f4c48dc33de.jpg)

## Demo
[Link](https://www.youtube.com/watch?v=XH4YWV54YcU)

## Project Overview

This project was created by Michael De Sousa in the fall of 2019.

A food scale is a common kitchen device that allows a user to weigh their food portions. The user can place their food on the scale and read its weight in a variety of different unit types. My project takes this concept and builds upon it by adding a barcode scanner, food database, and WiFi functionality. 

The scale uses a barcode scanner to look up foods in an SQLite database. Then, it calculates and displays nutritional values (calories, carbs, fats, etc.) in real-time based on the portion currently on the scale. Finally, the user can press a button on the scale to upload these nutritional values to an online spreadsheet/food diary. This helps automate calorie counting to make it an easier/faster process.

![Smart Scale-1](https://user-images.githubusercontent.com/22509729/119907564-6549a900-bf05-11eb-9076-25894addcaf4.png)

## Technologies Used

Hardware: 
- Raspberry Pi Zero W
- Symcode Barcode Scanner
- 5kg Load Cell + HX711 Amplifier
- Adafruit PiOLED 128x32 0.96” I2C Display

Programming Languages:
- C
- Python
- SQL

Libraries:
- [SQLite3](https://www.sqlite.org/index.html)
- [Wiring Pi](http://wiringpi.com/)
- [Gspread](https://docs.gspread.org/en/latest/)
- [OLED Display](https://github.com/adafruit/Adafruit_SSD1306)

*Note: This project was built upon my previous (simpler) food scale project that used the Atmega1284. For this project I ported my original code over to the Raspberry Pi. See my other github repo for the base code*

## Additional Documentation
For additional documentation on this project, including a full Bill of Materials, a pinout diagram, my testing methodology, etc. please see my report [here.](https://docs.google.com/document/d/1Y7rQbFt16Oc6LtM1B5nCzM-uwgqWTCO8CRwr7NF1Zvk/edit?usp=sharing)
