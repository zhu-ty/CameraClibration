/**
@brief class to calibrate single camera
@author zhu-ty
@date Nov 20, 2018
*/

#ifndef __CAMERA_CALIBRATION_SINGLE__
#define __CAMERA_CALIBRATION_SINGLE__

#include "SKCommon.hpp"

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

//#define ASPECT_RATIO 1.0
#define FIND_POINT_MIN_AREA_REDDOT 500
#ifdef _DEBUG
#define FIND_POINT_TIMEOUT_MS 10000
#else
#define FIND_POINT_TIMEOUT_MS 2000
#endif

class SingleCalibrater
{
private:
	//const int flags = cv::CALIB_USE_INTRINSIC_GUESS | cv::CALIB_FIX_ASPECT_RATIO | cv::CALIB_TILTED_MODEL | cv::CALIB_FIX_K3 | cv::CALIB_FIX_K4 | cv::CALIB_FIX_K5;
	/*
	https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga3207604e4b1a1758aa66acb6ed5aa65d
	*/
	const int flags = cv::CALIB_RATIONAL_MODEL | 
		//cv::CALIB_THIN_PRISM_MODEL | 
		//cv::CALIB_TILTED_MODEL | 
		cv::CALIB_ZERO_TANGENT_DIST |
		cv::CALIB_FIX_PRINCIPAL_POINT |
		cv::CALIB_FIX_K3 | 
		cv::CALIB_FIX_K4 | 
		cv::CALIB_FIX_K5 | 
		cv::CALIB_FIX_K6 |
		cv::CALIB_FIX_ASPECT_RATIO;

	std::string _xmlListFile;
	std::vector<std::string> _listFile;
	int _cornerWidth, _cornerHeight;
	double _squareSize;

	std::vector<std::vector<cv::Point2f>> _imagePoints;
	cv::Mat _cameraMatrix, _distCoeffs;
	cv::Size _imageSize;

	cv::Mat _vignetting;
	bool _red;
private:
	// 0::suc 1::fail
	int readStringList(const std::string& filename, std::vector<std::string>& l);
public:
	SingleCalibrater() {
		_xmlListFile = "";
		_cornerWidth = 0;
		_cornerHeight = 0;
		_squareSize = 0;
		_listFile.resize(0);
		_red = false;
	};
	~SingleCalibrater() {};

	int SetVignettingMat(std::string vigMat);

	int SetRedSpot(bool redSpot);

	int SetImageList(std::string xmlListFile);

	int SetImageList(std::vector<std::string> listFile);

	int SetBoardSize(int cornerWidth, int cornerHeight, double squareSize);

	int Calibrate(cv::Mat &cameraMatrix, cv::Mat &distCoeffs, bool renameFailFile = false);

	int SaveParams(std::string file);

	// 1:found 0:not found -1:timeout
	static int findChessboardCornersTimeout
	(cv::Mat &img, cv::Size &boardSize, std::vector<cv::Point2f> &out_pointList, int flag, int timeoutMs,
		const cv::Mat &_vignetting = cv::Mat(), bool findRedROI = false);
};

#endif //__CAMERA_CALIBRATION_SINGLE__