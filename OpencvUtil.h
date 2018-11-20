
#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <vector>

class OpencvUtil {

public:
	OpencvUtil() {};
	using int_pair = std::pair<int, int>; // first = height/x , second= width/y
	cv::Mat readImage(const std::string& imageName);
	void drawTestImage(std::string& text, std::string& ouput_file, int_pair dimension, int_pair pos);

	template<typename dType>
	std::vector<dType> getImageData(const std::string imageName);
	void dumpMatRaw(const cv::Mat& image, const std::string& fileName);

	void dumpMat(const cv::Mat& image, const std::string& fileName);
	void retriveMat(const std::string& fileName, cv::Mat& image);

	template<typename dType>
	int  convertMatToVec(const cv::Mat& mat, std::vector<dType> &array);
	template<typename dType>
	int convertVecToMat(std::vector<dType> data, cv::Mat& mat);

	int convertMatFromBmpFmt(cv::Mat& imgMat, uint32_t* data) ;
	int convertMatToBmpFrmt(cv::Mat& imgMat, uint32_t* data) ;

	void substractImage(const std::string imgName1,const std::string imgName2, std::string output);
};

