#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;


class Comparation {
private:
	Mat first;
	Mat second;
	Mat roi1;
	Mat roi2;
	int szer;
	int wys;
	int przyrostX;
	int przyrostY;
	int SADmax;
	Point2d firstPoint;
	Point2d secondPoint;
public:
	Comparation(Mat x , Mat y , int dzielSzer=5 , int dzielWys=5 );
	Point2d getSecondPoint();
	Point2d getFirstPoint();
	int SAD2(Mat roi1, Mat roi2);
	int fastSAD(Mat& roi1, Mat& roi2, const uchar*);
	void lokalizuj();
	void przeszukObszar();
	void wysw();
	
};