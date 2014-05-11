#include "ardrone/ardrone.h"
#include "time.h"
#include "math.h"
#include "stdio.h"


// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// --------------------------------------------------------------------------

ARDrone ardrone;

using namespace cv;
using namespace std;


void init();
bool Emergency();
double UpdatePosition(double position);
double UpdatePositionData();
void GetSensorPosition();

struct {
	double vx;
	double vy;
	double vz;
	double vr;
	double position_x;
	double position_y;
	double position_z;
} ardrone_parameter;

int h, s, v;


Mat img = ardrone.getImage();

int main(int argc, char **argv) {                          


	if (!ardrone.open()) {
		printf("Failed to initialize.\n");
		return -1;
	}

	int key = cvWaitKey(33);


	init();
	
	FILE *excel;


	excel = fopen ("excelへの出力.xls", "w");

	bool end_flag = false;


	while(1) {

		double altitude = ardrone.getAltitude();

		printf("高度：%f\n",altitude);
		fprintf(excel, "%f\n",altitude);

		// Get an image
		Mat img = ardrone.getImage();
		key = cvWaitKey(1);
		// Take off / Landing 
		if (key == ' ') {
			if (ardrone.onGround()) {
				ardrone.takeoff();
			}else{
				ardrone.landing();
				end_flag = true;
			}
		}

		//高度取得のためのフラグ
		if(end_flag && altitude<0.001)break;


		// Move
		if (key == 0x260000) ardrone_parameter.vx =  1.0;
		if (key == 0x280000) ardrone_parameter.vx = -1.0;
		if (key == 0x250000) ardrone_parameter.vr =  1.0;
		if (key == 0x270000) ardrone_parameter.vr = -1.0;
		if (key == 'q')      ardrone_parameter.vz =  1.0;
		if (key == 'a')      ardrone_parameter.vz = -1.0;


		//ardrone.landing();
		if(!img.empty())imshow("camera",img);

	}
	// See you
	ardrone.close();
	fclose(excel);

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



bool Emergency() {

	// Key input
	int key = cvWaitKey(33);
	if (key == 0x1b) return false;

	// Update
	if (!ardrone.update()) return false;

	// Get an image
	img = ardrone.getImage();

	// Take off / Landing 
	if (key == ' ') {
		if (ardrone.onGround()) ardrone.takeoff();
		else                    ardrone.landing();
	}

	// Move

	if (key == 0x260000) ardrone_parameter.vx =  1.0;
	if (key == 0x280000) ardrone_parameter.vx = -1.0;
	if (key == 0x250000) ardrone_parameter.vr =  1.0;
	if (key == 0x270000) ardrone_parameter.vr = -1.0;
	if (key == 'q')      ardrone_parameter.vz =  1.0;
	if (key == 'a')      ardrone_parameter.vz = -1.0;

	
	// Change camera
	static int mode = 0;
	if (key == 'c') ardrone.setCamera(++mode%4);

	// Display the image
	

	return true;

}