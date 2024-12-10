//
// Created by gao on 24-11-10.
//

#ifndef QUI_FACE_IDENTIFY_WIDGET_H
#define QUI_FACE_IDENTIFY_WIDGET_H

#include <QWidget>
#include <QString>
#include <thread>

#include "face_identify.h"

class CFaceIdentify;
class QGraphicsDropShadowEffect;
class QGraphicsBlurEffect;
class QPropertyAnimation;

QT_BEGIN_NAMESPACE
namespace Ui { class CFaceIdentifyWidget; }
QT_END_NAMESPACE

class CFaceIdentifyWidget : public QWidget {
Q_OBJECT

public:
    explicit CFaceIdentifyWidget(QWidget *parent = nullptr);
    ~CFaceIdentifyWidget() override;
    void NotifyCallBack(bool);//验证消息回调
protected:
    void hideEvent(QHideEvent *event) override;
private:
    enum class IdentifyStatus {
        IS_CAMERAUNOPEN = 0,
        IS_CAMERAOPEN,
        IS_CAMERACLOSE,
        IS_NONE
    };
    Ui::CFaceIdentifyWidget *_ui;
    CFaceIdentify _camera;//验证类
    QPropertyAnimation* _animation;//动画
    QGraphicsDropShadowEffect* _effect = nullptr;//摄像头打开状态阴影
    QGraphicsBlurEffect* _blurEffect = nullptr;//label模糊操作
    IdentifyStatus _identifyStatus = IdentifyStatus::IS_NONE;//验证状态
    QString _centerBtnSs;
    QString _centerBtnSsOFF;
    QString _centerBtnSsON;
    std::thread _cameraThread;//label设置线程

    void _initStyle();//初始化全局样式
    void _handleIdentifyStatus(IdentifyStatus);//处理验证状态
    void _initAnimation();//初始化动画
    void _labelShowThread();//label显示线程
private slots:
    void onCenterBtnClicked();//中心按钮点击事件

};


#endif //QUI_FACE_IDENTIFY_WIDGET_H
