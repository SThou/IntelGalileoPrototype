/*
#include <opencv2\opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat image;
	VideoCapture cap;
	cap.open(0);
	namedWindow("window",1);

	while(1)
	{
		cap>>image;
		imshow("window",image);
		waitKey(33);
	}
}


#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
  Mat im = imread(argc == 2 ? argv[1] : "Lenna.png", 1);
  if (im.empty())
  {
    cout << "Cannot open image!" << endl;
    return -1;
  }
  imshow("image", im);
  waitKey(0);
  return 0;
}

/*
//#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;

int main()
{

IplImage* img =  cvLoadImage("C:/Users/spartans/Documents/Eclipse Projects/opencvtut/src/DetectingContours.jpg");

//show the original image
cvNamedWindow("Original");
cvShowImage("Original",img);

 //smooth the original image using Gaussian kernel to remove noise
cvSmooth(img, img, CV_GAUSSIAN,3,3);

//converting the original image into grayscale
IplImage* imgGrayScale = cvCreateImage(cvGetSize(img), 8, 1);
cvCvtColor(img,imgGrayScale,CV_BGR2GRAY);

cvNamedWindow("GrayScale Image");
cvShowImage("GrayScale Image",imgGrayScale);

//thresholding the grayscale image to get better results
cvThreshold(imgGrayScale,imgGrayScale,100,255,CV_THRESH_BINARY_INV);

cvNamedWindow("Thresholded Image");
cvShowImage("Thresholded Image",imgGrayScale);

CvSeq* contour;  //hold the pointer to a contour
CvSeq* result;   //hold sequence of points of a contour
CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours

//finding all contours in the image
cvFindContours(imgGrayScale, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

//iterating through each contour
while(contour)
{
//obtain a sequence of points of the countour, pointed by the variable 'countour'
result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contour)*0.02, 0);

//if there are 3 vertices  in the contour and the area of the triangle is more than 100 pixels
if(result->total==3 && fabs(cvContourArea(result, CV_WHOLE_SEQ))>100 )
{
//iterating through each point
CvPoint *pt[3];
for(int i=0;i<3;i++){
pt[i] = (CvPoint*)cvGetSeqElem(result, i);
}

//drawing lines around the triangle
cvLine(img, *pt[0], *pt[1], cvScalar(255,0,0),4);
cvLine(img, *pt[1], *pt[2], cvScalar(255,0,0),4);
cvLine(img, *pt[2], *pt[0], cvScalar(255,0,0),4);

}

//obtain the next contour
contour = contour->h_next;
}

//show the image in which identified shapes are marked
cvNamedWindow("Tracked");
cvShowImage("Tracked",img);

cvWaitKey(0); //wait for a key press

//cleaning up
cvDestroyAllWindows();
cvReleaseMemStorage(&storage);
cvReleaseImage(&img);
cvReleaseImage(&imgGrayScale);

return 0;
}



//
//  main.cpp
//  OpenCV Test
//
//  Created by ST on 10/18/14.
//  Copyright (c) 2014 ST. All rights reserved.
//


#include <opencv2\opencv.hpp>
#include <iostream>
#include <stdio.h>
#include "Tserial.h"

using namespace std;
using namespace cv;

// Function Headers
Point detectAndDisplay( Mat frame );

// Global variables
String face_cascade_name = "C:/Users/spartans/Desktop/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "C:/Users/spartans/Desktop/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";

// Serial to Arduino global declarations
  int arduino_command;
  Tserial *arduino_com;
  short MSBLSB = 0;
  unsigned char MSB = 0;
  unsigned char LSB = 0;

// @function main 
int main( void )
{
    VideoCapture capture;
    Mat frame;
    int count = 0;
    int i = 0;
    Point temp;
    Point center;

    // serial to Arduino setup
      arduino_com = new Tserial();
      if (arduino_com!=0) {
      arduino_com->connect("COM9", 115200, spNONE); }

    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };

    //-- 2. Read the video stream
    capture.open( 1 );
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }

    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }

        //-- 3. Apply the classifier to the frame
        if(count < 5)
        count++;
        else
        {
        center = detectAndDisplay( frame );
        count = 0;
        i++;
        }
        if(i == 0)
            	{
            		temp = center;
            	}
            	if(i > 0)
            	{
            		if(temp.x > center.x + 20)
            		{
            			printf("panning right \n");
            		}
            		if(temp.x < center.x - 20)
            		    		{
            		    			printf("panning left \n");
            		    		}
            		i = 0;
            	}

        int c = waitKey(10);
        if( (char)c == 27 ) { break; } // escape
    }

    // Serial to Arduino - shutdown
        arduino_com->disconnect();
        delete arduino_com;
        arduino_com = 0;
    return 0;
}

// @function detectAndDisplay 
Point detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //Point temp;
    Point returnCenter;


    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
    //int x = faces.size();
    //printf("%i",x);
    //cout << faces.size() << endl;
    for ( size_t i = 0; i < faces.size(); i++ )
    {
    	Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
    	returnCenter = center;
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

		   // send X,Y of face center to serial com port	
     // send X axis
	 // read least significant byte 
     LSB = faces[i].x & 0xff;
     // read next significant byte 
     MSB = (faces[i].x >> 8) & 0xff;
	 arduino_com->sendChar( MSB );
	 arduino_com->sendChar( LSB );

    // Send Y axis
	LSB = faces[i].y & 0xff;
	MSB = (faces[i].y >> 8) & 0xff;
	arduino_com->sendChar( MSB );
	arduino_com->sendChar( LSB );
		
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;

        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );

        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
    }
    //-- Show what you got
    imshow( window_name, frame );
    return returnCenter;
}
*/