#include <opencv2/imgproc.hpp>
#include <iostream>
#include "face_identify.h"
#include "qui_debug_log_window.h"


//规范Rect范围函数
void limit_Rect(cv::Rect& rect,const cv::Mat &frame) {
    if(rect.x < 0) rect.x = 0;
    if(rect.y < 0) rect.y = 0;
    if(rect.x + rect.width > frame.cols) rect.width = frame.cols - rect.x;
    if(rect.y + rect.height > frame.rows) rect.height = frame.rows - rect.y;
}


CFaceIdentify::CFaceIdentify(){
    //初始化模型配置文件
    _initModel();
}

CFaceIdentify::CFaceIdentify(int index) :
    CCameraControl(index){
    _initModel();
    _startTime = high_resolution_clock::now();
    if(StatusCamera()) {
        _identifyThread = std::thread(&CFaceIdentify::_identify,this);
    }
}

CFaceIdentify::CFaceIdentify(const std::string &filename) :
    CCameraControl(filename){
    _initModel();
    _startTime = high_resolution_clock::now();
    if(StatusCamera()) {
        _identifyThread = std::thread(&CFaceIdentify::_identify,this);
    }
}

CFaceIdentify::~CFaceIdentify() {
    _threadBreak = false;
    if(_identifyThread.joinable()) {
        _identifyThread.join();
    }
}

/**
 * 打开摄像头
 * @param frame 输出一帧
 * @return 输出是否有效
 */
bool CFaceIdentify::GetFrame(QImage &frame) {
    cv::Mat _frame;
    if(CCameraControl::GetFrame(_frame)) {
        //传递_frame给线程
        if(duration<double,std::milli>(high_resolution_clock::now() - _startTime).count() > 500) {
            _transmitFrame = _safeClone(_frame);
            //唤醒线程
            _cv.notify_one();
            //检查检验结果
            if(_identifyResult) {
                _notifyFunc(true);
            }
        }
        return CvMat2QImage(_frame,frame);
    }
    return false;
}

/**
 * 打开摄像头，通过索引号
 * @param index 索引号
 * @return 打开是否成功
 */
bool CFaceIdentify::OpenCamera(int index) {
    if(!_model) {
        return false;
    }
    bool open = CCameraControl::OpenCamera(index);
    if(open) {
        _startTime = high_resolution_clock::now();
        //打开线程
        _identifyThread = std::thread(&CFaceIdentify::_identify,this);
    }
    return open;
}

/**
 * 打开摄像头通过设备名称
 * @param filename 设备名称，也就是文件名
 * @return 打开是否成功
 */
bool CFaceIdentify::OpenCamera(const std::string &filename) {
    if(!_model) {
        return false;
    }
    bool open = CCameraControl::OpenCamera(filename);
    if(open) {
        _startTime = high_resolution_clock::now();
        if(_identifyThread.joinable()) {
            _identifyThread.join();
        }
        //打开线程
        _identifyThread = std::thread(&CFaceIdentify::_identify,this);
    }
    return open;
}

/**
 * 关闭摄像头
 * @return 关闭摄像头之前状态
 */
bool CFaceIdentify::CloseCamera() {
    _threadBreak = false;
    return CCameraControl::CloseCamera();
}

/**
 * 注册验证结果通知函数
 * @param func 通知函数
 */
void CFaceIdentify::RegisterNotify(std::function<void(bool)> func) {
    _notifyFunc = func;
}

/**
 * 初始化模型配置
 */
void CFaceIdentify::_initModel() {
    try {
        _model = true;
        _recongnizer = LBPHFaceRecognizer::create();
        _recongnizer->read("../test/trainer.yml");
        _net = readNetFromCaffe(_prototxt_path, _model_path);
    }catch (cv::Exception e) {
        SHOWERROR(e.what());
        _model  = false;
    }
}

/**
 * 线程安全frane克隆一帧
 * @param frame 被克隆的一帧
 * @return 克隆好的一帧
 */
cv::Mat CFaceIdentify::_safeClone(const cv::Mat &frame) {
    std::lock_guard lck(_mtx);
    cv::Mat cloneFrame = frame.clone();
    return std::move(cloneFrame);
}

/**
 * 人脸验证线程
 */
void CFaceIdentify::_identify() {
    while(_threadBreak) {
        try{

            //上锁 clone frame
            std::unique_lock lck(_mtx);
            _cv.wait(lck);
            cv::Mat frame = _transmitFrame.clone();
            lck.unlock();

            //归一化
            cv::Mat blob = blobFromImage(frame, 1, cv::Size(300, 300), cv::Scalar(104, 117, 123));
            //获取脸部位置
            _net.setInput(blob);
            cv::Mat detections = _net.forward();
            cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());
            cv::Mat gray;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            for (int i = 0; i < detectionMat.rows; i++)
            {
                if (detectionMat.at<float>(i, 2) >= 0.5)
                {

                    int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                    int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                    int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                    int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
                    cv::Rect object((int)xLeftBottom, (int)yLeftBottom,
                        (int)(xRightTop - xLeftBottom),
                        (int)(yRightTop - yLeftBottom));

                    rectangle(frame, object, cv::Scalar(0, 255, 0));
                    double n;
                    int label = -1;
                    limit_Rect(object,gray);
                    _recongnizer->predict(gray(object), label, n);
                    if (n < 83)
                    {
                        _identifyResult = true;
                        return;
                    }
                }
            }
        }catch (cv::Exception e) {
            std::cout<<"ERROR: "<<e.what()<<'\n';
        }
    }

}





