#ifndef RotaryScanner_h
#define RotaryScanner_h

#include "Arduino.h"
#include "StepperMotor.h"
#include "hc_sr04.h"

#define SingleMesureComm "SM_P"		//Single Measurement Position (...)
#define MultMesureInRangeComm "MM_IR_P"		//Multiple Measurements In Range Position (...)
#define FullMeasureComm "FM_P"		//Full Measurement (360 degrees) 
#define ReqDataComm "GET_DAT"           //Request for data from appl (Position + distance)
#define GoHomeComm "HOM"           //Go to 0.0 position (home)

class RotaryScanner : private StepperMotor, private HC_SR04
{
private:
	char stopCommandChar = ' ';
	char stopArgumentChar = '\n';

	bool data_received = false;

	double startPos = 0.0;
	double endPos = 0.0;
	double stepDiff = 0.0;

	double homePos = 0.0;

	int distance;

	void task_router(int mode);

	int move_and_measure();

	
	void stabilize();

public:
	RotaryScanner(int mot_dir, int mot_step, int sens_trig, int sens_echo): 
		StepperMotor(mot_dir, mot_step, 5), HC_SR04(sens_trig, sens_echo) 
	{
		dist_measure();
	};

	RotaryScanner(int mot_dir, int mot_step, int sens_trig, int sens_echo, String unit, int measurements) : 
		StepperMotor(mot_dir, mot_step, 5), HC_SR04(sens_trig, sens_echo, unit, measurements) 
	{
		dist_measure_filtered();
	};


	void read_command();		//read command from UART (QT application)

	void write_data() override;
	

};




/*
class RotaryScanner 
{
private:
	StepperMotor motor;
	HC_SR04 sensor;

public:
	RotaryScanner(int mot_dir, int mot_step, int sens_trig, int sens_echo) :  motor(mot_dir, mot_step, 5),  sensor(sens_trig, sens_echo) {};

};
*/
#endif