#include<stdio.h>
#include<opencv2/opencv.hpp>
#include<opencv2/dnn.hpp>
using namespace std;
using namespace cv;
using namespace cv::dnn;


cv::String model_gt_file = "D:/new_cv/opencv/sources/samples/data/dnn/bvlc_googlenet.prototxt";
cv::String model_gb_file = "D:/new_cv/opencv/sources/samples/data/dnn/bvlc_googlenet.caffemodel";
String label_gt_file = "D:/new_cv/opencv/sources/samples/data/dnn/synset_words.txt";
vector<String> readfromgtxt();
const size_t width = 224;
const size_t height = 224;
int main(int argc, char** argv)
{
	vector<String> labels = readfromgtxt();
	Mat src = imread("D:/test/test.jpg");
	if (src.empty())
	{
		cout << "load image error" << endl;
		return -1;
	}
	Ptr<dnn::Importer> impoter;

	try {
		impoter = dnn::createCaffeImporter(model_gt_file, model_gb_file);
	}
	catch(const cv::Exception &err)//���Կ���һ����׽����
	{
		cerr << "model impoter error" << endl;
		return -1;
	}
	Net net;//��ʼ������
	impoter->populateNet(net);//�������
	Mat blobImg = blobFromImage(src, 1.0, Size(224, 224), Scalar(104, 103, 127));
	Mat out_p;//���1*1000 �ľ���
	for (int i = 0; i < 10; i++)//һ������Ϣ��������һ�£��������벻ͬά����ͬһ��ͼƬ��Ч��
	{
		net.setInput(blobImg, "data");
	    out_p = net.forward("prob");
//		cout << out_p.at<float>(0, 40)<<" ";
	}
	Mat probMat = out_p.reshape(1, 1);
	cout << out_p.size() << out_p.rows << " " << out_p.cols << endl;
	cout << probMat.size() << probMat.rows<<" "<<probMat.cols<< endl;

	imshow("src", src);
	waitKey(0);
	return 0;
}
vector<String> readfromgtxt()
{
	ifstream fp(label_gt_file);
	vector<String> result;
	if (!fp.is_open())
	{
		cout << "count not open the file" << endl;
		exit(-1);
	}
	string name;//�����ֶν���ÿһ�е�ֵ
	while (!fp.eof())
	{
		getline(fp, name);//�������name��
		string tmp = name.substr(name.find(" ") + 1);
		result.push_back(tmp);
	}
	cout << result.size() << endl;
	//for (vector<String>::iterator it = result.begin(); it != result.end(); it++)
	//{
	//	cout << *it << endl;
	//}
	return result;
}

