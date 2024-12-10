//
// Created by gao on 24-11-10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_qui_face_identify_widget.h" resolved
#include <QVariant>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>

#include "qui_face_identify_widget.h"
#include "ui_qui_face_identify_widget.h"


CFaceIdentifyWidget::CFaceIdentifyWidget(QWidget *parent) :
    QWidget(parent), _ui(new Ui::CFaceIdentifyWidget) {
    _ui->setupUi(this);
    _initStyle();
    _initAnimation();
    connect(_ui->centerBtn,&QPushButton::clicked,this,&CFaceIdentifyWidget::onCenterBtnClicked);
    connect(_animation,&QPropertyAnimation::valueChanged,[this](QVariant value) {
        _effect->setBlurRadius(value.toInt());
    });
}

CFaceIdentifyWidget::~CFaceIdentifyWidget() {
    delete _ui;
    if(_cameraThread.joinable()) {
        _cameraThread.join();
    }
}

/**
 * 验证信息回调函数
 * @param result 验证结果
 */
void CFaceIdentifyWidget::NotifyCallBack(bool result) {

}

void CFaceIdentifyWidget::hideEvent(QHideEvent *event) {
    _handleIdentifyStatus(IdentifyStatus::IS_CAMERACLOSE);
    QWidget::hideEvent(event);
}


/**
 * 用于处理各个验证过程所出现的状态
 * @param status 验证状态
 */
void CFaceIdentifyWidget::_handleIdentifyStatus(IdentifyStatus status) {

    switch (status)
    {
        // 打开摄像头
        case IdentifyStatus::IS_CAMERAOPEN: {
            //注册验证通知回调
            _camera.RegisterNotify(std::bind(&CFaceIdentifyWidget::NotifyCallBack,this,std::placeholders::_1));
            _effect->setColor(QColor("#000000"));
            _animation->setLoopCount(-1);
            _animation->start();
            _ui->centerBtn->setText("OFF");
            _ui->centerBtn->setStyleSheet(_centerBtnSsOFF);
            _cameraThread = std::thread(&CFaceIdentifyWidget::_labelShowThread,this);
            break;
        }
        // 关闭摄像头
        case IdentifyStatus::IS_CAMERACLOSE: {
            if(_camera.CloseCamera()) {
                if(_cameraThread.joinable()) {
                    _cameraThread.join();
                }
                _animation->stop();
                _ui->centerBtn->setText("ON");
                QImage img;
                _ui->centerBtn->setStyleSheet(_centerBtnSsON);
                if(CFaceIdentify::BlurQimage(_ui->label->pixmap().toImage(),img,30)) {
                    _ui->label->setPixmap(QPixmap::fromImage(img));
                }
            }
            break;
        }
        // 摄像头打开失败
        case IdentifyStatus::IS_CAMERAUNOPEN: {
            QImage img(":/imgs/resources/ban.png");
            _ui->label->setPixmap(QPixmap::fromImage(img.scaled(_ui->label->GetRadius(),_ui->label->GetRadius())));
            _ui->label2->setText("摄像头打开失败");
            break;
        }
        default:
            break;
    }
    _identifyStatus = status;
}

/**
 * 初始化样式
 */
void CFaceIdentifyWidget::_initStyle() {
    _ui->label->setPixmap(QPixmap(":/imgs/resources/background1.png"));
    //初始化阴影
    _effect = new QGraphicsDropShadowEffect(this);
    _effect->setOffset(0, 0);
    _ui->label->setGraphicsEffect(_effect);
    //中心按钮样式
    _centerBtnSsON = ":!hover{border-radius:5px;border:2px solid black;background-color:white;color:black;}"
        ":hover{border-radius:5px;border:2px solid black;background-color:black;color:white;}";
    _centerBtnSsOFF = ":!hover{border-radius:5px;border:2px solid black;background-color:black;color:white;}"
        ":hover{border-radius:5px;border:2px solid black;background-color:white;color:black;}";
}

/**
 * 初始化动画
 */
void CFaceIdentifyWidget::_initAnimation() {
    _animation = new QPropertyAnimation(this);
    _animation->setKeyValueAt(0, 0);
    _animation->setKeyValueAt(0.5, 80);
    _animation->setKeyValueAt(1, 0);
    _animation->setDuration(2000);
    _animation->setTargetObject(_ui->label);
}

/**
 * 中心按钮点击事件
 */
void CFaceIdentifyWidget::onCenterBtnClicked() {
    if(!_camera.StatusCamera())
    {
        if(_camera.OpenCamera("/dev/video0"))//打开摄像头
        {
            _handleIdentifyStatus(IdentifyStatus::IS_CAMERAOPEN);
        }
        else
        {
            _handleIdentifyStatus(IdentifyStatus::IS_CAMERAUNOPEN);
        }
    }
    else
    {
        _handleIdentifyStatus(IdentifyStatus::IS_CAMERACLOSE);
    }
}

/**
 * label设置线程
 */
void CFaceIdentifyWidget::_labelShowThread() {
    while(_camera.StatusCamera()) {
        QImage img;
        if(_camera.GetFrame(img)) {
            QPixmap pixmap = QPixmap::fromImage(img);
            _ui->label->setPixmap(pixmap);
        }
        std::this_thread::sleep_for (milliseconds(40));
    }
}

