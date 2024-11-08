#ifndef QUI_LOGIN_WIDGET_H
#define QUI_LOGIN_WIDGET_H

#include <chrono>
#include <thread>
#include <string>
#include <QWidget>
#include "face_identify.h"
#include "qui_manage_widget.h"

class QGraphicsDropShadowEffect;
class QPropertyAnimation;

using namespace std::chrono;

namespace Ui {
class CLoginWidget;
}

/**
 * 登录界面
 */
class CLoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CLoginWidget(QWidget *parent = nullptr);
    ~CLoginWidget();
    void notifyIdentify(bool);//验证回调
private:
    //验证状态
    enum class IdentifyStatus{
        NONE = 0,
        CAMERA_UNOPEN,//摄像头未打开
        CAMERA_OPEN,//摄像头已打开
        CAMERA_CLOSE,//摄像头关闭
        CODE_ERROR //验证码错误
    };
    //验证方式
    enum class IdentifyMode {
        IM_NONE = 0,
        IM_FACE, //验证方式人脸
        IM_CODE //验证方式验证码
    };
    Ui::CLoginWidget *_ui = nullptr;
    QGraphicsDropShadowEffect* _effectForOpen = nullptr;//摄像头打开状态阴影
    QPropertyAnimation* _animationForOpen = nullptr;//摄像头打开时动画
    QPropertyAnimation* _animationForOpenError = nullptr;//摄像头打开失败时动画
    QPropertyAnimation* _animationForChangeMode = nullptr;//验证模块转换动画
    CFaceIdentify _camera;//人脸识别类
    QString _centerBtnSs;//中心按钮样式
    std::thread _cameraThread;//label设置线程
    time_point<system_clock> _startTime;//验证时间
    bool _cameraOpen = false;//人脸检验摄像头打开标志
    CFunctionWidget* _mWidget = nullptr;//success界面
    IdentifyMode _identifyMode = IdentifyMode::IM_NONE;//当前验证模式
    IdentifyStatus _identifyStatus = IdentifyStatus::NONE;//当前验证状态
    void _initConnect();//初始化connect
    void _initAnimation();//初始化animation
    void _updateCenterStyle(bool,std::string);//更新中心按钮样式
    void _handleIdentifyStatus(IdentifyStatus);//处理验证状态
    void _labelShowThread();//label显示线程
    void _result(bool,IdentifyStatus status = IdentifyStatus::NONE);//验证结果


private slots:
    void onCloseBtnClicked();//窗口关闭点击
    void onSettingBtnClicked();//设置点击
    void onCenterBtnClicked();//中心按钮点击
    void onBackBtnClicked();//回滚窗口点击
    void shadowChanged(QVariant);//阴影设置
    void identifyModeChange(QVariant);//验证方式切换
    void updateMessageText(QString);//更新message label信息
    void codeIdentify();//验证码验证

signals:
    void changeMessageText(QString);
    void identifyResult(bool,IdentifyStatus staus = IdentifyStatus::NONE);


};

#endif // QUI_LOGIN_WIDGET_H
