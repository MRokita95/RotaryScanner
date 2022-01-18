#include "StepperMotor.h"
#include "Arduino.h"

StepperMotor::StepperMotor(int dir, int step, unsigned int step_option):pinDir(dir), pinStep(step)
{
	pinMode(pinDir, OUTPUT);
	pinMode(pinStep, OUTPUT);

	digitalWrite(pinDir, LOW);
	digitalWrite(pinStep, 0);


	steps_per_full_rotation = factors[step_option];


	inHome = true;
	actualPos = 0.0;
}



int StepperMotor::calcSteps(double actAngle, double reqAngle)
{

	if (actAngle < reqAngle)
	{
		clockwiseMove = true;
	}
	else
	{
		clockwiseMove = false;
	}

	double result = ((static_cast<double>(steps_per_full_rotation) * (reqAngle - actAngle))/360.0 );

	calculatedSteps = int(result);

	return  int(result);

}

void StepperMotor::makeMovement(int steps)
{
	if (clockwiseMove)
	{
		digitalWrite(pinDir, HIGH);
	}
	else
	{
		digitalWrite(pinDir, LOW);
	}

	for (int i = 1; i <= abs(steps); i++)
	{
		digitalWrite(pinStep,HIGH);
		delayMicroseconds(200);
		digitalWrite(pinStep,LOW);
		delayMicroseconds(100);

	}
}

void StepperMotor::moveRelative(double angle)
{
	int steps = calcSteps(actualPos, actualPos+angle);
	
	makeMovement(steps);

	actualPos += (static_cast<double>(steps)/ steps_per_full_rotation) * 360.0 ;

	/*if (actualPos > 360)
	{
		actualPos = actualPos - 360;
	}*/
}

void StepperMotor::moveAbs(double angle)
{
	int steps = calcSteps(actualPos, angle);
	
	makeMovement(steps );

	actualPos += (static_cast<double>(steps) / steps_per_full_rotation) * 360.0;

	/*if (actualPos > 360)
	{
		actualPos = actualPos - 360;
	}*/
}



double StepperMotor::readActPos()
{
	return actualPos;
}

