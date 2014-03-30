#include "ardrone/ardrone.h"

// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// --------------------------------------------------------------------------

ARDrone ardrone;

void roll(int move);
void forward(int move);
void stop(int move);
void init();
bool emergency();

double vx = 0.0, vy = 0.0, vz = 0.0, vr = 0.0;

int main(int argc, char **argv) {


	if (!ardrone.open()) {
		printf("Failed to initialize.\n");
		return -1;
	}

	init();

	ardrone.takeoff();

	stop(50);
	forward(100);
	stop(50);
	roll(70);
	stop(50);
	forward(100);
	stop(50);

	ardrone.landing();

	// See you
	ardrone.close();

	return 0;
}

void init() {
	// Battery
	printf("Battery = %d%%\n", ardrone.getBatteryPercentage());

	// Instructions
	printf("***************************************\n");
	printf("*       CV Drone sample program       *\n");
	printf("*           - How to Play -           *\n");
	printf("***************************************\n");
	printf("*                                     *\n");
	printf("* - Controls -                        *\n");
	printf("*    'Space' -- Takeoff/Landing       *\n");
	printf("*    'Up'    -- Move forward          *\n");
	printf("*    'Down'  -- Move backward         *\n");
	printf("*    'Left'  -- Turn left             *\n");
	printf("*    'Right' -- Turn right            *\n");
	printf("*    'Q'     -- Move upward           *\n");
	printf("*    'A'     -- Move downward         *\n");
	printf("*                                     *\n");
	printf("* - Others -                          *\n");
	printf("*    'C'     -- Change camera         *\n");
	printf("*    'Esc'   -- Exit                  *\n");
	printf("*                                     *\n");
	printf("***************************************\n\n");
}


void roll(int move){

	int move_count = 0;

	while (1) {

		vx = 0.0;
		vy = 0.0;
		vz = 0.0;
		vr = 1.0;

		bool controlFlag = emergency();
		if(controlFlag == false) ardrone.close();

		ardrone.move3D(vx, vy, vz, vr);

		move_count++;

		if(move_count==move) break;

	}
}

void forward(int move){

	int move_count = 0;

	while (1) {

		vx = 1.0;
		vy = 0.0;
		vz = 0.0;
		vr = 0.0;

		bool controlFlag = emergency();
		if(controlFlag == false) ardrone.close();

		ardrone.move3D(vx, vy, vz, vr);

		move_count++;

		if(move_count==move) break;

	}
}

void stop(int move){

	int move_count = 0;

	while (1) {

		vx = 0.0;
		vy = 0.0;
		vz = 0.0;
		vr = 0.0;

		bool controlFlag = emergency();
		if(controlFlag == false) ardrone.close();

		ardrone.move3D(vx, vy, vz, vr);

		move_count++;

		if(move_count==move) break;

	}
}

bool emergency() {

	// Key input
	int key = cvWaitKey(33);
	if (key == 0x1b) false;

	// Update
	if (!ardrone.update()) false;

	// Get an image
	IplImage *image = ardrone.getImage();

	// Take off / Landing 
	if (key == ' ') {
		if (ardrone.onGround()) ardrone.takeoff();
		else                    ardrone.landing();
	}

	// Move

	if (key == 0x260000) vx =  1.0;
	if (key == 0x280000) vx = -1.0;
	if (key == 0x250000) vr =  1.0;
	if (key == 0x270000) vr = -1.0;
	if (key == 'q')      vz =  1.0;
	if (key == 'a')      vz = -1.0;

	
	// Change camera
	static int mode = 0;
	if (key == 'c') ardrone.setCamera(++mode%4);

	// Display the image
	cvShowImage("camera", image);

	return true;

}