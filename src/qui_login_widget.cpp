#include <QPushButton>
#include <QDebug>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QScreen>
#include <iostream>
#include <chrono>
#include "qui_login_widget.h"
#include "ui_qui_login_widget.h"
#include "q_change_label.h"
#include "qui_debug_log_window.h"


CLoginWidget::CLoginWidget(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::CLoginWidget)
{
    _ui->setupUi(this);
    //缓存中心按钮样式
    _centerBtnSs = _ui->centerBtn->styleSheet();
    //设置窗口居中
    const QScreen *pScreen = QGuiApplication::primaryScreen();
    this->move((pScreen->geometry().width() - this->width()) / 2,(pScreen->geometry().height() - this->height()) / 2);
    //设置窗口无边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //设置label默认图片显示
    _ui->label->setPixmap(QPixmap(":/imgs/resources/dog.jepg"));
    //label初始化阴影设置
    _effectForOpen = new QGraphicsDropShadowEffect(this);
    _effectForOpen->setOffset(0, 0);
    _effectForOpen->setBlurRadius(0);
    _ui->label->setGraphicsEffect(_effectForOpen);
    //设置layout中控件水平居中
    _ui->widget2->layout()->setAlignment(Qt::AlignCenter);
    _ui->widget3->layout()->setAlignment(Qt::AlignCenter);
    //默认隐藏验证码界面
    _ui->widget3->hide();
    //设置默认模式
    _identifyMode = IdentifyMode::IM_FACE;
    //剩余初始化，一些成员需初始化，因此保证该部分在最后初始化
    _initAnimation();
    _initConnect();
}

CLoginWidget::~CLoginWidget()
{
    qDebug()<<"~CLoginWidget()";
    delete _ui;
    delete _effectForOpen;
    delete _animationForOpen;
    delete _animationForOpenError;
    if(nullptr != _mWidget) {
        delete _mWidget;
    }
}

/**
 * 初始化所有信号与槽的连接
 * 动画-label阴影
 * 登录结果-结果处理
 */
void CLoginWidget::_initConnect()
{
    connect(_animationForOpen, &QPropertyAnimation::valueChanged, this, &CLoginWidget::shadowChanged);
    connect(_animationForOpenError, &QPropertyAnimation::valueChanged, this, &CLoginWidget::shadowChanged);
    connect(_animationForChangeMode, &QPropertyAnimation::valueChanged, this, &CLoginWidget::identifyModeChange);
    connect(this,&CLoginWidget::changeMessageText,this, &CLoginWidget::updateMessageText);
    connect(this,&CLoginWidget::identifyResult, this, &CLoginWidget::_result);
    connect(_ui->codeLoginBtn,&QPushButton::clicked,this,&CLoginWidget::codeIdentify);
    connect(_ui->closeBtn,&QPushButton::clicked,this,&CLoginWidget::onCloseBtnClicked);
    connect(_ui->settingBtn,&QPushButton::clicked,this,&CLoginWidget::onSettingBtnClicked);
    connect(_ui->centerBtn,&QPushButton::clicked,this,&CLoginWidget::onCenterBtnClicked);
    connect(_ui->backBtn,&QPushButton::clicked,this,&CLoginWidget::onBackBtnClicked);
}

/**
 * @brief 动画初始化
 */
void CLoginWidget::_initAnimation()
{
    _animationForOpen = new QPropertyAnimation(this);
    _animationForOpen->setKeyValueAt(0, 0);
    _animationForOpen->setKeyValueAt(0.5, 80);
    _animationForOpen->setKeyValueAt(1, 0);
    _animationForOpen->setDuration(2000);
    _animationForOpen->setLoopCount(-1);
    _animationForOpen->setTargetObject(_ui->label);

    _animationForOpenError = new QPropertyAnimation(this);
    _animationForOpenError->setKeyValueAt(0, 0);
    _animationForOpenError->setKeyValueAt(0.5, 80);
    _animationForOpenError->setKeyValueAt(1, 0);
    _animationForOpenError->setDuration(1000);
    _animationForOpenError->setLoopCount(1);
    _animationForOpenError->setTargetObject(_ui->messageLabel);

    _animationForChangeMode = new QPropertyAnimation(this);
    _animationForChangeMode->setKeyValueAt(0, 0);
    _animationForChangeMode->setKeyValueAt(1, 30);
    _animationForChangeMode->setDuration(1000);
    _animationForChangeMode->setLoopCount(1);
    _animationForChangeMode->setTargetObject(_ui->widget2);
}

