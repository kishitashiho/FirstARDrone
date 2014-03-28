#include "ardrone/ardrone.h"

// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// --------------------------------------------------------------------------

// AR.Drone class
ARDrone ardrone;

void roll(int move_count);
void forward(int move_count);
void stop(int move_count);

double vx = 0.0, vy = 0.0, vz = 0.0, vr = 0.0;


int main(int argc, char **argv)
{


	// Initialize
	if (!ardrone.open()) {
		printf("Failed to initialize.\n");
		return -1;
	}

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

	ardrone.takeoff();

	int count=0;

	while (1) {
		// Key input
		int key = cvWaitKey(33);
		if (key == 0x1b) break;

		// Update
		if (!ardrone.update()) break;

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

		if(150<count && count<=200) {
			forward(50);
		}

		if(200<count && count<=300) {
			stop(10);
		}

		if(300<count && count<=400) {
			roll(10);
		}

		if(400<count && count<=500) {
			stop(10);
		}

		if(500<count && count<=600) {
			forward(50);
		}

		if(600<count) {
			break;
		}

		ardrone.move3D(vx, vy, vz, vr);

		count++;  
		printf("count == %d ",count);

		// Change camera
		static int mode = 0;
		if (key == 'c') ardrone.setCamera(++mode%4);

		// Display the image
		cvShowImage("camera", image);
	}

	ardrone.landing();

	// See you
	ardrone.close();

	return 0;
} 


void roll(int move_count){
	for(int i=0;i<move_count;i++){

		ardrone.move3D(0,0,0,100.0);
		printf("roll ");

	}
}

void forward(int move_count){
	for(int i=0;i<move_count;i++){

		ardrone.move3D(400.0,0,0,0);
		printf("forward ");

	}
}

void stop(int move_count){
	for(int i=0;i<move_count;i++){

		ardrone.move3D(0.0,0.0,0.0,0.0);
		printf("stop ");

	}
}