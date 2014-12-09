
//  main.cpp
//  SerialCommunicationTest
//
//  Created by ST on 11/19/14.
//  Copyright (c) 2014 ST. All rights reserved.
//
//  based on works from soerenbnoergaard
//
// Note: subtracting - '0' from char casts into an int value
//       chars are 1 byte, if considered as short they are 2 bytes
//
/*
#include <iostream>
#include <boost/asio.hpp>



#define Serial_Port "COM9"

using namespace std;
using namespace::boost::asio;

// serial settings
serial_port_base::baud_rate BAUD(9600);
serial_port_base::flow_control  FLOW(serial_port_base::flow_control::none);
serial_port_base::parity PARITY( serial_port_base::parity::none);
serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one);




int main(){

    io_service io;
    serial_port port(io, Serial_Port);

    // setup port
    port.set_option(BAUD);
    port.set_option(FLOW);
    port.set_option(PARITY);
    port.set_option(STOP);


    unsigned char input;
    char c;

    if(port.is_open()){

        while(1){
            cin>> input;

            // Output buffer
            unsigned char command[2] = {0};

            // convert and send
            command[0] = static_cast<unsigned char>(input);
            write(port, buffer(command, 1) );

            // read response
            read(port, buffer(&c, 1)); // read blocks until it has something
            cout<<"Read: "<<c<<endl;

        } // end while loop
    } // end if


} // end main
*/




#define cascade_path "C:/Users/spartans/Desktop/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"
#define csv_path "C:/Users/spartans/Desktop/opencv_backup/data_csv.csv"
#define device_id "0"
#define Serial_Port "COM9"

#include <opencv2\opencv.hpp>
#include <opencv2\contrib\contrib.hpp>

#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;
using namespace::boost::asio;

//Serial Settings
serial_port_base::baud_rate BAUD(9600);
serial_port_base::flow_control  FLOW(serial_port_base::flow_control::none);
serial_port_base::parity PARITY( serial_port_base::parity::none);
serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one);

int trackingLabel = 1;


static void read_csv(const string& filename, vector<Mat>& images,
		vector<int>& labels, char separator = ';')
{
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file)
	{
		string error_message =
				"No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line))
	{
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty())
		{
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

int main(int argc, const char *argv[])
{
	// Get the path to your CSV:
	string fn_haar = string(cascade_path);
	string fn_csv = string(csv_path);
	int deviceId = atoi(device_id);
	// These vectors hold the images and corresponding labels:
	vector<Mat> images;
	vector<int> labels;

	io_service io;
	    serial_port port(io, Serial_Port);

	    // setup port
	    port.set_option(BAUD);
	    port.set_option(FLOW);
	    port.set_option(PARITY);
	    port.set_option(STOP);

	    unsigned char dirX;
	    unsigned char dirX2;

	    int previousface = 0; // previous position
	    int previouscalc = 0;

	    //center points of screen for comparison
	    int xcenter = 200;
	    int ycenter = 200;

	// Read in the data (fails if no valid input filename is given, but you'll get an error message):
	try
	{
		read_csv(fn_csv, images, labels);
	} catch (cv::Exception& e)
	{
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg
				<< endl;
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
	if (!cap.isOpened())
	{
		cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
		return -1;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 400);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 400);
	// Holds the current frame from the Video device:
	Mat frame;
	for (;;)
	{
		cap >> frame;
		// Clone the current frame:
		Mat original = frame.clone();
		// Convert the current frame to grayscale:
		Mat gray;
		cvtColor(original, gray, CV_BGR2GRAY);
		// Find the faces in the frame:
		vector<Rect_<int> > faces;
		//std::vector <Rect> faces;
		haar_cascade.detectMultiScale(gray, faces);
		// At this point you have the position of the faces in
		// faces. Now we'll get the faces, make a prediction and
		// annotate it in the video. Cool or what?
		for (int i = 0; i < faces.size(); i++)
		{
			// Process face by face:
			Rect face_i = faces[i];
			// Crop the face from the image. So simple with OpenCV C++:
			Mat face = gray(face_i);
			// Since I am showing the Fisherfaces algorithm here, I also show how to resize the
			// face you have just found:
			Mat face_resized;
			cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0,
					INTER_CUBIC);
			// Now perform the prediction, see how easy that is:
			int prediction = model->predict(face_resized);
			// And finally write all we've found out to the original image!
			// First of all draw a green rectangle around the detected face:
			rectangle(original, face_i, CV_RGB(0, 255, 0), 1);
			// Create the text we will annotate the box with:
			string box_text = format("Prediction = %d", prediction);
			// Calculate the position for annotated text (make sure we don't
			// put illegal values in there):
			int pos_x = std::max(face_i.tl().x - 10, 0);
			int pos_y = std::max(face_i.tl().y - 10, 0);
			//HK:Center of the current bounding rectangle
			Point center(face_i.x + face_i.width / 2,
					face_i.y + face_i.height / 2);
			// And now put it into the image:
			putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN,
					1.0, CV_RGB(0, 255, 0), 2.0);
			//send prediction
			cout<<"X Position"<<  face_i.x << endl;
			            cout<<"previous"<<previouscalc<<endl;

			            float per = .025;
			              int newface = face_i.x; // incoming new position
			                  previouscalc = (previousface*per) + previousface;
			            if (face_i.x<100){ // go right

			                dirX2 = '2';

			                if (newface<previouscalc)
			                {
			                // Output buffer
			                unsigned char command[1] = {0};
			                    previousface = face_i.x; // assign face to previous and send off
			                    cout<<"Right Run"<<endl;

			                // convert and send
			                command[0] = static_cast<unsigned char>(dirX2);
			                write(port, buffer(command, 1) );

			                 //read response
			             //     read(port, buffer(&c, 1)); // read blocks until it has something
			              //    cout<<"Read: "<<c<<endl;
			            }
			            }

			            if (face_i.x>100){ // go left

			            dirX = '1';

			            if(prediction == trackingLabel){
			                if (newface>previouscalc){
			            // Output buffer
			            unsigned char command[1] = {0};

			                    previousface = face_i.x; // assign face to previous and send off
			                    cout<<"Left Run"<<endl;
			            // convert and send
			            command[0] = static_cast<unsigned char>(dirX);
			            write(port, buffer(command, 1) );

			            // read response
			          //  read(port, buffer(&c, 1)); // read blocks until it has something
			           // cout<<"Read: "<<c<<endl;
			            }
			            }


			            }

			}

		// Show the result:
		imshow("face_recognizer", original);
		// And display it:
		char key = (char) waitKey(20);
		// Exit this loop on escape:
		if (key == 27)
			break;
	}
	return 0;
}


