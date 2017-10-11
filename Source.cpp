

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Comparation.h"
#include <iostream>
#include <cmath>
#include <ctime>

using namespace cv;
using namespace std;



class ObliczKlatki {
private:
	VideoCapture film;
	vector <Point2d> punktyPrzes;
	vector <Point2d> wektorPrzes;
	vector <Point2d> wektorPrzes2;
	vector <Point2d> wektorWybObraz;
	Point pozycjaRoi1;
	vector <Mat> zdje;
	int wys;
	int szer;
	int minY;
	int maxY;
	int maxX;

public:
	ObliczKlatki(string x) {
		film.open(x);
		if (!film.isOpened()) {
			cout << "nie otworzono" << endl;
		}
	}
	void liczPunkty3() { // tworzenie wektora przesuniêæ 
		Point aktual;// aktualne przesuniêcie
		Mat first, second, x;
		int i = 0, izdj = 1; // liczniki 
		wektorPrzes.push_back(Point2d(0, 0)); // ustawiamy pierwszy punkt jako 0
		film.read(first);// wczytaj pierwsz¹ klatkê 
		szer = first.cols; //szerokoœæ obrazu
		wys = first.rows;// wysokoœæ obrazu
		first.copyTo(x);//kopiowanie obrazu do zmiennej x
		zdje.push_back(x);// wstawianie pierwszej klatki jako pierwszy element
		wektorWybObraz.push_back(Point2d(0, 0));
		while (film.read(second))// wczytuje klatki 
		{
			aktual = wektorPrzes[i] + (-1 * normCorrelationPhase(first, second));// oblicza przesuniêcie 											  // wzglêdem pocz¹tkowej klatki
			wektorPrzes.push_back(aktual);//wstawia przesuniêcie do wektora punktów
			cout << wektorPrzes[i] << endl;//wyœwietlanie 
			if ((abs(aktual.x) >= ((3 * first.cols) / 4)*izdj) || (aktual.y >= ((3 * first.rows) / 4)*izdj)) {//je¿eli przesuniêcie przekroczy 3/4 szerokoœci lub wysokoœci 
				Mat y;
				second.copyTo(y);//kopiowanie obrazu do y
				zdje.push_back(y);// wstawianie obrazu
				wektorWybObraz.push_back(aktual);//wstawiane punktu
				cout << "zaliczone:   " << aktual << endl;//wyœwietla
				if (aktual.y > maxY)maxY = aktual.y;//znajdowanie maksymalnej wartoœci wartoœci przesuniêcia wzglêdem osi y 
				if (aktual.y < minY)minY = aktual.y;// znajdowanie minimalnej --||--
				izdj++;//po ka¿dym wczytaniu zdjêcia przesuwa wartoœæ przesuniêcia dla której bêdzie wczytane koeljne zdjêcie
			}
			i++;
			second.copyTo(first);// kopiowanie do poprzedzaj¹cej klatki klatkê nastêpuj¹c¹ dla tej iteracji
		}
	}
	void liczPunkty2() { // tworzenie wektora przesuniêæ 
		Point aktual,aktual2;// aktualne przesuniêcie
		Mat first, second, x;
		int i = 0, izdj = 1; // liczniki 
		wektorPrzes.push_back(Point2d(0, 0));
		wektorPrzes2.push_back(Point2d(0, 1999));// ustawiamy pierwszy punkt jako 0
		film.read(first);// wczytaj pierwsz¹ klatkê 
		szer = first.cols; //szerokoœæ obrazu
		wys = first.rows;// wysokoœæ obrazu
		first.copyTo(x);//kopiowanie obrazu do zmiennej x
		zdje.push_back(x);// wstawianie pierwszej klatki jako pierwszy element
		wektorWybObraz.push_back(Point2d(0, 0));// wstawianie punktu wzglêdem 
		Mat lin( 2000, 13000, CV_8UC3, Scalar(0, 0, 0));										// którego wystêpuj¹ przesuniêcia
		while (film.read(second))// wczytuje klatki 
		{
			aktual = wektorPrzes[i]+ (-1*normCorrelationPhase(first, second));// oblicza przesuniêcie 	
			aktual2 = aktual;
			aktual2.y += 1999;
			line(lin, wektorPrzes2[i], aktual2, Scalar(255, 255, 255),4);// wzglêdem pocz¹tkowej klatki
			wektorPrzes2.push_back(aktual2);
			wektorPrzes.push_back(aktual);//wstawia przesuniêcie do wektora punktów
			cout << wektorPrzes[i] << endl;//wyœwietlanie 
			if ((aktual.x >= ((3 * first.cols) /4)*izdj) || (aktual.y >= ((3 * first.rows) / 4)*izdj)) {//je¿eli przesuniêcie przekroczy 3/4 szerokoœci lub wysokoœci 
				Mat y;
				second.copyTo(y);//kopiowanie obrazu do y
				zdje.push_back(y);// wstawianie obrazu
				wektorWybObraz.push_back(aktual);//wstawiane punktu
				cout << "zaliczone:   " << aktual << endl;//wyœwietla
				if (aktual.y > maxY)maxY = aktual.y;//znajdowanie maksymalnej wartoœci wartoœci przesuniêcia wzglêdem osi y 
				if (aktual.y < minY)minY = aktual.y;// znajdowanie minimalnej --||--
				izdj++;//po ka¿dym wczytaniu zdjêcia przesuwa wartoœæ przesuniêcia dla której bêdzie wczytane koeljne zdjêcie
			}
			i++;
			second.copyTo(first);// kopiowanie do poprzedzaj¹cej klatki klatkê nastêpuj¹c¹ dla tej iteracji
		}
		imwrite("wynikowy1.jpg", lin);
	}
	void liczPunkty() {
		wektorPrzes.push_back(Point(0, 0));
		Point aktual;
		int i =0, izdj=1;
		int minY = 0, maxY = 0, maxX = 0;
		Mat first, second,x;
		film.read(first);
		szer = first.cols;
		wys = first.rows;
		first.copyTo(x);
		zdje.push_back(x);
		wektorWybObraz.push_back(Point(0, 0));
		pozycjaRoi1 = Comparation(first, first).getFirstPoint();
		while (film.read(second))
		{
			Comparation por(first, second);
			por.lokalizuj();
			punktyPrzes.push_back(por.getSecondPoint());
			second.copyTo(first);
			aktual.x = wektorPrzes[i].x + pozycjaRoi1.x - punktyPrzes[i].x;
			aktual.y = wektorPrzes[i].y + pozycjaRoi1.y - punktyPrzes[i].y;
			wektorPrzes.push_back(aktual);
			cout << wektorPrzes[i] << endl;
			cout << "[ " << pozycjaRoi1.x - punktyPrzes[i].x << " , " << pozycjaRoi1.y - punktyPrzes[i].y << "]\n";
			if ((aktual.x >= ((3 * first.cols) / 4)*izdj)||(aktual.y>=((3*first.rows)/4)*izdj)) {
				Mat y;
				second.copyTo(y);
				zdje.push_back(y);
				wektorWybObraz.push_back(aktual);
				//cout << "zaliczone:   " << aktual << endl;
				if (aktual.y > maxY)maxY = aktual.y;
				if (aktual.y < minY)minY = aktual.y;
				izdj++;
			}
			i++;
		}
		maxX = aktual.x;
	}
	void lonczPan() {// ³¹czenie w panorame
		Mat wynik;
		Size roz = Size(zdje[0].cols, zdje[0].rows - minY + maxY);// wysokoœæ uwzglêdnia przesuniêcia pionowe
		Mat neut = (Mat_<double>(2, 3) << 1, 0, 0, 0, 1, 0);//Macierz rotacji nic nie zmienia
		for (int i = 0; i < zdje.size(); i++) {
			int przes = -(minY - wektorWybObraz[i].y);//przesuniêcie w osi y
			warpAffine(zdje[i], zdje[i], neut, roz);//zwiêkszanie rozmiaru 
			Mat M = (Mat_<double>(2, 3) << 1, 0, 0,// Macierz translacji w osi y 
				0, 1, przes);
			warpAffine(zdje[i], zdje[i], M, roz);//translacja w osi y
			zdje[i] = Mat(zdje[i], Rect(0, 0, 3* szer / 4, wys));// przycinanie zdjêcia 
			cout << przes << endl;
		}

		cout << minY << "     maxY:   " << maxY << "     maxX:   " << maxX << endl;
		hconcat(zdje, wynik);//³¹czenie zdjeæ 
		for (int i = 1; i < zdje.size(); i++) {
			Mat roi = Mat(wynik, Rect(wektorWybObraz[i].x -16, 0, +16, wynik.rows));//tworzenie wycinka obrazu dla którego obraz bêdzie filtrowany
			blur(roi, roi, Size(5, 5));// funkcja realizuj¹ca filtr uœredniaj¹cy 
			//imshow("dd", roi);
			//waitKey(500);
		}
		imwrite("wynikowy.jpg", wynik);//zapis do pliku
		namedWindow("gg", WINDOW_AUTOSIZE);
		imshow("gg", wynik);
		waitKey(0);

	}
	Mat tworzPano(int minY , int maxY, int maxX) {
		Mat docelowyObraz(maxX+szer, maxY - minY+wys, CV_8UC3);
		for (int i = 0; i < zdje.size(); i++) {

		}

	}

