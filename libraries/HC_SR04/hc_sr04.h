#ifndef hc_sr04_h
#define hc_sr04_h

#include "Arduino.h"


class HC_SR04
{
private:
	int trigPin;
	int echoPin;
	String dist_unit;

	bool cm_unit = false;
	bool mm_unit = false;

	int filter_mesurements = 3;
protected:
	int last_measure;

public:
	HC_SR04(int trig, int echo);
	HC_SR04(int trig, int echo, String unit);
	HC_SR04(int trig, int echo, String unit, int mesurements);

	int dist_measure();

	int dist_measure_filtered();

	virtual void write_data();
};

#endif