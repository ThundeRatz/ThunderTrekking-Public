#include "camera.hh"
#include "ThreadMotors.hh"

#define MAXXCOOR 320
#define MAXYCOOR 240

RNG rng(12345);

void Camera::saveiLowH(int iLowH){
	FILE *f;
	f = fopen("iLowH.txt", "w");
	fprintf(f, "%d", iLowH);
	fclose(f);
	}

void Camera::saveiHighH(int iHighH){
	FILE *f;
	f = fopen("iHighH.txt", "w");
	fprintf(f, "%d", iHighH);
	fclose(f);
	}

void Camera::saveiLowS(int iLowS){
	FILE *f;
	f = fopen("iLowS.txt", "w");
	fprintf(f, "%d", iLowS);
	fclose(f);
	}

void Camera::saveiHighS(int iHighS){
	FILE *f;
	f = fopen("iHighS.txt", "w");
	fprintf(f, "%d", iHighS);
	fclose(f);
	}

void Camera::saveiLowV(int iLowV){
	FILE *f;
	f = fopen("iLowV.txt", "w");
	fprintf(f, "%d", iLowV);
	fclose(f);
	}

void Camera::saveiHighV(int iHighV){
	FILE *f;
	f = fopen("iHighV.txt", "w");
	fprintf(f, "%d", iHighV);
	fclose(f);
	}


int Camera::getBiggest() {
		int j = 0;

		double Area = 0;

		for(int i = 0; i < contours.size();i++)
		{
			double p = contourArea(contours[i]);
			if(Area < p)
			{
				Area = p;
				j = i;
			}
		}
		return j;

}

int Camera::getXContour(int j) {
	if(contours.size() == 0 || (contours[j]).size() == 0)
		return -1;
	int i, sum = 0;
	for(i = 0;i < (contours[j]).size();i++) {
		sum += (contours[j][i]).x;
	}
	return sum/(contours[j]).size();
}

#define VELOCIDADE_MAX 50

void Camera::followContour(int j) {
	int i = getXContour(j);
	if(i != -1) {
		int vel1 = -VELOCIDADE_MAX + 4*VELOCIDADE_MAX*((double)i/MAXXCOOR);
		int vel2 = 3*VELOCIDADE_MAX - 4*VELOCIDADE_MAX*((double)i/MAXXCOOR);
		if(vel1 > VELOCIDADE_MAX) vel1 = VELOCIDADE_MAX;
		else if (vel1 < -VELOCIDADE_MAX) vel1 = -VELOCIDADE_MAX;
		if(vel2 > VELOCIDADE_MAX) vel2 = VELOCIDADE_MAX;
		else if(vel2 < -VELOCIDADE_MAX) vel2 = -VELOCIDADE_MAX;
		motor(vel1, vel2);
	}
	else motor(0,0);
}

Camera::Camera(){
	cap = VideoCapture(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, MAXXCOOR);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, MAXYCOOR);
	if (!cap.isOpened()) {
		std::cout << "Câmera desconectada" << std::endl;
	}
	FILE *f;
	//Se der segfault, copiar os seguintes arquivos para a pasta de execução:
	if((f = fopen("iLowH.txt", "r"))) {
		fscanf(f, "%d", &iLowH);
		fclose(f);
	} else iLowH = 0;

	if((f = fopen("iHighH.txt", "r"))) {
		fscanf(f, "%d", &iHighH);
		fclose(f);
	} else iHighH = 0;
	if((f = fopen("iLowS.txt", "r"))) {
		fscanf(f, "%d", &iLowS);
		fclose(f);
	} else iLowS = 0;
	if((f = fopen("iHighS.txt", "r"))) {
		fscanf(f, "%d", &iHighS);
		fclose(f);
	} else iHighS = 0;
	if((f = fopen("iLowV.txt", "r"))) {
		fscanf(f, "%d", &iLowV);
		fclose(f);
	} else iLowV = 0;
	if((f = fopen("iHighV.txt", "r"))) {
		fscanf(f, "%d", &iHighV);
		fclose(f);
	} else iHighV = 0;
#ifdef DEBUG
	namedWindow("Drawing", CV_WINDOW_AUTOSIZE);
	namedWindow("Control", CV_WINDOW_AUTOSIZE);
	namedWindow("Tresh", CV_WINDOW_AUTOSIZE);
	namedWindow("Biggest",CV_WINDOW_AUTOSIZE);

	cvCreateTrackbar("LowH", "Control", &iLowH, 179, saveiLowH); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179, saveiHighH);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255, saveiLowS); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255, saveiHighS);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255, saveiLowV); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255, saveiHighV);
#endif
}

void Camera::showBiggest() {
		cuda::GpuMat frame_gpu, frame_hsv_gpu, descriptors_scene;
		cap >> frame;
		frame_gpu.upload(frame);
		cuda::cvtColor(frame_gpu, frame_hsv_gpu, CV_BGR2HSV, 4);
		frame_hsv_gpu.download(frame_hsv);
		inRange(frame_hsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgSave);
#ifdef DEBUG
		imshow("Tresh", imgSave);
#endif
		frame_hsv_gpu.upload(imgSave);
		// Mat cannyOutput;
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



		Mat drawing = Mat::zeros(imgSave.size(), CV_8UC3);
		for(int i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		}
#ifdef DEBUG
		imshow("Drawing", drawing);
		imshow("Control", frame_hsv);
#endif
		// if (good_matches.size() >= 4) {
		color = Scalar( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		Mat biggest = Mat::zeros(imgSave.size(), CV_8UC3);
		drawContours(biggest, contours, getBiggest(), color, 2, 8, hierarchy, 0, Point());
#ifdef DEBUG
		imshow("Biggest", biggest);
#endif

		waitKey(1);
}
