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
#include <vector>


using namespace cv;
using namespace std;

/**
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )
{

  Mat img_1 = imread("Y:\\workspace\\OpenCV\\Debug\\Haris.png", CV_LOAD_IMAGE_GRAYSCALE);
  Mat img_2 = imread("Y:\\workspace\\OpenCV\\Debug\\HarishSov_5.png", CV_LOAD_IMAGE_GRAYSCALE);


  if( !img_1.data || !img_2.data )
  { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 800;

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );

  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;

  Mat descriptors_1, descriptors_2;

  extractor.compute( img_1, keypoints_1, descriptors_1 );
  extractor.compute( img_2, keypoints_2, descriptors_2 );

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_1, descriptors_2, matches );

  double max_dist = 0; double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_1.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );

  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
  //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
  //-- small)
  //-- PS.- radiusMatch can also be used here.
  std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors_1.rows; i++ )
  { if( matches[i].distance <= max(2*min_dist, 0.02) )
    { good_matches.push_back( matches[i]); }
  }

  //-- Draw only "good" matches
  Mat img_matches;
  drawMatches( img_1, keypoints_1, img_2, keypoints_2,
               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );


  // Addition for the boxes to track
  // -- localize the object
  vector <Point2f> obj;
  vector <Point2f> scene;

  for (int i = 0; i < good_matches.size(); i++)
  {
	  // -- get the keypoints from the good matches
	  obj.push_back( keypoints_1[good_matches[i].queryIdx].pt);
	  scene.push_back( keypoints_2[good_matches[i].trainIdx].pt);

  }

  Mat H = findHomography( obj, scene, CV_RANSAC);

  // -- get the corners from the image_1 (the object to be "detected")

  vector<Point2f> obj_corners(4);
  obj_corners[0] = cvPoint(0,0);
  obj_corners[1] = cvPoint(img_1.cols, 0);
  obj_corners[2] = cvPoint(img_1.cols, img_1.rows);
  obj_corners[3] = cvPoint(0, img_1.rows);
  vector<Point2f> det_corners(4);

  perspectiveTransform( obj_corners, det_corners, H);


  //-- Draw the lines between the corners ( the mapped object - image 2)
  line( img_matches, det_corners[0] + Point2f(img_1.cols, 0), det_corners[1] + Point2f(img_1.cols, 0 ), Scalar(0, 255, 0 ), 4);
  line( img_matches, det_corners[1] + Point2f(img_1.cols, 0), det_corners[2] + Point2f(img_1.cols, 0 ), Scalar(0, 255, 0 ), 4);
  line( img_matches, det_corners[2] + Point2f(img_1.cols, 0), det_corners[3] + Point2f(img_1.cols, 0 ), Scalar(0, 255, 0 ), 4);
  line( img_matches, det_corners[3] + Point2f(img_1.cols, 0), det_corners[0] + Point2f(img_1.cols, 0 ), Scalar(0, 255, 0 ), 4);


  //-- Show detected matches
  imshow( "Good Matches", img_matches );
/*
  // Original

  for( int i = 0; i < (int)good_matches.size(); i++ )
  { printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); }
*/
  waitKey(0);
  cout<< "end"<<endl;


  double test = (min_dist/max_dist);
  cout<< test << endl;

  if ((min_dist)<(0.80*min_dist)){

	  cout<<"This person is a match"<<endl;

  }

  return 0;
}



/*
using namespace std;
using namespace cv;

void readme();

int main(int argc, char** argv)

{

	if (argc != 3)
	{readme(); return -1;}




	initModule_nonfree();

	// Read input images

	Mat image1= imread("Y:\\workspace\\OpenCV\\Debug\\Star4.jpg");
	Mat image2= imread("Y:\\workspace\\OpenCV\\Debug\\Star5.jpg");


	if (!image1.data || !image2.data)
		printf("could not open");
		//return 0;



    // Display the images

	//namedWindow("Right Image");

	//imshow("Right Image",image1);

	//namedWindow("Left Image");

	//imshow("Left Image",image2);



	// vector of keypoints

	vector<KeyPoint> keypoints1;

	vector<KeyPoint> keypoints2;



	// Construction of the SURF feature detector

	SurfFeatureDetector surf(3000);



	// Detection of the SURF features

	surf.detect(image1,keypoints1);

	surf.detect(image2,keypoints2);



	cout << "Number of SURF points (1): " << keypoints1.size() << endl;

	cout << "Number of SURF points (2): " << keypoints2.size() << endl;

	if(keypoints1.size() == keypoints2.size())
		printf("The images are a match\n");
	else printf("the images are not a match\n");

	// Draw the kepoints

	Mat imageKP;

	drawKeypoints(image1,keypoints1,imageKP,Scalar(255,255,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	namedWindow("Right SURF Features");

	imshow("Right SURF Features",imageKP);

	drawKeypoints(image2,keypoints2,imageKP,Scalar(255,255,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	namedWindow("Left SURF Features");

	imshow("Left SURF Features",imageKP);



	// Construction of the SURF descriptor extractor

	SurfDescriptorExtractor surfDesc;



	// Extraction of the SURF descriptors

	Mat descriptors1, descriptors2;

	surfDesc.compute(image1,keypoints1,descriptors1);

	surfDesc.compute(image2,keypoints2,descriptors2);



	cout << "descriptor matrix size: " << descriptors1.rows << " by " << descriptors1.cols << endl;



	// Construction of the matcher

	//BruteForceMatcher<L2<float>> matcher; // deprecated
	BFMatcher matcher(NORM_L2);

	// Match the two image descriptors

	vector<cv::DMatch> matches;

	matcher.match(descriptors1,descriptors2, matches);



	cout << "Number of matched points: " << matches.size() << endl;




	nth_element(matches.begin(),    // initial position

		             matches.begin()+24, // position of the sorted element

					 matches.end());     // end position

	// remove all elements after the 25th

	matches.erase(matches.begin()+25, matches.end());



	Mat imageMatches;

	drawMatches(image1,keypoints1,  // 1st image and its keypoints

		            image2,keypoints2,  // 2nd image and its keypoints

					matches,			// the matches

					imageMatches,		// the image produced

					Scalar(0,0,0)); // color of the lines

	namedWindow("Matches");

	imshow("Matches",imageMatches);



	waitKey();

	return 0;



	int size=7;

	Mat imaf1;

	image1.convertTo(imaf1,CV_32F);



	Mat imaf2;

	image2.convertTo(imaf2,CV_32F);



	waitKey();




	return 0;

}

void readme(){
	cout << " Usage: ./SURF_FlannMatcher <img1> <img2>" << std::endl; }
}
*/
