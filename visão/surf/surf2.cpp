Mat img_1, img_2;
Mat img_1c = imread ("corner0001.JPG");
Mat img_2c = imread ("corner0002.JPG");
cvtColor (img_1c,img_1,CV_BGR2GRAY);
cvtColor (img_2c,img_2,CV_BGR2GRAY);

vector<KeyPoint> keypoints_1;
vector<KeyPoint> keypoints_2;
SurfFeatureDetector surf(2.50e3);
surf(img_1, Mat(), keypoints_1);
surf(img_2, Mat(), keypoints_2);

SurfDescriptorExtractor extractor;
cv::Mat descriptors_1,descriptors_2;
//compute descriptors
extractor.compute(img_1,keypoints_1,descriptors_1);
extractor.compute(img_2,keypoints_2,descriptors_2);

//use burte force method to match vectors
BruteForceMatcher<L2<float> >matcher;
vector<DMatch>matches;
matcher.match(descriptors_1,descriptors_2,matches);

//draw results
Mat img_matches;
drawMatches(img_1c,keypoints_1,img_2c,keypoints_2,matches,img_matches);
imshow("surf_Matches",img_matches);
