#include <cstdio>
#include <iostream>
#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/cudafilters.hpp"
#include "opencv2/cudafeatures2d.hpp"

using namespace cv;
using namespace std;

RNG rng(12345);

void saveiLowH(int iLowH){
	FILE *f;
	f = fopen("iLowH.txt", "w");
	fprintf(f, "%d", iLowH);
	fclose(f);
	}

void saveiHighH(int iHighH){
	FILE *f;
	f = fopen("iHighH.txt", "w");
	fprintf(f, "%d", iHighH);
	fclose(f);
	}

void saveiLowS(int iLowS){
	FILE *f;
	f = fopen("iLowS.txt", "w");
	fprintf(f, "%d", iLowS);
	fclose(f);
	}

void saveiHighS(int iHighS){
	FILE *f;
	f = fopen("iHighS.txt", "w");
	fprintf(f, "%d", iHighS);
	fclose(f);
	}

void saveiLowV(int iLowV){
	FILE *f;
	f = fopen("iLowV.txt", "w");
	fprintf(f, "%d", iLowV);
	fclose(f);
	}

void saveiHighV(int iHighV){
	FILE *f;
	f = fopen("iHighV.txt", "w");
	fprintf(f, "%d", iHighV);
	fclose(f);
	}


int main(int argc, char** argv) {
	double Area;
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	if (!cap.isOpened()) {
		std::cout << "Câmera desconectada" << std::endl;
		return -1;
	}
	FILE *f;
	f = fopen("iLowH.txt", "r");
	int iLowH;
	fscanf(f, "%d", &iLowH);
	fclose(f);
	f = fopen("iHighH.txt", "r");
	int iHighH;
	fscanf(f, "%d", &iHighH);
	fclose(f);
	f = fopen("iLowS.txt", "r");
	int iLowS;
	fscanf(f, "%d", &iLowS);
	fclose(f);
	f = fopen("iHighS.txt", "r");
	int iHighS;
	fscanf(f, "%d", &iHighS);
	fclose(f);
	f = fopen("iLowV.txt", "r");
	int iLowV;
	fscanf(f, "%d", &iLowV);
	fclose(f);
	f = fopen("iHighV.txt", "r");
	int iHighV;
	fscanf(f, "%d", &iHighV);
	fclose(f);

	//namedWindow("Drawing", CV_WINDOW_AUTOSIZE);
	namedWindow("Control", CV_WINDOW_AUTOSIZE);
	//namedWindow("Tresh", CV_WINDOW_AUTOSIZE);
	namedWindow("Biggest",CV_WINDOW_AUTOSIZE);

    cvCreateTrackbar("LowH", "Control", &iLowH, 179, saveiLowH); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179, saveiHighH);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255, saveiLowS); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255, saveiHighS);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255, saveiLowV); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255, saveiHighV);

	for (;;) {
		Mat frame;
        vector<KeyPoint> keypoints_scene;
        cuda::GpuMat frame_gpu, frame_hsv_gpu, descriptors_scene;

		cap >> frame;
        frame_gpu.upload(frame);
		cuda::cvtColor(frame_gpu, frame_hsv_gpu, CV_BGR2HSV, 4);

		Mat img_matches, frame_hsv;

		Mat imgSave;

		frame_hsv_gpu.download(frame_hsv);

        inRange(frame_hsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgSave);
        //imshow("Tresh", imgSave);
		frame_hsv_gpu.upload(imgSave);
		// Mat cannyOutput;
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		cuda::createMorphologyFilter(MORPH_ERODE, CV_8UC4, imgSave);
		cuda::createMorphologyFilter(MORPH_DILATE, CV_8UC4, imgSave);
		cuda::createMorphologyFilter(MORPH_DILATE, CV_8UC4, imgSave);
		cuda::createMorphologyFilter(MORPH_ERODE, CV_8UC4, imgSave);
        /*
        erode(imgSave, imgSave, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
        dilate( imgSave, imgSave, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
        dilate(imgSave, imgSave, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
        erode(imgSave, imgSave, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
		*/
		//Mat dst;
		frame_hsv_gpu.download(imgSave);
		findContours(imgSave, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));


		/*
		Mat drawing = Mat::zeros(imgSave.size(), CV_8UC3);
		for(int i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		}*/

		Mat biggest = Mat::zeros(imgSave.size(), CV_8UC3);;
		int j = 0;

		Area = 0;

			int q;
		for(int i = 0; i < contours.size();i++)
		{
			double p = contourArea(contours[i]);
			if(Area < p)
			{
				Area = p;
				j = i;
			}
		}
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(biggest, contours, j, color, 2, 8, hierarchy, 0, Point());

		imshow("Control", frame_hsv);
		//imshow("Drawing", drawing);
		imshow("Biggest", biggest);

		// if (good_matches.size() >= 4) {


		waitKey(1);
	}
}
