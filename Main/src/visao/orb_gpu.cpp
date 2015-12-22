#include <cstdio>
#include <iostream>
using namespace std;
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/gpu/gpu.hpp"

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
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
 
	if (!cap.isOpened()) {
		std::cout << "Câmera desconectada" << std::endl;
		return -1;
	}
	
	cone_img = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	if (!cone_img.data) {
		std::cout << "Erro na leitura da imagem" << std::endl;
		return -1;
	}


	std::cout << "GPU upload...\n";
	gpu::GpuMat descriptors_cone;
	gpu::GpuMat cone_gpu(cone_img);
	gpu::ORB_GPU detect_and_compute;
    vector<KeyPoint> keypoints_cone;
#warning mudar aqui
	gpu::GpuMat batata;
	detect_and_compute(cone_gpu, batata, keypoints_cone, descriptors_cone);
	
	std::cout << "Imagem processada" << std::endl;
	for (;;) {
        gpu::ORB_GPU scene_orb;
		Mat frame;
        vector<KeyPoint> keypoints_scene;
        gpu::GpuMat frame_gpu, frame_gray_gpu, descriptors_scene;
		
		cap >> frame;
        frame_gpu.upload(frame);
        //gpu::resize(src, resized, Size(0, 0), 0.5, 0.5);
		gpu::cvtColor(frame_gpu, frame_gray_gpu, CV_BGR2GRAY);
		scene_orb(frame_gray_gpu, gpu::GpuMat(), keypoints_scene, descriptors_scene);


        gpu::BruteForceMatcher_GPU<Hamming> matcher;
		std::vector<DMatch> matches;
		matcher.match(descriptors_cone, descriptors_scene, matches);
		
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
		
		Mat img_matches, frame_gray;
		frame_gray_gpu.download(frame_gray);
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
		
		waitKey(1);
	}
}
