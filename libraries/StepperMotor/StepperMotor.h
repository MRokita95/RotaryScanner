#ifndef StepperMotor_h
#define StepperMotor_h

#include "Arduino.h"


class StepperMotor
{
private:
	int pinDir;
	int pinStep;
	int pinEnable;
	int steps_per_full_rotation;
	int factors[8] = { 0, 200, 400, 400, 800, 1600, 3200, 6400 };

	bool inHome;
	double actualPos;

	bool clockwiseMove;

	long  calculatedSteps;

	int calcSteps(double actAngle, double reqAngle);
	void makeMovement(int steps);


public:
	StepperMotor(int dir, int step, unsigned int step_option);
	//StepperMotor(int dir, int step,int enable, unsigned int step_option);

	void moveRelative(double angle);
	void moveAbs(double angle);
	double readActPos();

};

#endif