	void wysw() {
		vector <string> nazw;
		namedWindow("pokaz", WINDOW_AUTOSIZE);
		namedWindow("pokaz1", WINDOW_AUTOSIZE);
		namedWindow("pokaz2", WINDOW_AUTOSIZE);
		namedWindow("pokaz3", WINDOW_AUTOSIZE);
		namedWindow("pokaz4", WINDOW_AUTOSIZE);
		for (int i = 0; i < wektorPrzes.size(); i++) {
			cout << wektorPrzes[i]<< endl;
		}
		imshow("pokaz", zdje[0]);
		imshow("pokaz1", zdje[1]);
		imshow("pokaz2",zdje[2]);
		imshow("pokaz3", zdje[3]);
		imshow("pokaz4", zdje[4]);
		waitKey(0);
		destroyAllWindows();
	}
	Point2d normCorrelationPhase(Mat x, Mat y) {//zwraca przesuniêcie 
		Mat xGray, yGray , hann;
		Point2d shift;
		cvtColor(x, xGray, CV_BGR2GRAY);//zamiana obrazu na obraz szary
		cvtColor(y, yGray, CV_BGR2GRAY);// jw.
		xGray.convertTo(xGray, CV_32FC1);//zamiana typu danych pikseli
		yGray.convertTo(yGray, CV_32FC1);//jw.
		createHanningWindow(hann, xGray.size(), CV_32FC1); // tworzy okno Hanninga o rozmiarze identycznym jak obrazy
		shift = phaseCorrelate(xGray, yGray, hann); // oblicza przesuniêcie 
		return shift;
	}
	


};


