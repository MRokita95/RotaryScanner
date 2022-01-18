#include "RotaryScanner.h"
#include "Arduino.h"


void RotaryScanner::read_command()
{
	int mode = 0;

	if (Serial.available() > 0)
	{
		String recv_message;
		recv_message = Serial.readStringUntil(stopCommandChar);

		


		if (recv_message == SingleMesureComm)
		{
			String recv_arg;
			recv_arg = Serial.readStringUntil(stopArgumentChar);	//stop position (for mesurment)


			endPos = recv_arg.toDouble();

			data_received = true;
			mode = 1;
		}

		else if (recv_message == MultMesureInRangeComm)
		{
			String recv_arg1;
			String recv_arg2;
			String recv_arg3;
			recv_arg1 = Serial.readStringUntil(stopCommandChar);	//start position
			recv_arg2 = Serial.readStringUntil(stopCommandChar);	//stop position
			recv_arg3 = Serial.readStringUntil(stopArgumentChar);	//step in degres

			startPos = recv_arg1.toDouble();
			endPos = recv_arg2.toDouble();
			stepDiff = recv_arg3.toDouble();

			data_received = true;
			mode = 2;
		}

		else if (recv_message == FullMeasureComm)
		{
			String recv_arg;
			recv_arg = Serial.readStringUntil(stopArgumentChar);	//step in degres

			startPos = 0.0;
			endPos = 360.0;
			stepDiff = recv_arg.toDouble();

			data_received = true;
			mode = 3;
		}

		else if (recv_message == ReqDataComm)
		{
			data_received = true;
			mode = 10;
		}

		else if (recv_message == GoHomeComm)
		{
			data_received = true;
			mode = 20;
		}

	}

	if (data_received)
	{
		task_router(mode);
		
		data_received = false;
	}

}


void RotaryScanner::task_router(int mode)
{
	switch (mode)
	{
	case 1:
		moveAbs(endPos);
		stabilize();
		distance = dist_measure_filtered();

		write_data();
		break;

	case 2:
		moveAbs(startPos);

		do {
			if (startPos > endPos) stepDiff = -stepDiff;

			distance = move_and_measure();
			write_data();

		} while (readActPos() <= endPos);

		moveAbs(startPos);
		write_data();

		break;

	case 3:
		moveAbs(startPos);

		do {
			distance = move_and_measure();
			write_data();

		} while (readActPos() <= endPos);

		moveAbs(startPos);
		write_data();

		break;

	case 10:

		write_data();

	case 20:
		moveAbs(homePos);
		write_data();
	}
}

int RotaryScanner::move_and_measure()
{
	moveRelative(stepDiff);
	stabilize();
	int result = dist_measure_filtered();
	return result;
}

void RotaryScanner::write_data()
{
	Serial.print(readActPos());
	Serial.print(": ");
	Serial.println(last_measure);
}


void RotaryScanner::stabilize()
{
	delay(100);
}