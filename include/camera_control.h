#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#include <opencv2/highgui.hpp>
#include <map>
#include <string>
#include <optional>
#include <QImage>

template<class T>
typename std::optional<T> Ret;

class CCameraControl
{
public:
    CCameraControl();
    ~CCameraControl();
    explicit CCameraControl(int index);
    explicit CCameraControl(const std::string& filename);
    std::map<int,std::string> GetCameraDevicesMessage();//设备信息返回
    bool OpenCamera(int);//设备选择并打开摄像头
    bool OpenCamera(const std::string&);//设备选择并打开摄像头
    bool CloseCamera();//关闭摄像头
    bool StatusCamera() const;//摄像头状态
    bool SetTailorSize(int w,int h);
    bool GetFrame(QImage &frame);
    bool GetFrame(cv::Mat &frame);
private:
    cv::VideoCapture _cap;
    int _videoWidth = 0;
    int _videoHeight = 0;
    cv::Rect _rect = {0,0,0,0};
    cv::Size _tailorSize = {0,0};
    bool _isOpen = false;
    static bool _cvMat2QImage(const cv::Mat& mat,QImage &img);
};

#endif // CAMERA_CONTROL_H