int main() {
	/*VideoCapture video("sk2.avi");
	Mat frame, curr, prev, curr64f, prev64f, hann;
	char key;
	video.read(frame);
	cvtColor(frame, prev, CV_BGR2GRAY);
	createHanningWindow(hann, prev.size(), CV_32FC1);
	while (video.read(frame)) {
		cvtColor(frame, curr, CV_BGR2GRAY);
		curr.convertTo(curr, CV_32FC1);
		prev.convertTo(prev, CV_32FC1);
		Point2d shift = phaseCorrelate(prev, curr, hann);
		curr.copyTo(prev);
		cout << shift << endl;
	}
	cout << prev.cols << endl;*/
	//cout << trz << endl;
	//cout << i <<"    przesuniecie:   "<<punkt<< endl;


	string nazwa;
	int ktory;
	double t1, t2, t3;
	cout << "podaj nazwe:  ";
	cin >> nazwa;
	ObliczKlatki kalt2(nazwa);
	t1 = clock();
	cout << "1. Metoda przeszukiwania oknami\n2. Metoda korelacji faz\nwybierz ktora:  "<<endl;
	cin >> ktory;
	if (ktory == 1)kalt2.liczPunkty();
	if (ktory == 2)kalt2.liczPunkty2();
	kalt2.lonczPan();
	t2 = clock();
	//kalt.wysw();
	cout << "time:   " << (t2 - t1) / CLOCKS_PER_SEC << endl;
	getchar();
	return 0;
}