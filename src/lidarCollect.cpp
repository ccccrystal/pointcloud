#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/registration/icp.h>
#include <pcl/features/normal_3d.h>
#include <opencv2/opencv.hpp>
#include "recvLidarData.h"

void LidarParsing();
void UpdateImage();
double const PI = 3.1415926535897932384626433832795;
std::vector<int> cloudItensity;
pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
cv::VideoCapture capture;
cv::Mat srcImage;

int main()
{
	//capture.open(0);
	//capture.open("rtsp://admin:ganzhi@602@192.168.100.38:554");
	//int imageWidth = 1920;
	//int imageHeight = 1080;
	//capture.set(CV_CAP_PROP_FRAME_WIDTH, imageWidth);
	//capture.set(CV_CAP_PROP_FRAME_HEIGHT, imageHeight);
	//boost::thread imageUpdateThread(boost::bind(&UpdateImage));
	boost::thread cloudUpdateThread(boost::bind(&LidarReceiveThread));

	struct timeval tv;
	gettimeofday(&tv,0);
	struct tm *t = localtime(&(tv.tv_sec));
	char chCloudFileName[100] = { 0 };
	char chVideoFileName[100] = { 0 };
	sprintf(chCloudFileName, "%d-%02d-%02d %02d-%02d-%02d-Lidar16-2368.txt", 1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	//sprintf(chVideoFileName, "%d-%02d-%02d %02d-%02d-%02d-%d-%d.avi", 1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, imageWidth, imageHeight);
	//cv::VideoWriter writer(chVideoFileName, CV_FOURCC('M','J','P','G'), 10, cv::Size(imageWidth, imageHeight));
	FILE* file = fopen(chCloudFileName, "a");

	viewer->addCoordinateSystem(0.1);
	viewer->initCameraParameters();
	viewer->setCameraPosition(0, 0, 50, 0, 0, 0);

	while (true)
	{
		if (dataFlag)
		{
			LidarParsing();

			//if(!srcImage.empty())
			{
				//保存极坐标到txt
				/*fprintf(file, "%d\r\n", (int)lidarAngle.size());
				for(int i = 0; i < (int)lidarAngle.size(); i++)
				{
					fprintf(file, "%.2f", lidarAngle[i]);
					for(int j = 0; j < 32; j++)
						fprintf(file, " %d %d", lidarDist[j][i], lidarItensity[j][i]);
					fprintf(file, "\r\n");
				}*/

				//保存三维坐标到txt
				fprintf(file, "%d\r\n", (int)cloud->points.size());
				for(int i = 0; i < (int)cloud->points.size(); i++)
				{
					fprintf(file, "%.2f %.2f %.2f %d\r\n", cloud->points.at(i).x, cloud->points.at(i).y, cloud->points.at(i).z, cloudItensity[i]);
				}

				//writer << srcImage;
				//imshow("image", srcImage);
				//cv::waitKey(1);
			}

			viewer->addPointCloud(cloud, "cloud");
			viewer->spinOnce(1);
			viewer->removeAllPointClouds();
			viewer->removeAllShapes();
			cloud->points.clear();
			cloudItensity.clear();
			lidarDist.clear();
			lidarAngle.clear();
			lidarItensity.clear();
			dataFlag = false;
		}
		else
			usleep(1000);
	}
	return 0;
}

//数据解析
void LidarParsing()
{
	//计算16条线的正余弦值
	double cosTheta[16] = { 0 };
	double sinTheta[16] = { 0 };
	float theta[16] = {-15, 1, -13, 3, -11, 5, -9, 7, -7, 9, -5, 11, -3, 13, -1, 15};
	for (int i = 0; i < 16; i++)
	{
		cosTheta[i] = cos(theta[i] * PI / 180.f);
		sinTheta[i] = sin(theta[i] * PI / 180.f);
	}

	//计算一圈数据每个方向的正余弦值
	std::vector<std::vector<double> > sinAngle;
	std::vector<std::vector<double> > cosAngle;
	sinAngle.resize(16);
	cosAngle.resize(16);
	for (int i = 0; i < 16; i++)
	{
		for(int j = 0; j < (int)lidarAngle[i].size(); j++)
		{
			sinAngle[i].push_back(sin(lidarAngle[i][j] * PI / 180.f));
			cosAngle[i].push_back(cos(lidarAngle[i][j] * PI / 180.f));
		}
	}

	float lidarPitch = 0;
	float lidarRoll = 0;
	float lidarHeading = 0;
	float sinPitch = (float)sin(lidarPitch * CV_PI / 180.0);
	float cosPitch = (float)cos(lidarPitch * CV_PI / 180.0);
	float sinRoll = (float)sin(lidarRoll * CV_PI / 180.0);
	float cosRoll = (float)cos(lidarRoll * CV_PI / 180.0);
	float sinHeading = (float)sin(lidarHeading * CV_PI / 180.0);
	float cosHeading = (float)cos(lidarHeading * CV_PI / 180.0);

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < (int)lidarAngle[i].size(); j++)
		{
			//距离雷达小于30cm的点丢弃
			if(lidarDist[i][j] < 30)
			{
				continue;
			}

			pcl::PointXYZ PointTemp;
			PointTemp.x = (lidarDist[i][j] * cosTheta[i] * sinAngle[i][j]) / 100.f;
			PointTemp.y = (lidarDist[i][j] * cosTheta[i] * cosAngle[i][j]) / 100.f;
			PointTemp.z = (lidarDist[i][j] * sinTheta[i]) / 100.f;

			pcl::PointXYZRGB temp;
			temp.x = cosHeading * cosRoll * PointTemp.x + (-sinHeading * cosPitch + cosHeading * sinRoll * sinPitch) * PointTemp.y + (sinHeading * sinPitch + cosHeading * sinRoll * cosPitch) * PointTemp.z;
			temp.y = sinHeading * cosRoll * PointTemp.x + (cosHeading * cosPitch + sinHeading * sinRoll * sinPitch) * PointTemp.y + (-cosHeading * sinPitch + sinHeading * sinRoll * cosPitch) * PointTemp.z;
			temp.z = -sinRoll * PointTemp.x + cosRoll * sinPitch * PointTemp.y + cosRoll * cosPitch * PointTemp.z;

			if (lidarItensity[i][j] <= 63)
			{
				temp.r = 0;
				temp.g = 254 - 4 * lidarItensity[i][j];
				temp.b = 255;
			}
			else if (lidarItensity[i][j] > 63 && lidarItensity[i][j] <= 127)
			{
				temp.r = 0;
				temp.g = 4 * lidarItensity[i][j] - 254;
				temp.b = 510 - 4 * lidarItensity[i][j];
			}
			else if (lidarItensity[i][j] > 127 && lidarItensity[i][j] <= 191)
			{
				temp.r = 4 * lidarItensity[i][j] - 510;
				temp.g = 255;
				temp.b = 0;
			}
			else if (lidarItensity[i][j] > 191 && lidarItensity[i][j] <= 255)
			{
				temp.r = 255;
				temp.g = 1022 - 4 * lidarItensity[i][j];
				temp.b = 0;
			}
			cloud->points.push_back(temp);
			cloudItensity.push_back(lidarItensity[i][j]);
		}
	}
}

void UpdateImage()
{
	while(true)
	{
		capture.read(srcImage);
	}
}
