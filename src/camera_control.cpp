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


bool CCameraControl::OpenCamera(int index)
{
    if(_cap.isOpened()) {
        _cap.release();
        _isOpen = false;
    }
    _cap = cv::VideoCapture(index,cv::CAP_V4L2);
    if(_cap.isOpened()) {
        _isOpen = true;
        _videoWidth = _cap.get(cv::CAP_PROP_FRAME_WIDTH);
        _videoHeight = _cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        int x,y;
        if(_videoWidth >= _videoHeight) {
            x = (_videoWidth - _videoHeight)/2;
            _rect = cv::Rect(x,0,_videoHeight,_videoHeight);
        }
        else {
            y = (_videoHeight - _videoWidth)/2;
            _rect = cv::Rect(0,y,_videoWidth,_videoWidth);
        }
        return true;
    }
    return false;
}

bool CCameraControl::OpenCamera(const std::string& filename)
{
    if(_cap.isOpened()) {
        _cap.release();
        _isOpen = false;
    }
    _cap = cv::VideoCapture(filename,cv::CAP_V4L2);
    if(_cap.isOpened()) {
        _isOpen = true;
        _videoWidth = _cap.get(cv::CAP_PROP_FRAME_WIDTH);
        _videoHeight = _cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        int x,y;
        if(_videoWidth >= _videoHeight) {
            x = (_videoWidth - _videoHeight)/2;
            _rect = cv::Rect(x,0,_videoHeight,_videoHeight);
        }
        else {
            y = (_videoHeight - _videoWidth)/2;
            _rect = cv::Rect(0,y,_videoWidth,_videoWidth);
        }
        return true;
    }
    return false;
}

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
 * 1.判断摄像头是否打开
 * 2.获取cv::Mat frame
 * 3.resize到 传参正确图像 --不正确，应当由调用者来修改尺寸，减少该类的冗余
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
    else
    {
        _cap >> _frame;
        cv::Mat _frame_resize;
        if(!_frame.empty()) {
            if(_tailorSize.width>0 && _tailorSize.height>0) {

                cv::resize(_frame(_rect),_frame_resize,_tailorSize,cv::INTER_LINEAR);
            }
            else {
                _frame(_rect).copyTo(_frame_resize);
            }
            return _cvMat2QImage(_frame_resize,frame);
        }
        return false;
    }
}

bool CCameraControl::_cvMat2QImage(const cv::Mat& mat,QImage &img)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (mat.type() == CV_8UC1)
    {
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
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        img = image.rgbSwapped();
        return true;
    }
    else if (mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        img = QImage(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return true;
    }
    else
    {
        return false;
    }
}

bool CCameraControl::SetTailorSize(int w, int h) {
    if(w>0 && h >0) {
        _tailorSize = cv::Size(w,h);
        return true;
    }
    return false;
}
