#ifndef hc_sr04_h
#define hc_sr04_h

#include "Arduino.h"


class HC_SR04
{
private:
	//pin configuration
	int trigPin;
	int echoPin;
	String dist_unit;

	bool cm_unit = false;
	bool mm_unit = false;

	//filtering mode (from 3 measues return second after sorting - median filter)
	int filter_mesurements = 3;

protected:
	int last_measure;	//holds last measured distance

public:
	//only pins configuration
	HC_SR04(int trig, int echo);
	//add init (mm or cm)
	HC_SR04(int trig, int echo, String unit);
	//add number of measurements for filtering 
	HC_SR04(int trig, int echo, String unit, int mesurements);

	//procedure without filtering
	int dist_measure();

	//procedure with filtering
	int dist_measure_filtered();

	//write data to serial
	virtual void write_data();
};

#endif