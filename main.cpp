#include "ardrone/ardrone.h"
#include "time.h"
#include "math.h"

#define RED_NUM 15000		//赤色が検出された数（白の部分）
#define RP_CONTROLER 0.005	//左右の追跡の定数
#define ZP_CONTROLER 0.001	//上下追跡の定数

// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// --------------------------------------------------------------------------

ARDrone ardrone;

using namespace cv;
using namespace std;

void Roll(int interval_sec);
void Forward(int interval_sec);
void Stop(int interval_sec);
void Rise(int interval_sec);
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
IplImage *image = 0;

int main(int argc, char **argv) {                          


	if (!ardrone.open()) {
		printf("Failed to initialize.\n");
		return -1;
	}

	init();

	ardrone.takeoff();

	Stop(10);

	RedChase();

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


void Roll(int interval_sec){

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

void Forward(int interval_sec){

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

void Stop(int interval_sec){

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

void Rise(int interval_sec){

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
		Mat gray_image;
		vector<Vec3f> circles;
		int red_cnt = 0;

		cvtColor(image, hsv_image, CV_BGR2HSV);
		cvtColor(image, gray_image, CV_BGR2GRAY);

		GaussianBlur(gray_image, gray_image, Size(11, 11), 10, 10);
		//Canny(gray_image, gray_image, 50, 100);

		double altitude = ardrone.getAltitude();
		bool flag = emergency();
		cout<<"高度："<<altitude<<endl;

	

		int image_x=0, image_y=0, cnt=0;
		for(int i=0; i<hsv_image.rows; i++) {
			for(int j=0; j<hsv_image.cols; j++) {

				int col = hsv_image.step*i+(j*3);
				if(hsv_image.data[col] < 10 || 170 < hsv_image.data[col]) {

					if(90 < hsv_image.data[col]) {

						niti.data[col] = 255;
						niti.data[col+1] = 255;
						niti.data[col+2] = 255;

						image_x += j;
						image_y += i;
						cnt++;
						red_cnt++;

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

			image_x /= cnt;
			image_y /= cnt;

			circle(image, Point(image_x, image_y), 10, Scalar(255, 0, 0), -1, CV_AA);
			circle(image, Point((image.size().width)/2, (image.size().height)/2), 10, Scalar(255, 0, 0), -1, CV_AA);

		}

		

		if(red_cnt > RED_NUM){
			
			if(image_x < ((image.size().width)/2 - 50) || image_x > ((image.size().width)/2 + 50)) {

				ardrone_parameter.vr = -(image_x - (image.size().width)/2) * RP_CONTROLER;
			
			}

			if(image_y < ((image.size().height)/2 - 20) || image_y > ((image.size().height)/2 + 20)) {

				ardrone_parameter.vz = -(image_y - (image.size().height)/2) * ZP_CONTROLER;

				if(altitude > 1.0) {

					ardrone_parameter.vz = 0.0;
		
				}
			}

			ardrone_parameter.vx = 0.0;
			ardrone_parameter.vy = 0.0;

			
		
		}  else {

			ardrone_parameter.vx = 0.0;
			ardrone_parameter.vy = 0.0;
			ardrone_parameter.vz = 0.0;
			ardrone_parameter.vr = 0.0;

		}

		ardrone.move3D(ardrone_parameter.vx, ardrone_parameter.vy, ardrone_parameter.vz, ardrone_parameter.vr);


		//HoughCircles(gray_image, circles, CV_HOUGH_GRADIENT, 1, 100, 20, 50);

		//vector<Vec3f>::iterator it = circles.begin();
		//for(; it != circles.end(); ++it) {
		//	Point center(saturate_cast<int>((*it)[0]), saturate_cast<int>((*it)[1]));
		//	int radius = saturate_cast<int>((*it)[2]);
		//	circle(image, center, radius,Scalar(0, 255, 0), 2);
		//}

		erode(niti, niti, Mat());
		erode(niti, niti, Mat());
		erode(niti, niti, Mat());

		dilate(niti, niti, Mat());
		dilate(niti, niti, Mat());
		dilate(niti, niti, Mat());

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