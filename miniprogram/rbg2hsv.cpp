#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
class vec3 {
public:
	vec3(float x, float y, float z): r(x),g(y),b(z){};
	float r;
	float g;
	float b;
};
vec3 RGBtoHSV(vec3 color)
{
	using namespace std;
	float cmin = min(color.r, min(color.g, color.b));
	float cmax = max(color.r, max(color.g, color.b));
	float h = 0.0;
	float s = 0.0;
	float v = cmax;
	float diff = cmax - cmin;
	if (cmax != 0.0)
		s = diff / cmax;
	else
		return vec3(0.0,0.0,0.0);
	if (color.r == cmax)
		h = (color.g - color.b) / diff;
	else if (color.g == cmax)
		h = 2.0 + (color.b - color.r) / diff;
	else
		h = 4.0 + (color.r - color.g) / diff;
	h = h*60.0;
	if (h<0.0)
		h = h + 360.0;
	h = h / 360.0;
	return vec3(h, s, v);

}
vec3 HSVtoRGB(vec3 color) {
	float h = color.r;
	float s = color.g;
	float v = color.b;
	float r, g, b, f, p, q, t;
	if (s == 0.0)
	{
		r = g = b = v;
		return vec3(r, g, b);
	}
	float temp = h*6.0;
	float n = floor(temp);

	f = temp - n;
	p = v*(1.0 - s);
	q = v*(1.0 - f*s);
	t = v*(1.0 - (1.0 - f)*s);

	switch (int(n))
	{
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default:
		r = v;
		g = p;
		b = q;
		break;
	}
	return vec3(r, g, b);

}
void main()
{
	using namespace cv;
	Mat img0 = imread("2.jpg");
	Mat img;
	resize(img0,img,Size(200, 200));
	
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			vec3 a(img.at<cv::Vec3b>(i, j)[2]/255.0, img.at<cv::Vec3b>(i, j)[1] / 255.0, img.at<cv::Vec3b>(i, j)[0] / 255.0);
			vec3 b = RGBtoHSV(a);
			vec3 c = HSVtoRGB(b);
			img.at<cv::Vec3b>(i, j)[2] = c.r * 255;
			img.at<cv::Vec3b>(i, j)[1] = c.g * 255;
			img.at<cv::Vec3b>(i, j)[0] = c.b * 255;
		}
	}
	Mat img2;
	cvtColor(img, img2, CV_HSV2BGR);
	imshow("l",img);
	moveWindow("l", 400, 400);
	waitKey(200000);
}