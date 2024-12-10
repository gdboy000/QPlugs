#include "camera_control.h"
#include <QDebug>
#include <iostream>
#include <opencv2/opencv.hpp>

CCameraControl::CCameraControl() {

}

CCameraControl::~CCameraControl() {
    if(_cap.isOpened()) {
        _cap.release();
        _isOpen = false;
    }
}

CCameraControl::CCameraControl(int index) {
    _cap = cv::VideoCapture(index,cv::CAP_V4L2);
    if(_cap.isOpened()) {
        _isOpen = true;
    }
}

CCameraControl::CCameraControl(const std::string& filename) {
    _cap = cv::VideoCapture(filename,cv::CAP_V4L2);
    if(_cap.isOpened()) {
        _isOpen = true;
    }
}

/**
 * 打开摄像头
 * @param index 摄像头设备索引号
 * @return 打开是否成功
 */
bool CCameraControl::OpenCamera(int index)
{
    if(_cap.isOpened()) {
        _cap.release();
        _isOpen = false;
    }
    _cap = cv::VideoCapture(index,cv::CAP_V4L2);
    if(_cap.isOpened()) {
        _isOpen = true;
        // _videoWidth = _cap.get(cv::CAP_PROP_FRAME_WIDTH);
        // _videoHeight = _cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        // int x,y;
        // if(_videoWidth >= _videoHeight) {
        //     x = (_videoWidth - _videoHeight)/2;
        //     _rect = cv::Rect(x,0,_videoHeight,_videoHeight);
        // }
        // else {
        //     y = (_videoHeight - _videoWidth)/2;
        //     _rect = cv::Rect(0,y,_videoWidth,_videoWidth);
        // }
        return true;
    }
    return false;
}

/**
 * 打开摄像头
 * @param filename 摄像头设备名
 * @return 打开是否成功
 */
bool CCameraControl::OpenCamera(const std::string& filename)
{
    if(_cap.isOpened()) {
        _cap.release();
        _isOpen = false;
    }
    _cap = cv::VideoCapture(filename,cv::CAP_V4L2);
    if(_cap.isOpened()) {
        _isOpen = true;
        // _videoWidth = _cap.get(cv::CAP_PROP_FRAME_WIDTH);
        // _videoHeight = _cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        // int x,y;
        // if(_videoWidth >= _videoHeight) {
        //     x = (_videoWidth - _videoHeight)/2;
        //     _rect = cv::Rect(x,0,_videoHeight,_videoHeight);
        // }
        // else {
        //     y = (_videoHeight - _videoWidth)/2;
        //     _rect = cv::Rect(0,y,_videoWidth,_videoWidth);
        // }
        return true;
    }
    return false;
}

/**
 * 关闭摄像头
 * @return 关闭之前摄像头状态
 */
bool CCameraControl::CloseCamera()
{
    if(_cap.isOpened())
    {
        _cap.release();
        _isOpen = false;
        return true;
    }
    else
    {
        return false;
    }

}

/**
 * 返回摄像头状态
 * @return 摄像头状态
 */
bool CCameraControl::StatusCamera() const
{
    if(_cap.isOpened())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * 获取一帧
 * @param frame (QImage)传出参数
 * @return 函数执行是否成功
 */
bool CCameraControl::GetFrame(QImage &frame)
{
    cv::Mat _frame;
    if(!_cap.isOpened())
    {
        return false;
    }
    _cap >> _frame;
    cv::Mat _frame_resize;
    if(!_frame.empty()) {
        // _frame(_rect).copyTo(_frame_resize);
        // return CvMat2QImage(_frame_resize,frame);
        return CvMat2QImage(_frame,frame);
    }
    return false;
}

/**
 * 获取一帧
 * @param frame 传出一帧cv::Mat
 * @return 获取是否成功
 */
bool CCameraControl::GetFrame(cv::Mat &frame) {
    if(!_cap.isOpened())
    {
        return false;
    }
    _cap >> frame;
    cv::Mat _frame_resize;
    if(!frame.empty()) {
        return true;
    }
    return false;
}

/**
 * 静态函数 cv::Mat 转 QImage
 * @param mat cv::Mat一帧
 * @param img QImage 一帧
 * @return 转换是否成功
 */
bool CCameraControl::CvMat2QImage(const cv::Mat& mat,QImage &img)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (mat.type() == CV_8UC1)
    {
        qDebug()<<"8-bits unsigned, NO. OF CHANNELS = 1";
        img = QImage(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        img.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            img.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar* pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar* pDest = img.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return true;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if (mat.type() == CV_8UC3)
    {
        qDebug()<<"8-bits unsigned, NO. OF CHANNELS = 3";
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        img = image.rgbSwapped();
        return true;
    }
    else if (mat.type() == CV_8UC4)
    {
        qDebug()<<"8-bits unsigned, NO. OF CHANNELS = 4";
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        img = QImage(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return true;
    }
    else
    {
        qDebug()<<"ERROR: Mat could not be converted to QImage.";
        return false;
    }
}

bool CCameraControl::QImage2CvMat(const QImage &img, cv::Mat &mat) {
    try {
        qDebug()<<"format: "<<img.format();
        QImage image = img.convertToFormat(QImage::Format_ARGB32);
        mat = cv::Mat(image.height(),image.width(),CV_8UC4,image.bits(),image.bytesPerLine());
        return true;
    }catch(cv::Exception e) {
        std::cout<<"error: "<<e.what()<<'\n';
        return false;
    }

}


bool CCameraControl::BlurQimage(const QImage &src, QImage &dst,int radius) {
    cv::Mat frame;
    if(QImage2CvMat(src, frame)) {
        if(radius == 0 || radius % 2!=1) {
            radius++;
        }
        cv::Mat frame2;
        GaussianBlur(frame, frame2, cv::Size(radius,radius), 0);
        if(CvMat2QImage(frame2, dst)) {
            return true;
        }

    }
    return false;
}

// bool CCameraControl::SetTailorSize(int w, int h) {
//     if(w>0 && h >0) {
//         _tailorSize = cv::Size(w,h);
//         return true;
//     }
//     return false;
// }
