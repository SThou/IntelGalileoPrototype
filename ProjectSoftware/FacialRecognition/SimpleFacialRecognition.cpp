//
//  SimpleFacialRecognition.cpp
//  OpenCV
//
//  Created by ST on 11/17/14.
//  Copyright (c) 2014 ST. All rights reserved.
//  Based on Tutorial by Philipp Wagner
//  Author: Sovisit Thou & Harish Kothandapani
//  Last Edit: November 18. 2014 8:04 PM


#define faceCascade_path "/opt/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml"
#define csv_path "/Users/ST/Documents/XcodeWorkspace/OpenCV/Picture_Source/data_csv.csv"
#define device_id "1" // 1 for second available camera 0 for default camera

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;


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



int main(){
    
    // The path to CSV file:
    string faceCascade = string(faceCascade_path);
    string file_csv = string(csv_path);
    int deviceId = atoi(device_id);
    
    // Open the video stream
    VideoCapture capture(deviceId);
    if(!capture.isOpened()) {
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
    
    Mat camStream; // holds frame stream from video device
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    
    // 1. Face Detection
    
    vector<Mat> faceImages; // data of faces to be recognized
    vector<int> faceLables; // data of corresponding labels to faces
    vector<Mat> histEquifaceImages; // histogram equalization of images
    
    
    // use CascadeClassifier class as detector
    CascadeClassifier faceDetector; // use for detecting face
    try{faceDetector.load(faceCascade); }
    catch (cv::Exception e) {}
    if ( faceDetector.empty()){
        cerr << "Error: Could not load Face Detector" <<endl;
        exit(1);
    }
    
    // Load pathways to image to load faces
    try { read_csv(file_csv, faceImages, faceLables);}
    catch (cv::Exception e){
        cerr<<"Error: Could not load CSV file for image path" <<endl;
    }
    
        
        
    // 2. Face preprocessing
    // apply histogram equilization
    histEquifaceImages.resize(faceImages.size()); // size unusued vector to size of faceimages read
    
    for(int i = 0; i < faceImages.size(); i++) {
        equalizeHist(faceImages[i], histEquifaceImages[i]);
        }
        
        
    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size AND we need to reshape incoming faces to this size:
    int im_width = faceImages[0].cols;
    int im_height = faceImages[0].rows;
        
    // 3. Train and Learn faces
    // Create a FaceRecognizer and train it on the given images:
    Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    model->train(histEquifaceImages, faceLables);
        
    
    // 4. Face Recognition
    char key = 'a';
    while(key != 27){
        
        capture >> camStream;
        
        
        // clone the incoming stream frame to use later
        Mat originalFrameStream = camStream.clone();
        
        // Convert the incoming stream to grey for better processing
        Mat greyConvertStream;
        cvtColor(originalFrameStream, greyConvertStream, CV_BGR2GRAY);
        
        Size minFeaturesSize(44,44); // the higher the number, the faster the stream but less match                    // 24x24 is for native mac
        
                                    // the lower the number, the slower the stream but more match
        float searchScaleFactor = 1.1f;
        
        // Find the faces in the stream:
        vector<Rect_<int> > facesInStream;
        faceDetector.detectMultiScale(greyConvertStream, facesInStream, searchScaleFactor, 4, CASCADE_SCALE_IMAGE, minFeaturesSize);
        
        // Get the faces within the stream and make a prediction of whose who.
        
        for(int i = 0; i < facesInStream.size(); i++){
            
            // process the faces in stream
            Rect face_i = facesInStream[i];
            
            // crop the face from the stream
            Mat face = greyConvertStream(face_i);
            
            // resize the face, necessary for processing Eigenfaces and Fisherfaces
            Mat face_resize;
            resize(face, face_resize, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC); // 8x8 pixel neighborhood
            
            // Perform the prediction
            int prediction = model->predict(face_resize);
            
            // Reveal the results
            
            // Draw a green rectangle for detected face
            rectangle(greyConvertStream, face_i, CV_RGB(0,0,255), 2, 4); // color, thickness, lines
            
            // Text for annotating with box
            string box_text = format("Person: %d", prediction);
            
            // Position for Text
            int pos_x = max(face_i.tl().x - 10, 0);
            int pos_y = max(face_i.tl().y - 10, 0);
            
            // put text into image
            putText(greyConvertStream, box_text, Point(pos_x, pos_y), FONT_HERSHEY_COMPLEX, 1.0, CV_RGB(0,255,0), 2.0);
            
        }  // end for loop
        
            // show the result:
        imshow("Face_Recognixer", greyConvertStream);
        
        
        } // end while loop
        
        key = waitKey(1); // hold the program
        
        
    } // end main
