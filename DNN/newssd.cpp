#include<iostream>
#include<opencv2/dnn.hpp>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::dnn;

String caffe_bin_file = "D:/new_cv/opencv/sources/samples/data/dnn/model_ssd_det/VGG_ILSVRC2016_SSD_300x300_iter_440000.caffemodel";
String caffe_txt_file = "D:/new_cv/opencv/sources/samples/data/dnn/model_ssd_det/deploy.prototxt";
String label_txt = "D:/new_cv/opencv/sources/samples/data/dnn/model_ssd_det/label.txt";

vector<String> readfromLabels();
const size_t width = 300;
const size_t height = 300;
const int meanv[3]= { 104, 117, 123 };
Mat getMean(const size_t &width,const size_t &height)
{
	vector<Mat> channels;
	for (int i = 0; i < 3; i++)
	{
			Mat channel(width,height,CV_32F,Scalar(meanv[i]));
			channels.push_back(channel);
	}
	Mat mean;
	merge(channels, mean);//ͼ��ϲ�
	imshow("mean", mean);
	return mean;
}
Mat preprocessed(const Mat &src)
{
	Mat out_p;
	src.convertTo(out_p, CV_32F);//����������ԭʼͼ�񣬴��¶���һ��Mat ����������
	Mat mean = getMean(width, height);
	resize(out_p, out_p, Size(width, height));
	subtract(out_p, mean, out_p);//ȡ��ֵ
	imshow("out_p", out_p);
	return out_p;
}
int main(int argc, char** argv)
{
	Mat src = imread("D:/test/test.jpg");
	if (src.empty())
	{
		cout << "load image error" << endl;
		return -1;
	}
	imshow("src", src);
	Mat input_src = preprocessed(src);
	
	//Ptr<dnn::Importer> importer;//����ָ����÷�
	//try {
	//	importer = dnn::createCaffeImporter(caffe_txt_file, caffe_bin_file);
	//}
	//catch (const cv::Exception &err)//Importer can throw error which we can catch 
	//{
	//	cerr << err.msg << endl;
	//}

	//Net net;
	//importer->populateNet(net);//�������
	//importer.release();
	Net net = readNetFromCaffe(caffe_txt_file, caffe_bin_file);

	Mat blobImg = blobFromImage(input_src);//תΪblob��ʽ����
	net.setInput(blobImg, "data");//���������ļ�
	Mat detection = net.forward("detection_out");
	//Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());//�������������������ֵ
	Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
	cout << "detection.size" << detection.size[2]<<"    "<<detection.size[3] << endl;
	cout << "detectionMat " << detectionMat.rows << " " << detectionMat.cols << endl;
	float confidence = 0.3;
	for (int i = 0; i < detectionMat.rows; i++)
	{
		//for (int j = 0; j < detectionMat.cols; j++)
		//{
		//	cout << detectionMat.at<float>(i, j) << " ";
		//}
		//cout << endl;
		cout << i << endl;
		float con_det = detectionMat.at<float>(i, 2);
		if (con_det > confidence)
		{
			size_t objindex = size_t(detectionMat.at<float>(i, 0));//Mat ���±�����
			float x1 = detectionMat.at<float>(i, 3)*src.cols;
			float y1 = detectionMat.at<float>(i, 4)*src.rows;
			float x2 = detectionMat.at<float>(i, 5)*src.cols;
			float y2 = detectionMat.at<float>(i, 6)*src.rows;

			Rect rct((int)x1, (int)y1, (int)(x2 - x1), (int)(y2 - y1));//x1,x2,weight,height

			rectangle(src, rct, Scalar(3, 4, 32), 2, 8, 0);
		}
		
	}
	imshow("newsrc", src);
	vector<String>result = readfromLabels();
	waitKey(0);
	return 0;
}
vector<String>readfromLabels()
{
	ifstream fp(label_txt);//�����ļ���
	vector<String> result;
	if (!fp.is_open())//�ж��Ƿ����������
	{
		cout << "count not open file" << endl;
		exit(-1);
	}
	string labelname;
	while (!fp.eof()) //ȡ��״̬
	{
		getline(fp, labelname);//��ȡһ�����ݵ�label��
		if (labelname.length())
		{
			string temps = labelname.substr(labelname.find(",") + 1);//��ȡ��һ�������Ժ�Ĳ���
			string temp2 = temps.substr(temps.find(",") + 1);
			result.push_back(temp2);
		}
	}
	//for (vector<String>::iterator it = result.begin(); it != result.end(); it++)
	//{
	//	cout << *it << endl;
	//}
	return result;
}