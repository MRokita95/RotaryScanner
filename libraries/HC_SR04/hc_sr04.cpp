#include "hc_sr04.h"
#include "Arduino.h"

HC_SR04::HC_SR04(int trig, int echo) :trigPin(trig), echoPin(echo)
{
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);

	cm_unit = true;

}

HC_SR04::HC_SR04(int trig, int echo, String unit = "cm") :trigPin(trig), echoPin(echo), dist_unit(unit)
{
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);

	if (dist_unit == "cm") cm_unit = true;
	else if (dist_unit == "mm") mm_unit = true;
	else cm_unit = true;
}

HC_SR04::HC_SR04(int trig, int echo, String unit="cm", int mesurements = 3) :trigPin(trig), echoPin(echo), dist_unit(unit), filter_mesurements(mesurements)
{
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);

	if (dist_unit == "cm") cm_unit = true;
	else if (dist_unit == "mm") mm_unit = true;
	else cm_unit = true;
}


int HC_SR04::dist_measure()
{
	long time, distance;

	//calculate speed sound constant value (based on documentation)
	long sound_speed;

	if (cm_unit) sound_speed = 58;
	else if (mm_unit) sound_speed = 580;

	//measurement procedure
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);

	time = pulseIn(echoPin, HIGH);
	distance = time / sound_speed;

	return distance;
}

/*
int HC_SR04::dist_measure_filtered()
{
	int* measurements = new int[filter_mesurements];
	int* zero_marker = measurements;
 

	for (int i = 0; i < filter_mesurements; i++)
	{
		measurements[i] = dist_measure();
	}

	measurements = zero_marker;


	//median filter (sorting measurements and return mid value - cut high and low values)
	for (int i = 0; i < filter_mesurements; i++)
	{
		for (int j = 0; j < filter_mesurements-1; j++,measurements++)
		{
			if (measurements[j] > measurements[j + 1])
			{
				int temp_measure = measurements[j + 1];
				measurements[j + 1] = measurements[j];
				measurements[j] = temp_measure;
			}
		}
		measurements = zero_marker;
	}

	int mid_element = static_cast<int>(filter_mesurements / 2);

	int result = measurements[mid_element];

	delete[] measurements;
	delete[] zero_marker;

	last_measure = result;

	return result;
}
*/


int HC_SR04::dist_measure_filtered()
{
	int measurements [filter_mesurements];


	for (int i = 0; i < filter_mesurements; i++)
	{
		measurements[i] = dist_measure();
	}


	//median filter (sorting measurements and return mid value - cut high and low values)
	for (int i = 0; i < filter_mesurements; i++)
	{
		for (int j = 0; j < filter_mesurements - 1; j++)
		{
			if (measurements[j] > measurements[j + 1])
			{
				int temp_measure = measurements[j + 1];
				measurements[j + 1] = measurements[j];
				measurements[j] = temp_measure;
			}
		}
	}

	int mid_element = static_cast<int>(filter_mesurements / 2);

	last_measure = measurements[mid_element];

	return measurements[mid_element];
}



void HC_SR04::write_data()
{
	Serial.println(last_measure);
}