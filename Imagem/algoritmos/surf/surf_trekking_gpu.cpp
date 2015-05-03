#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/nonfree/gpu.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#define MATCHER		_FLANN

using namespace cv;
int main(int argc, char** argv) {
	Mat cone_img;
	int camera_id;
	
	if (argc < 2) {
		std::cout << "Uso: surf_trekking câmera imagem" << std::endl;
		return -1;
	}
	
	sscanf(argv[1], "%d", &camera_id);
	VideoCapture cap(camera_id);
	
	if (!cap.isOpened()) {
		std::cout << "Câmera desconectada" << std::endl;
		return -1;
	}
	
	cone_img = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	if (!cone_img.data) {
		std::cout << "Erro na leitura da imagem" << std::endl;
		return -1;
	}
	
	std::vector<KeyPoint> keypoints_cone;
	
	gpu::SURF_GPU detect_and_compute(400);
	
	gpu::GpuMat cone_gpu(cone_img), descriptors_cone;;
	//detect_and_compute.detect(cone_img, keypoints_cone);
	//detect_and_compute.compute(cone_img, keypoints_cone, descriptors_cone);
	detect_and_compute(cone_gpu, gpu::GpuMat(), keypoints_cone, descriptors_cone);
	
	std::cout << "Imagem processada" << std::endl;
	for (;;) {
		Mat frame, frame_gray;
		std::vector<KeyPoint> keypoints_scene;
		
		cap >> frame;
		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		
		//detect_and_compute.detect(frame_gray, keypoints_scene);
		//detect_and_compute.compute(frame_gray, keypoints_scene, descriptors_scene);
		gpu::GpuMat frame_gpu(cone_gpu), descriptors_scene;
		detect_and_compute(frame_gpu, gpu::GpuMat(), keypoints_scene, descriptors_scene);
		
#if MATCHER == _BRUTEFORCE
		BFMatcher matcher(NORM_L2);
#elif MATCHER == _FLANN
		FlannBasedMatcher matcher;
#endif
		std::vector<DMatch>matches;
		matcher.match(Mat(descriptors_cone), Mat(descriptors_scene), matches);
		
		/*Mat img_matches;
		drawMatches(cone_img, keypoints_cone, frame_gray, keypoints_scene, matches, img_matches);
		std::cout << "Frame" << std::endl;
		imshow("surf_Matches", frame);*/
		
		double max_dist = 0; double min_dist = 100;
		//-- Quick calculation of max and min distances between keypoints
		for(int i = 0; i < descriptors_cone.rows; i++) {
			double dist = matches[i].distance;
			if( dist < min_dist ) min_dist = dist;
			if( dist > max_dist ) max_dist = dist;
		}
		
		printf("-- Max dist : %f \n", max_dist );
		printf("-- Min dist : %f \n", min_dist );
		
		//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
		std::vector< DMatch > good_matches;

		for (int i = 0; i < descriptors_cone.rows; i++) {
			if (matches[i].distance < 3*min_dist) {
				good_matches.push_back( matches[i]);
			}
		}
		
		Mat img_matches;
		drawMatches(cone_img, keypoints_cone, frame_gray, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		//-- Localize the object
		std::vector<Point2f> obj;
		std::vector<Point2f> scene;

		if (good_matches.size() >= 4) {
			for (int i = 0; i < good_matches.size(); i++) {
				obj.push_back(keypoints_cone[ good_matches[i].queryIdx ].pt);
				scene.push_back(keypoints_scene[ good_matches[i].trainIdx ].pt);
			}
			
			Mat H = findHomography(obj, scene, CV_RANSAC);

			std::vector<Point2f> obj_corners(4);
			obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint(cone_img.cols, 0);
			obj_corners[2] = cvPoint(cone_img.cols, cone_img.rows ); obj_corners[3] = cvPoint(0, cone_img.rows);
			std::vector<Point2f> scene_corners(4);

			perspectiveTransform(obj_corners, scene_corners, H);
			//~ if (/*scene_corners[1].x - scene_corners[0].x > 50 &&
				//~ scene_corners[2].x - scene_corners[3].x > 50 &&
				//~ scene_corners[3].y - scene_corners[0].y > 50 &&
				//~ scene_corners[2].y - scene_corners[1].y > 50 &&*/
				
				//~ scene_corners[1].y - scene_corners[0].y > -50 &&
				//~ scene_corners[1].y - scene_corners[0].y < 50 &&
				//~ scene_corners[2].y - scene_corners[3].y > -50 &&
				//~ scene_corners[2].y - scene_corners[3].y < 50 &&
				
				//~ scene_corners[0].x - scene_corners[3].x > -50 &&
				//~ scene_corners[0].x - scene_corners[3].x < 50 &&
				//~ scene_corners[1].x - scene_corners[2].x > -50 &&
				//~ scene_corners[1].x - scene_corners[2].x < 50 &&
				
				//~ (scene_corners[2].x - scene_corners[0].x) / (scene_corners[2].y - scene_corners[0].y) -
				//~ cone_img.cols / cone_img.rows > 0.6 &&
				//~ (scene_corners[2].x - scene_corners[0].x) / (scene_corners[2].y - scene_corners[0].y) -
				//~ cone_img.cols / cone_img.rows < 1.6) {
				line(img_matches, scene_corners[0] + Point2f(cone_img.cols, 0), scene_corners[1] + Point2f(cone_img.cols, 0), Scalar(0, 255, 0), 4);
				line(img_matches, scene_corners[1] + Point2f(cone_img.cols, 0), scene_corners[2] + Point2f(cone_img.cols, 0), Scalar(0, 255, 0), 4);
				line(img_matches, scene_corners[2] + Point2f(cone_img.cols, 0), scene_corners[3] + Point2f(cone_img.cols, 0), Scalar(0, 255, 0), 4);
				line(img_matches, scene_corners[3] + Point2f(cone_img.cols, 0), scene_corners[0] + Point2f(cone_img.cols, 0), Scalar(0, 255, 0), 4);
				
				std::cout << (scene_corners[0].x + scene_corners[1].x) / 2 - frame.cols / 2 << std::endl;
			//~ }
		}
		imshow("Good Matches & Object detection", img_matches);
		
		waitKey(5);
	}
}

