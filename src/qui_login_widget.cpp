#include "qui_login_widget.h"
#include "ui_qui_login_widget.h"
#include "q_change_label.h"
#include <QApplication>
#include <QIcon>
#include <QPushButton>
#include <QDebug>
#include <QVBoxLayout>
#include <QPainter>
#include <QBitmap>
#include <QErrorMessage>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QScreen>
#include <iostream>

#include "qui_debug_log_window.h"

/**
 * @brief
 * @param parent
 */
CLoginWidget::CLoginWidget(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::CLoginWidget)
{
    _ui->setupUi(this);
    _centerBtnSs = _ui->centerBtn->styleSheet();
    //设置窗口居中
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    int x = (primaryScreen->geometry().width() - this->width()) / 2;
    int y = (primaryScreen->geometry().height() - this->height()) / 2;
    this->move(x,y);
    //设置窗口flag
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //设置label默认图片显示
    QPixmap pixmap(":/imgs/resources/dog.jepg");
    _ui->label->setPixmap(pixmap);
    //label打开成功阴影设置
    _effectForOpen = new QGraphicsDropShadowEffect(this);
    _effectForOpen->setOffset(0, 0);
    _effectForOpen->setBlurRadius(0);
    _ui->label->setGraphicsEffect(_effectForOpen);

    _camera.SetTailorSize(_ui->label->width(), _ui->label->height());

    _initAnimation();
    _initConnect();
}

CLoginWidget::~CLoginWidget()
{
    delete _ui;
    delete _widget;
    delete _box;
    delete _effectForOpen;
    delete _animationForOpen;
    delete _animationForOpenError;
    delete _td;
    delete _rock;
}

/**
 * @brief 初始化所有信号与槽的连接
 * 动画-label阴影
 * 登录结果-结果处理
 */
void CLoginWidget::_initConnect()
{
    connect(_animationForOpen, &QPropertyAnimation::valueChanged, this, &CLoginWidget::shadowChanged);
    connect(_animationForOpenError, &QPropertyAnimation::valueChanged, this, &CLoginWidget::shadowChanged);
    connect(this,&CLoginWidget::login, this, &CLoginWidget::takeResult);
    //connect(this, &CLoginWidget::waitForBtnEnter, this, &CLoginWidget::updateCenterMessage);
    connect(this, &CLoginWidget::changeLabel, this, &CLoginWidget::receiveVideo);
}

/**
 * @brief 动画初始化
 */
void CLoginWidget::_initAnimation()
{
    _animationForOpen = new QPropertyAnimation(this);
    _animationForOpen->setKeyValueAt(0, 0);
    _animationForOpen->setKeyValueAt(0.5, 120);
    _animationForOpen->setKeyValueAt(1, 0);
    _animationForOpen->setDuration(2000);
    _animationForOpen->setLoopCount(-1);
    _animationForOpen->setTargetObject(_ui->label);

    _animationForOpenError = new QPropertyAnimation(this);
    _animationForOpenError->setKeyValueAt(0, 0);
    _animationForOpenError->setKeyValueAt(0.5, 120);
    _animationForOpenError->setKeyValueAt(1, 0);
    _animationForOpenError->setDuration(1000);
    _animationForOpenError->setLoopCount(1);
    _animationForOpenError->setTargetObject(_ui->messageLabel);
}

/**
 * @brief 窗口关闭按钮槽函数
 */
void CLoginWidget::on_closeBtn_clicked()
{
    this->close();
}

/**
 * @brief 窗口设置按钮槽函数
 */
void CLoginWidget::on_settingBtn_clicked()
{

}

/**
 * @brief 中心按钮点击槽函数
 */
void CLoginWidget::on_centerBtn_clicked()
{
    _cameraOpen = _camera.StatusCamera();//获取摄像头状态
    if(!_cameraOpen)
    {
        if(_camera.OpenCamera("/dev/video0"))//打开摄像头
        {
            SHOWMESSAGE("打开摄像头成功");
            _handleCameraTestStatus(CameraTestStatus::CAMERA_OPEN);
            _cameraThread = std::thread(&CLoginWidget::_labelShowThread,this);
        }
        else
        {
            SHOWMESSAGE("打开摄像头失败");
            _handleCameraTestStatus(CameraTestStatus::CAMERA_UNOPEN);
        }
    }
    else
    {
        SHOWMESSAGE("关闭摄像头");
        _camera.CloseCamera();
        _cameraThread.join();
        _handleCameraTestStatus(CameraTestStatus::CAMERA_CLOSE);
        SHOWMESSAGE();
    }
    //message->show();
}

/**
 * @brief 窗口回滚按钮槽函数
 */
void CLoginWidget::on_backBtn_clicked()
{

}

/**
 * @brief 动画影响阴影槽函数
 * @param value 需要转int
 */
void CLoginWidget::shadowChanged(QVariant value)
{
    _effectForOpen->setBlurRadius(value.toInt());
}

/**
 * @brief 拿取检验结果槽函数
 * @param val
 */
void CLoginWidget::takeResult(bool val)
{

}

/**
 * @brief 中心按钮文字样式改变函数
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
 * @brief 处理摄像头响应状态
 * @param status 摄像头响应状态
 */
void CLoginWidget::_handleCameraTestStatus(CameraTestStatus status)
{
    //按钮处于关闭样式
    QString offStyle = "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #F5DEB3 ,stop:0.5 #FFDEAD ,stop:1 #FFE4C4 );";
    //按钮处于点击样式
    QString onStyle = "background-color:red;";
    switch (status)
    {
        case CameraTestStatus::CAMERA_OPEN: {
            _effectForOpen->setColor(QColor("#CC9966"));
            _animationForOpen->start();
            _ui->centerBtn->setText("结束");
            _ui->centerBtn->setStyleSheet(_centerBtnSs+onStyle);
            break;
        }
        case CameraTestStatus::CAMERA_CLOSE: {
            _animationForOpen->stop();
            _ui->centerBtn->setText("开始");
            _ui->centerBtn->setStyleSheet(_centerBtnSs+offStyle);
            break;
        }
        case CameraTestStatus::CAMERA_UNOPEN: {
            _effectForOpen->setColor(QColor("red"));
            _animationForOpenError->start();
            _ui->messageLabel->setText("<span style=\"color: red; text-decoration: underline; text-decoration-color: red;\">摄像头打开失败</span>");
            break;
        }
        default:
            break;

    }

}

/**
 * @brief 线程：获取CCameraControl对象返回得QImage对象,转换成QPixmap，放入label中显示
 */
void CLoginWidget::_labelShowThread() {
    while(_camera.StatusCamera()) {
        QImage img;
        if(_camera.GetFrame(img)) {
            QPixmap pixmap = QPixmap::fromImage(img);
            _ui->label->setPixmap(pixmap);
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(40));
    }
    SHOWMESSAGE("验证完成！！");
}

void CLoginWidget::receiveVideo(QPixmap pixmap)
{
    _ui->label->setPixmap(pixmap);
}

void CLoginWidget::getKey(QString str)
{
    if (str != "1234") emit defeate();
    else takeResult(true);
}






