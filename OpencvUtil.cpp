#include "OpencvUtil.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>

static const bool LOG_VERBOSE =
        std::getenv("LOG_VERBOSE") ? atoi(std::getenv("LOG_VERBOSE")) == 1 : false;




void OpencvUtil::dumpMat(const cv::Mat& image, const std::string& fileName) {
	cv::FileStorage fs(fileName, cv::FileStorage::WRITE);
	fs << "mat" << image;
	fs.release();
}

void OpencvUtil::retriveMat(const std::string& fileName, cv::Mat& image) {
	cv::FileStorage fs2(fileName, cv::FileStorage::READ);
	fs2["mat"] >> image;
	fs2.release();
}

template<typename dType>
int  OpencvUtil::convertMatToVec(const cv::Mat& mat, std::vector<dType> &array) {
	//std::vector<dType> array;
	if (mat.isContinuous()) {
	  array.assign((dType*)mat.datastart, (dType*)mat.dataend);
	} else {
	  for (int i = 0; i < mat.rows; ++i) {
	    array.insert(array.end(), mat.ptr<dType>(i), mat.ptr<dType>(i)+mat.cols);
	  }
	}
	if(LOG_VERBOSE)
	    std::cout <<"array.size"<<array.size();

	return 0;
}

template<typename dType>
int OpencvUtil::convertVecToMat(std::vector<dType> input, cv::Mat& mat) {

	//TODO untested
	if (mat.isContinuous()) {
		memcpy(mat.data, input.data(), input.size()*sizeof(dType));
	} else {
		dType * vecPtr = input.data();
	  for (int i = 0; i < mat.rows; ++i) {
		  memcpy(mat.ptr<dType>(i), vecPtr, mat.cols*sizeof(dType));
		  vecPtr = vecPtr + mat.cols;
	  }
	}
	if(LOG_VERBOSE)
	    std::cout <<"array.size"<<mat.size();
	return 0;
}

void OpencvUtil::dumpMatRaw(const cv::Mat& image, const std::string& fileName) {
	std::vector<float> img;
	convertMatToVec<float>(image, img);

	std::ofstream fstrm(fileName);
	for(unsigned i = 0; i < img.size(); i++)
		fstrm<<img.at(i)<< " ";

}
cv::Mat OpencvUtil::readImage(const std::string& imageName) {
	cv::Mat image;
	image = cv::imread(imageName, -1);
	return image;

}

//http://opencvexamples.blogspot.com/2013/10/basic-drawing-examples.html
void OpencvUtil::drawTestImage(std::string& text, std::string& ouput_file,
        int_pair dimension, int_pair pos){
	cv::Mat image = cv::Mat::zeros(dimension.first, dimension.second, CV_32FC3);
	cv::Point pos_pt(pos.first, pos.second);
	cv::Scalar clor (0, 200, 200);
	cv::putText(image, text, pos_pt, cv::FONT_HERSHEY_SIMPLEX, 1,clor, 4);
	cv::imwrite(ouput_file, image);
}

template<typename dType>
std::vector<dType> OpencvUtil::getImageData(const std::string imageName){
	cv::Mat img = cv::imread(imageName, -1);
	std::vector<dType> data;
	convertMatToVec<dType>(img, data);
	return data;
}

void OpencvUtil::substractImage(const std::string imgName1,const std::string imgName2,
        std::string output){
#if 1
	cv::Mat img1 = cv::imread(imgName1, -1);
	cv::Mat img2 = cv::imread(imgName2, -1);

	cv::Mat img3;
	cv::subtract(img1,img2,img3);
	dumpMat(img3, "substractImage");
	cv::imwrite(output,img3);
#else
	cv::Mat img1 = cv::imread(imgName1, -1);
	cv::Mat img2 = cv::imread(imgName2, -1);

	std::vector<cv::Mat> vecImg1;
	std::vector<cv::Mat> vecImg2;
	std::vector<cv::Mat> vecImg3;

	for(unsigned channel = 0; channel < img1.channels(); channel++)
	{
		cv::Mat m1(img1.rows, img1.cols, CV_32FC1);
		vecImg1.push_back(m1);

		cv::Mat m2(img1.rows, img1.cols, CV_32FC1);
		vecImg2.push_back(m1);

		cv::Mat m3(img1.rows, img1.cols, CV_32FC1);
		vecImg3.push_back(m1);

	}

	cv::split(img1, vecImg1);
	cv::split(img2, vecImg2);

	for(unsigned channel = 0; channel < img1.channels(); channel++)
	{
		cv::subtract(vecImg1[channel],vecImg2[channel], vecImg3[channel]);
	}

	cv::Mat img3;
	cv::merge(vecImg3, img3);
	dumpMat(img3, "substractImage");
	cv::imwrite(output,img3);

#endif
}

int OpencvUtil::convertMatToBmpFrmt(cv::Mat& imgMat, uint32_t* data) {
    // accept only char type matrices
    if (imgMat.depth() != CV_8U) return -1;
    if (imgMat.channels() != 3) return -2;

    for (int i = imgMat.rows - 1; i >= 0; i--) {
        cv::Vec3b* pix = imgMat.ptr<cv::Vec3b>(i);
        for (int j = 0; j < imgMat.cols; j++) {
            uchar blue = pix[j][0];
            uchar green = pix[j][1];
            uchar red = pix[j][2];

            uint32_t pixel = 0;
            pixel |= (uint32_t)red;
            pixel |= (uint32_t)(green << 8);
            pixel |= (uint32_t)(blue << 16);

            *data = pixel;
            data++;
        }
    }

    return 0;
}

int OpencvUtil::convertMatFromBmpFmt(cv::Mat& imgMat, uint32_t* data) {
    if (imgMat.depth() != CV_8U) return -1;
    if (imgMat.channels() != 3) return -2;

    for (int i = imgMat.rows - 1; i >= 0; i--) {
        cv::Vec3b* pix = imgMat.ptr<cv::Vec3b>(i);
        for (int j = 0; j < imgMat.cols; j++) {
            uint32_t pixel = *data;
            data++;

            uchar red = (uchar)(pixel & 0xff);
            uchar green = (uchar)((pixel & 0xff00) >> 8);
            uchar blue = (uchar)((pixel & 0xff0000) >> 16);

            pix[j][0] = blue;
            pix[j][1] = green;
            pix[j][2] = red;
        }
    }

    return 0;
}

#if 1
int main(int argc, char** argv)
{
	char* imageName = argv[1];
	if(argc != 2)
	{
		std::cout << " Usages: ./ImageUtil.exe cat.jpg" << std::endl;
		return 0;
	}

	OpencvUtil util;

	cv::Mat img = cv::imread(imageName);

	std::vector<int> vec1;

	util.convertMatToVec<int>(img, vec1);
	std::for_each(vec1.begin(), vec1.end(), [](int x){x= x -10;});
	util.convertVecToMat(vec1,img);

	cv::imwrite("output.jpg",img);

	cv::waitKey(0);

	return 0;
}
#endif
