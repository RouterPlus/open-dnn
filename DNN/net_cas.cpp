#include <opencv2/opencv.hpp>
#include<iostream>
#include<opencv2/dnn.hpp>
using namespace std;
using namespace cv;
using namespace cv::dnn;

int main(int argc, char** argv)
{
	String case_file = "D:/new_cv/opencv/sources/data/haarcascades/haarcascade_frontalface_alt_tree.xml";
	Mat src = imread("D:/test/test.jpg");
	if (src.empty())
	{
		cout << "src is empty" << endl;
		return -1;
	}

	CascadeClassifier cascader;//������������

	cascader.load(case_file);
	if (cascader.empty())
	{
		cout << "load error" << endl;
		return -1;
	}
	vector<Rect> res;//�������ص����������
	Mat gray;//ʹ�ûҶ�ͼ����м��
	cvtColor(src, gray, CV_RGB2GRAY);//��ȡ�Ҷ�ͼ��
	imshow("gray", gray);
	equalizeHist(gray, gray);
	cascader.detectMultiScale(gray, res, 1.2, 3, 0, Size(27, 27));
	/*
	void detectMultiScale(
	const Mat& image,                //�����Ҷ�ͼ��
	CV_OUT vector<Rect>& objects,    //���������ľ��ο�����
	double scaleFactor = 1.1,        //ǰ��������̵�ɨ�����������ڵı���ϵ����Ĭ��Ϊ1.1 ��ÿ��������������10%
	int minNeighbors = 3,            //���ɼ��Ŀ������ھ��ε���С���� �����ɼ��Ŀ���С���εĸ�����С��minneighbors - 1 ���ᱻ�ų�
									 //���minneighborsΪ0 ���������κβ����ͷ������б����ѡ���ο�
	int flags = 0,                   //������ΪCV_HAAR_DO_CANNY_PRUNING ��������ʹ��Canny��Ե������ų���Ե�������ٵ�����
	Size minSize = Size(),
	Size maxSize = Size()            //������������������Ƶõ���Ŀ������ķ�Χ
	);
	*/
	for (size_t t = 0; t < res.size(); t++)
	{
		rectangle(src, res[t], Scalar(1, 1, 2), 1, 8, 0);
	}
	imshow("detection result", src);

	waitKey(0);
	return 0;
}