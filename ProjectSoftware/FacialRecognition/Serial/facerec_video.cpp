/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */
/*
#define cascade_path "C:/Users/spartans/Desktop/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"
#define csv_path "C:/Users/spartans/Desktop/opencv_backup/data_csv.csv"
#define device_id "0"

#include <opencv2\opencv.hpp>
#include <opencv2\contrib\contrib.hpp>
#include "Tserial.h"
//#include "opencv2/core.hpp"
//#include "opencv2/face.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
//using namespace cv::face;
using namespace std;

// Serial to Arduino global declarations
  int arduino_command;
  Tserial *arduino_com;
  short MSBLSB = 0;
  unsigned char MSB = 0;
  unsigned char LSB = 0;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

int main(int argc, const char *argv[]) {
    // Get the path to your CSV:
    string fn_haar = string(cascade_path);
    string fn_csv = string(csv_path);
    int deviceId = atoi(device_id);
    // These vectors hold the images and corresponding labels:
    vector<Mat> images;
    vector<int> labels;
    //unsigned char c;
    //HK: serial to Arduino setup
          arduino_com = new Tserial();
          if (arduino_com!=0) {
        	  cout << "Connecting yo" << endl;
          arduino_com->connect("COM8", 9600, spNONE); }



    // Read in the data (fails if no valid input filename is given, but you'll get an error message):
    try {
        read_csv(fn_csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size AND we need to reshape incoming faces to this size:
    int im_width = images[0].cols;
    int im_height = images[0].rows;
    // Create a FaceRecognizer and train it on the given images:
    Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    model->train(images, labels);
    // That's it for learning the Face Recognition model. You now
    // need to create the classifier for the task of Face Detection.
    // We are going to use the haar cascade you have specified in the
    // command line arguments:
    //
    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
    // Get a handle to the Video device:
    VideoCapture cap(deviceId);
    // Check if we can use this device at all:
    if(!cap.isOpened()) {
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH,400);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,400);
    // Holds the current frame from the Video device:
    Mat frame;
    for(;;) {
        cap >> frame;
        // Clone the current frame:
        Mat original = frame.clone();
        // Convert the current frame to grayscale:
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        // Find the faces in the frame:
        vector< Rect_<int> > faces;
        //std::vector <Rect> faces;
        haar_cascade.detectMultiScale(gray, faces);
        // At this point you have the position of the faces in
        // faces. Now we'll get the faces, make a prediction and
        // annotate it in the video. Cool or what?
        for(int i = 0; i < faces.size(); i++) {
            // Process face by face:
            Rect face_i = faces[i];
            // Crop the face from the image. So simple with OpenCV C++:
            Mat face = gray(face_i);
            // Since I am showing the Fisherfaces algorithm here, I also show how to resize the
            // face you have just found:
            Mat face_resized;
            cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
            // Now perform the prediction, see how easy that is:
            int prediction = model->predict(face_resized);
            // And finally write all we've found out to the original image!
            // First of all draw a green rectangle around the detected face:
            rectangle(original, face_i, CV_RGB(0, 255,0), 1);
            // Create the text we will annotate the box with:
            string box_text = format("Prediction = %d", prediction);
            // Calculate the position for annotated text (make sure we don't
            // put illegal values in there):
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
            //HK:Center of the current bounding rectangle
            //Point center(face_i.x + face_i.width/2, face_i.y + face_i.height/2);
            // And now put it into the image:
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
                     // send X,Y of face center to serial com port
                          // send X axis
                     	 // read least significant byte
                          LSB = face_i.x & 0xff;
                          // read next significant byte
                          MSB = (face_i.x >> 8) & 0xff;
                     	 arduino_com->sendChar( MSB );
                     	 arduino_com->sendChar( LSB );

                         // Send Y axis
                     	LSB = face_i.y & 0xff;
                     	MSB = (face_i.y >> 8) & 0xff;
                     	arduino_com->sendChar( MSB );
                     	arduino_com->sendChar( LSB );

        }
        // Show the result:
        imshow("face_recognizer", original);
        // And display it:
        char key = (char) waitKey(20);
        // Exit this loop on escape:
        if(key == 27)
            break;
    }
    // Serial to Arduino - shutdown
            arduino_com->disconnect();
            delete arduino_com;
            arduino_com = 0;
    return 0;
}
*/

