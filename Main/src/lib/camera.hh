#pragma once

#include <cstdio>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/cudafilters.hpp"
#include "opencv2/cudafeatures2d.hpp"

using namespace cv;
using namespace std;
			
class Camera{
	public:
		Camera();
		void showBiggest();
		int getBiggest();
		//Pega as coordenadas do contour do índice j
		int getXContour(int j);
		//Siga o contorno de índice j
		void followContour(int j);
	private:
		static void saveiLowH(int iLowH);
		static void saveiHighH(int iHighH);
		static void saveiLowS(int iLowS);
		static void saveiHighS(int iHighS);
		static void saveiLowV(int iLowV);
		static void saveiHighV(int iHighV);
		VideoCapture cap;
		Mat frame;
		vector<KeyPoint> keypoints_scene;
		Mat img_matches, frame_hsv;
		Mat imgSave;
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		Scalar color;
		int iLowH;
		int iHighH;
		int iLowS;
		int iHighS;
		int iLowV;
		int iHighV;
};
