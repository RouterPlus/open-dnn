#include <opencv2/opencv.hpp>
#include<iostream>
#include<opencv2/dnn.hpp>
using namespace std;
using namespace cv;
using namespace cv::dnn;
void predict_age(Mat &src);
void predict_gender(Mat &src);
String case_file = "D:/new_cv/opencv/sources/data/haarcascades/haarcascade_frontalface_alt_tree.xml";
String model_age1_file = "D:/new_cv/opencv/sources/samples/data/age_net.caffemodel";
String model_age1_txt = "D:/new_cv/opencv/sources/samples/data/age_deploy.prototxt";

String model_gender1_bin = "D:/new_cv/opencv/sources/samples/data/gender_net.caffemodel";
String model_gender1_txt = "D:/new_cv/opencv/sources/samples/data/gender_deploy.prototxt";

int main(int argc, char** argv)
{


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
	cascader.detectMultiScale(gray, res, 1.02, 1, 0, Size(27, 27));
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
	cout << res.size() << endl;
	for (size_t t = 0; t < res.size(); t++)
	{
		rectangle(src, res[t], Scalar(1, 1, 2), 1, 8, 0);
		Mat dst = src(res[t]);
		predict_age(dst);
		predict_gender(dst);
	}
	imshow("detection result", src);

	waitKey(0);
	return 0;
}
vector<String> get_age_label()
{
	vector<String> age_labels;
	age_labels.push_back("0-2");
	age_labels.push_back("4-6");
	age_labels.push_back("8-13");
	age_labels.push_back("15-20");
	age_labels.push_back("25-32");
	age_labels.push_back("38-43");
	age_labels.push_back("48-53");
	age_labels.push_back("60-");
	return age_labels;
}
void predict_age(Mat &src)
{
	Net net = readNetFromCaffe(model_age1_txt, model_age1_file);
	if (net.empty())
	{
		cout << "load net error" << endl;
		exit(-1);
	}
	Mat blobImg = blobFromImage(src, 1.0, Size(227, 227));
	net.setInput(blobImg, "data");
	Mat probMat = net.forward("prob");
	probMat.reshape(1, 1);//1��1ͨ��
	Point index;//������Ϣ
	double objvalue;//�����ֵ
	minMaxLoc(probMat, NULL, &objvalue, NULL, &index);//������Сֵ��ȡ���ֵ
	size_t objindex = index.x;//label �±�
	vector<String>labels = get_age_label();
	putText(src, format("age:%s", labels[objindex].c_str()), Point(2, 20), FONT_HERSHEY_PLAIN, 0.7, Scalar(1, 12, 3), 1, 8);
}
void predict_gender(Mat &src)
{
	Net net = readNetFromCaffe(model_age1_txt, model_age1_file);
	if (net.empty())
	{
		cout << "load net error" << endl;
		exit(-1);
	}
	Mat blobImg = blobFromImage(src, 1.0, Size(227, 227));
	net.setInput(blobImg, "data");
	Mat probMat = net.forward("prob");
	probMat.reshape(1, 1);//1��1ͨ��
	putText(src, format("gender:%s", (probMat.size[0] > probMat.size[1] ? "M" : "F")), Point(2, 10), FONT_HERSHEY_PLAIN, 0.7, Scalar(2, 2, 3), 1, 8);
}