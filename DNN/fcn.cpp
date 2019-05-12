#include<iostream>	
#include<opencv2/opencv.hpp>
#include<opencv2/dnn.hpp>
using namespace std;
using namespace cv;
using namespace cv::dnn;

String fcn_label_txt = "D:/new_cv/opencv/sources/samples/data/dnn/pascal-classes.txt";
String fcn_txt = "D:/new_cv/opencv/sources/samples/data/dnn/fcn8s-heavy-pascal.prototxt";
String fcn_model = "D:/new_cv/opencv/sources/samples/data/dnn/fcn8s-heavy-pascal.caffemodel";

vector<Vec3b> readColors();
int main(int argc, char** argv)
{

	Mat src = imread("D:/test/test.jpg");
	if (src.empty())
	{
		cout << "imput img is empty" << endl;
		return -1;
	}
	//resize
	resize(src, src, Size(500, 500));
	imshow("src", src);
	vector<Vec3b> labels = readColors();
	Mat blobimg = blobFromImage(src);


	Net net = readNetFromCaffe(fcn_txt,fcn_model);
	if (net.empty())
	{
		cout << "init net error";
		return -1;
	}

	float time = getTickCount(); //��ʱ��
	cout << time;
	net.setInput(blobimg, "data");
	Mat score = net.forward("score");
	float tt = getTickCount() - time;
	cout << "time consume: " << (tt / getTickFrequency() * 1000) << endl;


	cout << score.size <<" "<< score.size[1]<<" " << score.size[2]<<" " << score.size[3] << endl;;
	const int rows = score.size[2];//height
	const int cols = score.size[3];//width
	const int chls = score.size[1];//channels

	Mat maxC1(rows, cols, CV_8UC1); //�洢����ѡ����ͨ����21��֮һ��
	Mat MaxVal(rows, cols, CV_32FC1);//�洢label��key

	//����Ҫ������score �����ݽṹΪ 21��ͨ����ÿ��ͨ����500*500���صĽ��
	//set ���ұ�
	for (int c = 0; c < chls; c++) //һ��21��ͨ������21��ͨ����ÿ�������ص�����Ŷ�
	{
		for (int row = 0; row < rows; row++)//500��
		{
			const float *ptrScore = score.ptr<float>(0, c, row);//ָ��ÿ��ͨ���ĵ�1��λ�ã��洢����21��label�����Ŷ� 
		//	cout << *ptrScore << endl;
			uchar *ptrMaxC1 = maxC1.ptr<uchar>(row); //Ϊ��
			float *ptrMaxVal = MaxVal.ptr<float>(row);
			for(int col = 0; col < cols; col++) //500��
			{
				if (ptrScore[col] > ptrMaxVal[col])
				{
					ptrMaxVal[col] = ptrScore[col]; //�洢���յ�ֵҲ����color�ʵĽ��
					ptrMaxC1[col] = (uchar)c;//�洢����ѡ���ͨ��
				}
			}
		}
	}

	//look up colors
	Mat result = Mat::zeros(rows, cols, CV_8UC3);//��ʼ��ȫ0��ͼ�����
	for (int row = 0; row < rows; row++)
	{
		const uchar *ptrMaxC1 = maxC1.ptr<uchar>(row); //ָ���Ӧ�Ĳ��ұ��е���
		Vec3b *ptrColor = result.ptr<Vec3b>(row);//ָ����ұ��е���
		for (int col = 0; col < cols; col++)
		{
			ptrColor[col] = labels[ptrMaxC1[col]];//��Ӧλ��
		}
	}
	imshow("result", result);
	Mat dst;
	addWeighted(src, 0.4, result, 0.6, 0, dst);
	imshow("FCN", dst);

	waitKey(0);
	return 0;
}
vector<Vec3b> readColors()
{
	vector<Vec3b> colors;
	ifstream fp(fcn_label_txt);//������
	if (!fp.is_open())
	{
		cout << "can not open label file" << endl;
		exit(-1);
	}
	string line;
	while (!fp.eof())//�����û�н���
	{
		getline(fp, line);
		if (line.length())//�ж��Ƿ���Դ�
		{
			stringstream ss(line);//�����ַ���
			string name;
			ss >> name;
		//	cout << name << endl;;
			int temp;
			Vec3b color;
			ss >> temp;
			cout << temp;
			color[0] = (uchar)temp;
			ss >> temp;
			color[1] = (uchar)temp;
			ss >> temp;
			color[2] = (uchar)temp;
	//		cout << temp << endl;
			colors.push_back(color);
		}
	}
	return colors;
}
/*
background 0 0 0
aeroplane 128 0 0
bicycle 0 128 0
bird 128 128 0
*/
