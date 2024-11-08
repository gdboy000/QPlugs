#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#include <opencv2/highgui.hpp>
#include <string>
#include <QImage>

/**
 * 摄像头控制基类
 */
class CCameraControl
{
public:
    CCameraControl();
    virtual ~CCameraControl();
    explicit CCameraControl(int index);
    explicit CCameraControl(const std::string& filename);
    //std::map<int,std::string> GetCameraDevicesMessage();//设备信息返回
    virtual bool OpenCamera(int);//设备选择并打开摄像头
    virtual bool OpenCamera(const std::string&);//设备选择并打开摄像头
    virtual bool CloseCamera();//关闭摄像头
    virtual bool StatusCamera() const;//摄像头状态
    virtual bool GetFrame(QImage &frame);//获取一帧QImage
    virtual bool GetFrame(cv::Mat &frame);//获取一帧cv::Mat
    static bool CvMat2QImage(const cv::Mat& mat,QImage &img);//cv::Mat转QImage
private:
    cv::VideoCapture _cap;//摄像头对象
    int _videoWidth = 0;//unused
    int _videoHeight = 0;//unused
    cv::Rect _rect = {0,0,0,0};//unused
    cv::Size _tailorSize = {0,0};//unused
    bool _isOpen = false;//摄像头是否打开

};


#endif // CAMERA_CONTROL_H
