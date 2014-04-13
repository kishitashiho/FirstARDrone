#include "ardrone/ardrone.h"
#include "time.h"

// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// --------------------------------------------------------------------------

ARDrone ardrone;

using namespace cv;

void roll(int interval_sec);
void forward(int interval_sec);
void stop(int interval_sec);
void rise(int interval_sec);
void RedChase(void);

void init();
bool emergency();

struct {
	double vx;
	double vy;
	double vz;
	double vr;
} ardrone_parameter;

int h, s, v;
//long int a, b, c;
IplImage *image = 0;

int main(int argc, char **argv) {


	if (!ardrone.open()) {
		printf("Failed to initialize.\n");
		return -1;
	}

	init();

	//ardrone.takeoff();

	RedChase();

	//stop(5);
	//forward(100);
	//stop(50);
	//roll(3);
	//stop(5);
	//forward(100);
	//rise(1);
	//stop(3);

	//ardrone.landing();

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


void roll(int interval_sec){

	//int move_count = 0;
	time_t old_time = time(NULL);

	while (1) {

		ardrone_parameter.vx = 0.0;
		ardrone_parameter.vy = 0.0;
		ardrone_parameter.vz = 0.0;
		ardrone_parameter.vr = 1.0;

		bool controlFlag = emergency();
		if(controlFlag == false) ardrone.close();

		ardrone.move3D(ardrone_parameter.vx, ardrone_parameter.vy, ardrone_parameter.vz, ardrone_parameter.vr);

		//move_count++;
		time_t now_time = time(NULL);

		//if(move_count==move) break;
		if(now_time - old_time > interval_sec) break;
	}
}

void forward(int interval_sec){

	//int move_count = 0;
	time_t old_time = time(NULL);

	while (1) {

		ardrone_parameter.vx = 1.0;
		ardrone_parameter.vy = 0.0;
		ardrone_parameter.vz = 0.0;
		ardrone_parameter.vr = 0.0;

		bool controlFlag = emergency();
		if(controlFlag == false) ardrone.close();

		ardrone.move3D(ardrone_parameter.vx, ardrone_parameter.vy, ardrone_parameter.vz, ardrone_parameter.vr);

		//move_count++;
		time_t now_time = time(NULL);

		//if(move_count==move) break;
		if(now_time - old_time > interval_sec) break;
	}
}

void stop(int interval_sec){

	//int move_count = 0;
	time_t old_time = time(NULL);

	while (1) {

		ardrone_parameter.vx = 0.0;
		ardrone_parameter.vy = 0.0;
		ardrone_parameter.vz = 0.0;
		ardrone_parameter.vr = 0.0;

		bool controlFlag = emergency();
		if(controlFlag == false) ardrone.close();

		ardrone.move3D(ardrone_parameter.vx, ardrone_parameter.vy, ardrone_parameter.vz, ardrone_parameter.vr);

		//move_count++;
		time_t now_time = time(NULL);

		//if(move_count==move) break;
		if(now_time - old_time > interval_sec) break;
	}
}

void rise(int interval_sec){

	//int move_count = 0;
	time_t old_time = time(NULL);

	while (1) {

		ardrone_parameter.vx = 0.0;
		ardrone_parameter.vy = 0.0;
		ardrone_parameter.vz = 1.0;
		ardrone_parameter.vr = 0.0;

		bool controlFlag = emergency();
		if(controlFlag == false) ardrone.close();

		ardrone.move3D(ardrone_parameter.vx, ardrone_parameter.vy, ardrone_parameter.vz, ardrone_parameter.vr);

		//move_count++;
		time_t now_time = time(NULL);

		//if(move_count==move) break;
		if(now_time - old_time > interval_sec) break;
	}
}

void RedChase() {
	    
	while(1) {

		Mat red_image;
		Mat hsv_image;
		Mat image = ardrone.getImage();
		Mat niti = Mat(Size(image.cols,image.rows),CV_8UC3);

		cvtColor(image, hsv_image, CV_BGR2HSV);

		int x=0, y=0, cnt=0;
		for(int i=0; i<hsv_image.rows; i++) {
			for(int j=0; j<hsv_image.cols; j++) {

				int col = hsv_image.step*i+(j*3);
				if(hsv_image.data[col] < 10 || 170 < hsv_image.data[col]) {

					if(90 < hsv_image.data[col]) {

						niti.data[col] = 255;
						niti.data[col+1] = 255;
						niti.data[col+2] = 255;

						x += j;
						y += i;
						cnt++;

					}
				}

				else{

					niti.data[col] = 0;
					niti.data[col+1] = 0;
					niti.data[col+2] = 0;

				}
				
			}

		}
		if(cnt!=0) {
			x /= cnt;
			y /= cnt;

			circle(image, Point(x, y), 10, Scalar(255, 0, 0), -1, CV_AA);
		}




		dilate(niti, niti, Mat());
		erode(niti, niti, Mat());

		if(!niti.empty()) imshow("camera",image);
		if(!niti.empty()) imshow("niti",niti);

		bool controlFlag = emergency();
		if(controlFlag == false) ardrone.close();

	}

}

bool emergency() {

	// Key input
	int key = cvWaitKey(33);
	if (key == 0x1b) return false;

	// Update
	if (!ardrone.update()) return false;

	// Get an image
	Mat img = ardrone.getImage();

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
	//imshow("camera1", img);

	return true;

}