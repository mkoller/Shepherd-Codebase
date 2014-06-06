/**
 ******************************************************************************
 * @file    application.cpp
 * @authors  Satish Nair, Zachary Crockett and Mohit Bhoite
 * @version V1.0.0
 * @date    05-November-2013
 * @brief   Tinker application
 ******************************************************************************
  Copyright (c) 2013 Spark Labs, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/  
#include "application.h"

#include "spark_disable_cloud.h"

#define SIMULATION_OFF 0
#define SIMULATION_ON 1

/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);

// Define the pins we're going to call pinMode on
//int led = D0;  // You'll need to wire an LED to this one to see it blink.
int led2 = D7; // This one is the built-in tiny one to the right of the USB jack
int button1 = D1;
int output1 = D2;
int simulation_state;
 
TCPClient client;
byte server[] = { 192, 168, 16, 6 }; // Google

 
void setup()
{
	simulation_state = SIMULATION_OFF;
    pinMode(button1, INPUT_PULLDOWN);
    pinMode(output1, OUTPUT);
	pinMode(led2, OUTPUT);
    delay(500);
 
	digitalWrite(output1, LOW);
	// take control of the LED
	RGB.control(true);
 
    if (client.connect(server, 5005)) {
        digitalWrite(led2, HIGH);
		client.print('C');
    }
    else {
        digitalWrite(led2, LOW);
    }
}
 
void loop() {
    delay(500);
	int buttonVal = digitalRead(button1);
	
	char printVal = 'Z';
	if( buttonVal == 0) {
		printVal = '0';
	}
	else {
		printVal = '1';
	}
	
	if( buttonVal && (simulation_state == SIMULATION_OFF) ) {
		simulation_state = SIMULATION_ON;
		// turn on red light
		RGB.color(255, 0, 0);
        // turn on the vibrator
        digitalWrite(output1, HIGH);
		client.flush();
		client.stop();
	}
	else if( buttonVal && (simulation_state == SIMULATION_ON) ) {
		simulation_state = SIMULATION_OFF;
		client.connect(server, 5005);
		client.print(printVal);
	}
	
	if( client.available() && (simulation_state != SIMULATION_ON) ) {
		char c = client.read();
		digitalWrite(led2, HIGH);
		// turn on the green light
		RGB.color(0, 255, 0);
        // turn off the vibrator
		digitalWrite(output1, LOW);
        
        client.flush();
        client.stop();
        client.connect(server, 5005);
        client.print(printVal);
	}
	else if(simulation_state != SIMULATION_ON) {
		// turn on red light
		RGB.color(255, 0, 0);
        // turn on the vibrator
        digitalWrite(output1, HIGH);
        
		digitalWrite(led2, LOW);
        
		client.flush();
        client.stop();
		delay(200);
		
        client.connect(server, 5005);
		client.write('R');
	}
	
	delay(500);
}

/*******************************************************************************
 * Function Name  : tinkerDigitalRead
 * Description    : Reads the digital value of a given pin
 * Input          : Pin 
 * Output         : None.
 * Return         : Value of the pin (0 or 1) in INT type
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerDigitalRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT_PULLDOWN);
		return digitalRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT_PULLDOWN);
		return digitalRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerDigitalWrite
 * Description    : Sets the specified pin HIGH or LOW
 * Input          : Pin and value
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerDigitalWrite(String command)
{
	bool value = 0;
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(command.substring(3,7) == "HIGH") value = 1;
	else if(command.substring(3,6) == "LOW") value = 0;
	else return -2;

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, value);
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		digitalWrite(pinNumber+10, value);
		return 1;
	}
	else return -3;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogRead
 * Description    : Reads the analog value of a pin
 * Input          : Pin 
 * Output         : None.
 * Return         : Returns the analog value in INT type (0 to 4095)
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerAnalogRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT);
		return analogRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT);
		return analogRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogWrite
 * Description    : Writes an analog value (PWM) to the specified pin
 * Input          : Pin and Value (0 to 255)
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerAnalogWrite(String command)
{
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	String value = command.substring(3);

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		analogWrite(pinNumber, value.toInt());
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		analogWrite(pinNumber+10, value.toInt());
		return 1;
	}
	else return -2;
}