/**
 * 窗口关闭按钮槽函数
 */
void CLoginWidget::onCloseBtnClicked()
{
    this->close();
}

/**
 * 窗口设置按钮槽函数
 */
void CLoginWidget::onSettingBtnClicked()
{

}

/**
 * 心按钮点击槽函数
 */
void CLoginWidget::onCenterBtnClicked()
{
    _cameraOpen = _camera.StatusCamera();//获取摄像头状态
    if(!_cameraOpen)
    {
        if(_camera.OpenCamera("/dev/video0"))//打开摄像头
        {
            _handleIdentifyStatus(IdentifyStatus::CAMERA_OPEN);
        }
        else
        {
            _handleIdentifyStatus(IdentifyStatus::CAMERA_UNOPEN);
        }
    }
    else
    {
        _handleIdentifyStatus(IdentifyStatus::CAMERA_CLOSE);
    }
}

/**
 * 窗口回滚按钮槽函数
 */
void CLoginWidget::onBackBtnClicked()
{
    _animationForChangeMode->start();
    if(_identifyMode == IdentifyMode::IM_FACE && _identifyStatus == IdentifyStatus::CAMERA_OPEN) {
        _handleIdentifyStatus(IdentifyStatus::CAMERA_CLOSE);
    }
}

/**
 * 动画影响阴影槽函数
 * @param value 动画槽，需要转int
 */
void CLoginWidget::shadowChanged(QVariant value)
{
    _effectForOpen->setBlurRadius(value.toInt());
}

/**
 * 验证窗口动画切换
 * @param value 动画槽，需要转int
 */
void CLoginWidget::identifyModeChange(QVariant value)
{
    static int h = this->height() - _ui->widget1->height();
    const int _value = value.toInt();
    //当前模式为人脸识别
    if(IdentifyMode::IM_FACE == _identifyMode) {
        if(30 == _value) {
            //切换为验证码模式，隐藏人脸识别界面
            _identifyMode = IdentifyMode::IM_CODE;
            _ui->widget2->hide();
            return;
        }
        if(1 == _value) {
            //必须提前show，要不widget1会下降
            _ui->widget3->show();
            _ui->widget3->setMaximumHeight(h);
        }
        if(_value > 0) {
            //通过设置最大高度，改变layout中的窗口大小
            _ui->widget2->setMaximumHeight(h/_value);
        }
    }
    //当前模式为验证码
    else if(IdentifyMode::IM_CODE == _identifyMode) {
        if(30 == _value) {
            //切换为人脸识别模式，隐藏验证码界面
            _identifyMode = IdentifyMode::IM_FACE;
            _ui->widget3->hide();
            return;
        }
        if(1 == _value) {
            //必须提前show，要不widget1会下降
            _ui->widget2->show();
            _ui->widget2->setMaximumHeight(h);
        }
        if(_value > 0) {
            //通过设置最大高度，改变layout中的窗口大小
            _ui->widget3->setMaximumHeight(h/_value);
        }
    }
}

/**
 * 更新验证返回消息
 * @param text 消息
 */
void CLoginWidget::updateMessageText(QString text) {
    _ui->messageLabel->setText(QString("<span style=\"color: red; text-decoration: underline; text-decoration-color: red;\">%1</span>").arg(text));
}

/**
 * 验证结果处理函数
 * @param result    验证结果
 * @param status    验证状态
 */
