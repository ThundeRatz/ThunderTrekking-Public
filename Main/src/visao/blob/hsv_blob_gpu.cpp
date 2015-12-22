#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/gpu/gpu.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cout << "Cannot open the web cam\n";
        return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE);

    int iLowH = 0;
    int iHighH = 179;

    int iLowS = 0; 
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    gpu::GpuMat src, resized, imgHSV_GPU;
    for (;;) {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

        if (!bSuccess) {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        src.upload(imgOriginal);
        
        gpu::resize(src, resized, Size(0, 0), 0.5, 0.5);
        gpu::cvtColor(resized, imgHSV_GPU, COLOR_BGR2HSV, 4);

        Mat imgHSV;
        Mat imgThresholded;
        imgHSV_GPU.download(imgHSV);

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(4, 4)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(4, 4)) ); 

        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        imshow("Thresholded Image", imgThresholded);
        imshow("Original", imgOriginal);

        if (waitKey(1) == 27) {
            cout << "ESC\n";
            break; 
        }
    }
    return 0;
}
