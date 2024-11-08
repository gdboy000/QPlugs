//
// Created by gao on 24-11-3.
//

#ifndef FACE_IDENTIFY_H
#define FACE_IDENTIFY_H

#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <QWidget>
#include <condition_variable>
#include <opencv2/face.hpp>
#include <opencv2/dnn.hpp>
#include "camera_control.h"

using namespace cv::dnn;
using namespace cv::face;
using namespace std::chrono;

/**
 * 该类的验证是计时验证，暂定一秒进行两次验证
 */
class CFaceIdentify :public CCameraControl{
public:
    CFaceIdentify();
    CFaceIdentify(int);//初始化模块
    CFaceIdentify(const std::string&);
    ~CFaceIdentify();
    bool GetFrame(QImage &) override;
    bool OpenCamera(int) override;
    bool OpenCamera(const std::string &) override;
    bool CloseCamera() override;
    void RegisterNotify(std::function<void(bool)>);
private:
    std::thread _identifyThread;
    const std::string _prototxt_path = "../test/deploy.prototxt";
    const std::string _model_path = "../test/res10_300x300_ssd_iter_140000.caffemodel";
    cv::Ptr<LBPHFaceRecognizer> _recongnizer;
    Net _net;
    std::mutex _mtx;
    std::condition_variable _cv;
    cv::Mat _transmitFrame;
    std::atomic<bool> _threadBreak = true;
    std::atomic<bool> _identifyResult = false;
    time_point<high_resolution_clock> _startTime, _endTime;
    bool _model = false;
    std::function<void(bool)> _notifyFunc;
    void _initModel();
    cv::Mat _safeClone(const cv::Mat&);
    void _identify();

};



#endif //FACE_IDENTIFY_H
