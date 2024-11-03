#ifndef QUI_LOGIN_WIDGET_H
#define QUI_LOGIN_WIDGET_H

#include <QWidget>
#include <QVariant>
#include <future>
#include <thread>
#include <string>
#include "camera_control.h"

class QParallelAnimationGroup;
class QStackedWidget;
class QVBoxLayout;
class QGraphicsDropShadowEffect;
class QPropertyAnimation;

namespace Ui {
class CLoginWidget;
}

class CLoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CLoginWidget(QWidget *parent = nullptr);
    ~CLoginWidget();

private:
    enum class CameraTestStatus{
        CAMERA_UNOPEN = 0,
        CAMERA_OPEN,
        CAMERA_CLOSE
    };
    Ui::CLoginWidget *_ui;
    QStackedWidget* _stackWidget;
    QParallelAnimationGroup* _upGroup;
    QParallelAnimationGroup* _downGroup;
    QWidget* _widget;
    QVBoxLayout* _box;
    QGraphicsDropShadowEffect* _effectForOpen;//摄像头打开状态阴影
    QPropertyAnimation* _animationForOpen;//摄像头打开时动画
    QPropertyAnimation* _animationForOpenError;//摄像头打开失败时动画
    std::thread* _td;
    std::thread* _rock;
    QString _str;
    CCameraControl _camera;
    QString _centerBtnSs;
    std::thread _cameraThread;
    bool _cameraOpen = false;//人脸检验摄像头打开标志
    void _initConnect();
    void _initAnimation();
    void _updateCenterStyle(bool,std::string);
    void _handleCameraTestStatus(CameraTestStatus);
    void _labelShowThread();

private slots:
    void on_closeBtn_clicked();//窗口关闭
    void on_settingBtn_clicked();//人脸检验设置
    void on_centerBtn_clicked();//
    void on_backBtn_clicked();//回滚窗口
    void shadowChanged(QVariant);
    void takeResult(bool);
    void receiveVideo(QPixmap);
    void getKey(QString);

signals:
    void login(bool);
    void waitForBtnEnter(int);
    void changeLabel(QPixmap);
    void leave(QString);
    void releave();
    void defeate();


};

#endif // QUI_LOGIN_WIDGET_H