void CLoginWidget::_result(bool result,IdentifyStatus status) {
    if(result) {
        switch (_identifyMode) {
            case IdentifyMode::IM_FACE: {
                QString successStyle = "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #CCFF99 ,stop:0.5 #00FF00 ,stop:1 #77FF00 );";
                _camera.CloseCamera();
                _ui->centerBtn->setText("验证成功");
                _ui->centerBtn->setStyleSheet(_centerBtnSs+successStyle);
                break;
            }
            case IdentifyMode::IM_CODE: {
                _ui->label2->setText("<span style=\"color: green;\">登陆成功,跳转...</span>");
                _ui->label2->repaint();
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        _mWidget = new CFunctionWidget();
        _mWidget->show();
        this->close();
    }
    else {
        _handleIdentifyStatus(status);
    }
}

/**
 * 验证码验证判断
 */
void CLoginWidget::codeIdentify() {
    QString code = _ui->lineEdit->text();
    if(code == "123456") {
        _result(true);
    }
    else {
       _result(false,IdentifyStatus::CODE_ERROR);
    }
}


/**
 * 中心按钮文字样式改变函数
 * @param on  点击之前按钮状态
 */
void CLoginWidget::_updateCenterStyle(bool on,std::string message)
{
    //按钮处于关闭样式
    QString offStyle = "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #F5DEB3 ,stop:0.5 #FFDEAD ,stop:1 #FFE4C4 );";
    //按钮处于点击样式
    QString onStyle = "background-color:red;";
    if(on)
    {
        //设置打开文本
        _ui->centerBtn->setText(message.c_str());
        _ui->centerBtn->setStyleSheet(_centerBtnSs+offStyle);
    }
    else
    {
        //设置等待打开文本
        _ui->centerBtn->setText(message.c_str());
        _ui->centerBtn->setStyleSheet(_centerBtnSs+onStyle);
    }
}

/**
 * 处理摄像头响应状态
 * @param status 摄像头响应状态
 */
void CLoginWidget::_handleIdentifyStatus(IdentifyStatus status)
{
    //按钮处于关闭样式
    QString offStyle = "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #F5DEB3 ,stop:0.5 #FFDEAD ,stop:1 #FFE4C4 );";
    //按钮处于点击样式
    QString onStyle = "background-color:red;";
    _identifyStatus = status;
    switch (status)
    {
        case IdentifyStatus::CAMERA_OPEN: {
            SHOWMESSAGE("打开摄像头成功");
            _camera.RegisterNotify(std::bind(&CLoginWidget::notifyIdentify,this,std::placeholders::_1));
            _effectForOpen->setColor(QColor("#CC9966"));
            _animationForOpen->start();
            _ui->centerBtn->setText("结束");
            _ui->centerBtn->setStyleSheet(_centerBtnSs+onStyle);
            _cameraThread = std::thread(&CLoginWidget::_labelShowThread,this);
            break;
        }
        case IdentifyStatus::CAMERA_CLOSE: {
            SHOWMESSAGE("关闭摄像头");
            _camera.CloseCamera();
            if(_cameraThread.joinable()) {
                _cameraThread.join();
            }
            _animationForOpen->stop();
            _ui->centerBtn->setText("开始");
            _ui->centerBtn->setStyleSheet(_centerBtnSs+offStyle);
            break;
        }
        case IdentifyStatus::CAMERA_UNOPEN: {
            SHOWMESSAGE("打开摄像头失败");
            _effectForOpen->setColor(QColor("red"));
            _animationForOpenError->start();
            _ui->messageLabel->setText("<span style=\"color: red; text-decoration: underline; text-decoration-color: red;\">摄像头打开失败</span>");
            break;
        }
        case IdentifyStatus::CODE_ERROR:{
            _ui->label2->setText("<span style=\"color: red;\">code error</span>");
            _ui->label2->repaint();
            break;
        }
        default:
            break;

    }

}

/**
 * 线程：获取CCameraControl对象返回得QImage对象,转换成QPixmap，放入label中显示
 */
void CLoginWidget::_labelShowThread() {
    _startTime = system_clock::now();
    while(_camera.StatusCamera()) {
        QImage img;
        if(_camera.GetFrame(img)) {
            QPixmap pixmap = QPixmap::fromImage(img);
            _ui->label->setPixmap(pixmap);
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(40));
        //间隔输出识别状况
        if(duration<double>(system_clock::now() - _startTime).count() > 10) {
            emit changeMessageText("识别失败...");
        }
        if(duration<double>(system_clock::now() - _startTime).count() > 12) {
            emit changeMessageText(" ");
            _startTime = system_clock::now();
        }

    }
    SHOWMESSAGE("验证完成！！");
}

void CLoginWidget::notifyIdentify(bool result)
{
    if(result)
        emit identifyResult(true);
}





