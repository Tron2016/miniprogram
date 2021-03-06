#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
void ColorPCA(cv::Mat &color)
{
	using namespace cv;
	std::vector<cv::Mat> channels;
	Mat color2;
	color.convertTo(color2, CV_32FC1);
	cv::split(color2, channels);
	int sum = color.rows*color.cols;
	Mat b = channels[0].reshape(0, sum);
	Mat g = channels[1].reshape(0, sum);
	Mat r = channels[2].reshape(0, sum);
	Mat diff(color.rows*color.cols, 3, CV_32FC1);
	Scalar sc1 = cv::mean(b);
	Scalar sc2 = cv::mean(g);
	Scalar sc3 = cv::mean(r);
	Mat db = b -Mat(sum,1, CV_32FC1, sc1);
	Mat dg = g - Mat(sum, 1, CV_32FC1, sc2);
	Mat dr = r - Mat(sum, 1, CV_32FC1, sc2);
	Mat diffb = diff.colRange(0, 1);
	Mat diffg = diff.colRange(1, 2);
	Mat diffr = diff.colRange(2, 3);

	db.copyTo(diffb);
	dg.copyTo(diffg);
	dr.copyTo(diffr);
	Mat eigenValues;
	Mat eigenVectors;
	Mat diffT;
	cv::transpose(diff, diffT);
	Mat cov(diffT*diff/sum);
	eigen(cov, eigenValues, eigenVectors);
	Mat dv;
	cv::sqrt(eigenValues, dv);
	int Rows = color.rows;
	int Cols = color.cols*color.channels();

	cv::Mat noise(3, 1, CV_32FC1);
	cv::randn(noise, 0, 0.01);
	Mat noiseD = cv::Mat::diag(noise);
	Mat eigenVectorsT;
	cv::transpose(eigenVectors, eigenVectorsT);
	Mat val = eigenVectorsT*(noiseD*dv);


	std::vector<cv::Mat> channels2;
	cv::split(color, channels2);
	channels2[0] = channels2[0] + val.at<Vec3f>(0, 0);
	channels2[1] = channels2[1] + val.at<Vec3f>(1, 0);
	channels2[2] = channels2[2] + val.at<Vec3f>(2, 0);
	cv::merge(channels2, color);

}

void main()
{
	using namespace cv;
	Mat img0 = imread("2.jpg");
	Mat img;
	resize(img0,img,Size(200, 200));
	ColorPCA(img);
	imshow("2",img);
	moveWindow("2", 400, 400);
	waitKey(200000);
}