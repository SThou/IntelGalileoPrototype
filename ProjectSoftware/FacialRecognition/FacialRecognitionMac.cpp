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
 *
 *  Enhanced by Harish Kothandapani and Sovisit Thou
 *  This file is implemented based on MacOS pathway
 *  Revision Date: 11/17/2014 7:06 PM
 */

#define cascade_path "/opt/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml"
#define csv_path "/Users/ST/Documents/XcodeWorkspace/OpenCV/data_csv.csv"
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

int main() {
    
    // Get the path to CSV file:
    string fn_haar = string(cascade_path);
    string fn_csv = string(csv_path);
    int deviceId = atoi(device_id);
    
    
    // These vectors hold the images and corresponding labels:
    vector<Mat> images;
    vector<int> labels;
    vector<Mat> equimages;
    // Read in the data (fails if no valid input filename is given, but you'll get an error message):
    
    try {
        read_csv(fn_csv, images, labels);
        
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    
    equimages.resize(images.size());
    // perform a histogram
    for(int i = 0; i < images.size(); i++) {
        
        equalizeHist(images[i], equimages[i]);
    }
    
    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size AND we need to reshape incoming faces to this size:
    int im_width = equimages[0].cols;
    int im_height = equimages[0].rows;
    
    // Create a FaceRecognizer and train it on the given images:
    Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
    
    model->train(equimages, labels);
    
    
    
    
    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
    
    // Get a handle to the Video device:
    VideoCapture cap(deviceId);
    
    // Check if we can use this device at all:
    if(!cap.isOpened()) {
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
    // Holds the current frame from the Video device:
    Mat frame;
    
    for(;;) {
        cap >> frame;
        
        // Clone the current frame:
        Mat original = frame.clone();
        
        // Convert the current frame to grayscale:Ω
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        
        
        
        Size minFeatureSize(20,20);
        float searchScaleFactor = 1.1f;
        
        // Find the faces in the frame:
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces, searchScaleFactor, 4, CASCADE_SCALE_IMAGE, minFeatureSize);
        
        // At this point you have the position of the faces in
        // faces. Now we'll get the faces, make a prediction and
        // annotate it in the video.
        
        for(int i = 0; i < faces.size(); i++) {
            
            // Process face by face:
            Rect face_i = faces[i];
            
            // Crop the face from the image.
            Mat face = gray(face_i);
            
            // Resizing the face is necessary for Eigenfaces and Fisherfaces. You can easily
            // verify this, by reading through the face recognition tutorial coming with OpenCV.
            // Resizing IS NOT NEEDED for Local Binary Patterns Histograms, so preparing the
            // input data really depends on the algorithm used.
            //
            // I strongly encourage you to play around with the algorithms. See which work best
            // in your scenario, LBPH should always be a contender for robust face recognition.
            //
            // Since I am showing the Fisherfaces algorithm here, I also show how to resize the
            // face you have just found:
            
            Mat face_resized;
            
            cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
            
            // Now perform the prediction
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
            // And now put it into the image:
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
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
