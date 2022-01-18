#ifndef StepperMotor_h
#define StepperMotor_h

#include "Arduino.h"


class StepperMotor
{
private:
	//configuration pins
	int pinDir;
	int pinStep;
	int pinEnable;
	int steps_per_full_rotation;
	int factors[8] = { 0, 200, 400, 400, 800, 1600, 3200, 6400 };

	//indication variables
	bool inHome;
	double actualPos;

	//move direction detection (based on demanded position)
	bool clockwiseMove;

	//number of steps calculated on parameters and demended position
	long  calculatedSteps;

	//functions calculating demanded steps and move steps 
	int calcSteps(double actAngle, double reqAngle);
	void makeMovement(int steps);


public:
	StepperMotor(int dir, int step, unsigned int step_option);
	//StepperMotor(int dir, int step,int enable, unsigned int step_option);

	//two types of movement
	void moveRelative(double angle);	//go to pos about pos differences
	void moveAbs(double angle);			//go to absolute position
	double readActPos();				//returns actual position

};

#endif