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

//inherits from stepper motor and distance sensor classes in private 
class RotaryScanner : private StepperMotor, private HC_SR04
{
private:
	//chars in serial frame (for separating data and command types)
	char stopCommandChar = ' ';
	char stopArgumentChar = '\n';

	//data receiver indicator
	bool data_received = false;

	//data to be overriten from serial port
	double startPos = 0.0;
	double endPos = 0.0;
	double stepDiff = 0.0;

	//definition of home position
	double homePos = 0.0;

	//measured distance
	int distance;

	//task routing based on receved command from app - starts proper sequence of events
	void task_router(int mode);

	//move to set Position and returns measuren value
	int move_and_measure();

	//short delay time after move and before measurement
	void stabilize();

public:
	//during constructing object init measuring
	RotaryScanner(int mot_dir, int mot_step, int sens_trig, int sens_echo): 
		StepperMotor(mot_dir, mot_step, 5), HC_SR04(sens_trig, sens_echo) 
	{
		dist_measure();
	};

	//during constructing object init measuring
	RotaryScanner(int mot_dir, int mot_step, int sens_trig, int sens_echo, String unit, int measurements) : 
		StepperMotor(mot_dir, mot_step, 5), HC_SR04(sens_trig, sens_echo, unit, measurements) 
	{
		dist_measure_filtered();
	};


	void read_command();		//read command from UART (QT application)

	//virtual function inherited fro  HC_SR04 class and overriten (for extension of actual position)
	void write_data() override;	
	

};


#endif