/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#define cascade_path "C:/Users/spartans/Desktop/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"
#define csv_path "C:/Users/spartans/Desktop/opencv_backup/data_csv.csv"
#define device_id "0"

#include <opencv2\opencv.hpp>
#include <opencv2\contrib\contrib.hpp>
#include "SerialClass.h"
//#include "opencv2/core.hpp"
//#include "opencv2/face.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
//using namespace cv::face;
using namespace std;

// Serial to Arduino global declarations
  int arduino_command;
  Serial* arduino_com = new Serial("COM8");
  short MSBLSB = 0;
  unsigned char MSB = 0;
  unsigned char LSB = 0;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

int main(int argc, const char *argv[]) {
    // Get the path to your CSV:
    string fn_haar = string(cascade_path);
    string fn_csv = string(csv_path);
    int deviceId = atoi(device_id);
    // These vectors hold the images and corresponding labels:
    vector<Mat> images;
    vector<int> labels;
    char buffer[8];
    bool writedata;
    //unsigned char c;
    //HK: serial to Arduino setup check
    if (arduino_com->IsConnected())
    {
    		cout<< "connected yo" << endl;
    	//printf("Connected");
    }
    	// Read in the data (fails if no valid input filename is given, but you'll get an error message):
    try {
        read_csv(fn_csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size AND we need to reshape incoming faces to this size:
    int im_width = images[0].cols;
    int im_height = images[0].rows;
    // Create a FaceRecognizer and train it on the given images:
    Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    model->train(images, labels);
    // That's it for learning the Face Recognition model. You now
    // need to create the classifier for the task of Face Detection.
    // We are going to use the haar cascade you have specified in the
    // command line arguments:
    //
    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
    // Get a handle to the Video device:
    VideoCapture cap(deviceId);
    // Check if we can use this device at all:
    if(!cap.isOpened()) {
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH,400);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,400);
    // Holds the current frame from the Video device:
    Mat frame;
    for(;;) {
        cap >> frame;
        // Clone the current frame:
        Mat original = frame.clone();
        // Convert the current frame to grayscale:
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        // Find the faces in the frame:
        vector< Rect_<int> > faces;
        //std::vector <Rect> faces;
        haar_cascade.detectMultiScale(gray, faces);
        // At this point you have the position of the faces in
        // faces. Now we'll get the faces, make a prediction and
        // annotate it in the video. Cool or what?
        for(int i = 0; i < faces.size(); i++) {
            // Process face by face:
            Rect face_i = faces[i];
            // Crop the face from the image. So simple with OpenCV C++:
            Mat face = gray(face_i);
            // Since I am showing the Fisherfaces algorithm here, I also show how to resize the
            // face you have just found:
            Mat face_resized;
            cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
            // Now perform the prediction, see how easy that is:
            int prediction = model->predict(face_resized);
            // And finally write all we've found out to the original image!
            // First of all draw a green rectangle around the detected face:
            rectangle(original, face_i, CV_RGB(0, 255,0), 1);
            // Create the text we will annotate the box with:
            string box_text = format("Prediction = %d", prediction);
            // Calculate the position for annotated text (make sure we don't
            // put illegal values in there):
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
            //HK:Center of the current bounding rectangle
            //Point center(face_i.x + face_i.width/2, face_i.y + face_i.height/2);
            // And now put it into the image:
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
                     // send X,Y of face center to serial com port
                          // send X axis
                     	 // read least significant byte
                          LSB = face_i.x & 0xff;
                          // read next significant byte
                          MSB = (face_i.x >> 8) & 0xff;
                     	 writedata = arduino_com->WriteData(buffer,MSB );

                     	 cout << writedata << endl;
                     	writedata =  arduino_com->WriteData(buffer, LSB );

                     	 cout << writedata << endl;
                         // Send Y axis
                     	LSB = face_i.y & 0xff;
                     	MSB = (face_i.y >> 8) & 0xff;
                     	writedata = arduino_com->WriteData(buffer, MSB );

                     	 cout << writedata << endl;
                     	writedata = arduino_com->WriteData(buffer, LSB );

                     	 cout << writedata << endl;

        }
        // Show the result:
        imshow("face_recognizer", original);
        // And display it:
        char key = (char) waitKey(20);
        // Exit this loop on escape:
        if(key == 27)
            break;
    }
    return 0;
}

