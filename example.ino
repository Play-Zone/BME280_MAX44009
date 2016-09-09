/***********************************************************************
*
* Sample-Sketch for the BME280 / MAX44009 Breakout
*
* Connections:
* Arduino -> Breakout
* VCC        VCC
* GND        GND
* TX         DR
* RX         CT
*
* Make sure there are NO jumpers installed on the board (S0 or S1 to GND)
* Baudrate is 9600
*
* Copyright (c) 2016 GY & PLAY-ZONE GmbH http://www.play-zone.ch

* Permission is hereby granted, free of charge, to any person obtaining a 
* copy of this software and associate documentation files (the "Software"), 
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software.
*
*************************************************************************/

unsigned char Re_buf[15], counter = 0;
unsigned char sign = 0, num = 0;
uint32_t LUX = 0;

typedef struct
{
	uint32_t P;
	uint16_t Temp;
	uint16_t Hum;
	uint16_t Alt;
} bme;

void setup() {
	Serial.begin(9600);
	delay(1);
	Serial.write(0XA5);
	Serial.write(0X03);
	Serial.write(0XA8);
}

void loop() {

	unsigned char i = 0, sum = 0;
	uint16_t data_16[2] = { 0 };
	bme Bme;

	if (sign)
	{
		for (i = 0; i < (num - 1); i++)
		{
			sum += Re_buf[i];
		}
		if (sum == Re_buf[i])
		{
			if (Re_buf[2] == 0x45)
			{

				Bme.Temp = (Re_buf[4] << 8) | Re_buf[5];
				data_16[0] = (Re_buf[6] << 8) | Re_buf[7];
				data_16[1] = (Re_buf[8] << 8) | Re_buf[9];
				Bme.P = (((uint32_t)data_16[0]) << 16) | data_16[1];
				Bme.Hum = (Re_buf[10] << 8) | Re_buf[11];
				Bme.Alt = (Re_buf[12] << 8) | Re_buf[13];

				Serial.print("Temp: ");
				Serial.print((float)Bme.Temp / 100);
				Serial.print(" DegC  PRESS : ");
				Serial.print(((float)Bme.P) / 100);
				Serial.print(" Pa  HUM : ");
				Serial.print((float)Bme.Hum / 100);
				Serial.print(" % ALT:");
				Serial.print(Bme.Alt);
				Serial.print("m");
			}
			else
			{
				LUX = ((uint32_t)Re_buf[4] << 24) | ((uint32_t)Re_buf[5] << 16) | ((uint32_t)Re_buf[6] << 8) | Re_buf[7];

				Serial.print(" lux:");
				Serial.println((float)LUX / 100);
			}
			sign = 0;
		}
	}
}

void serialEvent() {
	while (Serial.available()) {
		Re_buf[counter] = (unsigned char)Serial.read();
		if (counter == 0 && Re_buf[0] != 0x5A) return;
		counter++;
		if (counter == 3)
		{
			if (Re_buf[2] == 0x45)
				num = 15;
			else
				num = 9;
		}
		if (counter == num)
		{
			counter = 0;
			sign = 1;
		}
	}
}

