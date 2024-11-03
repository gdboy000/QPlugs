//
// Created by gao on 24-10-24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GUIModuleLabelWidget.h" resolved

#include <QPropertyAnimation>

#include "qui_module_label.h"
#include "q_modules_widget.h"
#include "ui_qui_module_label.h"
#include "qui_debug_log_window.h"
#include <QDebug>


CModuleLabelWidget::CModuleLabelWidget(CModulesWidget *parent) :
    QWidget(parent),_ui(new Ui::CModuleLabelWidget), _connectWidget(parent) {
    _ui->setupUi(this);
    int h = _ui->iconLabel->height();
    int forwardH = h * 0.8;
    _ui->iconLabel->resize(forwardH,forwardH);
    this->setAttribute(Qt::WA_StyledBackground);
    this->setMouseTracking(true);
    _initAnimation();//注意顺序，需要先初始化动画
    _initConnect();

}

CModuleLabelWidget::~CModuleLabelWidget() {
    delete _ui;
    delete _animation;
}

bool CModuleLabelWidget::InitModuleLabelFromXml(const QString& xmlFile) {
    _xmlFile.setFileName(xmlFile);
    return _initModuleLabelFromXml();
}


bool CModuleLabelWidget::_initModuleLabelFromXml() {
    if (!_xmlFile.open(QIODevice::ReadOnly))
    {
        SHOWMESSAGE(QString("open xml error:%1").arg(_xmlFile.fileName()));
        return false;
    }
    if (!_xmlDoc.setContent(&_xmlFile))
    {
        SHOWMESSAGE(QString("read xml error:%1").arg(_xmlFile.fileName()));
        _xmlFile.close();
        return false;
    }
    const QDomElement root = _xmlDoc.documentElement();//读取根节点
    const QDomElement element = _xmlDoc.elementsByTagName("message").at(0).toElement();
    _moduleName = element.namedItem("name").firstChild().nodeValue();
    _moduleBrief = element.namedItem("brief").firstChild().nodeValue();
    _moduleDownload = element.namedItem("download").firstChild().nodeValue();
    _moduleAttention = element.namedItem("attention").firstChild().nodeValue();
    _moduleSource = element.namedItem("source").firstChild().nodeValue();
    _webUrl = _xmlDoc.elementsByTagName("install").at(0).toElement().namedItem("url").firstChild().nodeValue();
    _iconUrl = _xmlDoc.elementsByTagName("install").at(0).toElement().namedItem("icon_url").firstChild().nodeValue();
    _iconUrlFrom = _xmlDoc.elementsByTagName("install").at(0).toElement().namedItem("icon_url").toElement().attribute("from");
    _xmlFile.close();
    return true;
}

void CModuleLabelWidget::showEvent(QShowEvent *event) {
    if(!_xmlDoc.isNull()) {
        _initLoadLabel();
    }
    QWidget::showEvent(event);
}

void CModuleLabelWidget::enterEvent(QEnterEvent *event) {
    static bool first_set = false;
    _originalStyleSheet = this->styleSheet();
    _animation->start();
    if(!first_set)
    {

        //启动hover动画
        first_set = true;
    }
}

void CModuleLabelWidget::leaveEvent(QEvent *event) {
    //this->setStyleSheet(_originalStyleSheet);
    //启动leave动画
    _animation2->start();
}

void CModuleLabelWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        //qDebug()<<"发送信号\n";
        emit clicked(this);
        event->accept();
    }
    else {
        event->ignore();
    }

}

void CModuleLabelWidget::_initLoadLabel() {
    _ui->titleLabel->setText(QString("<span style='color: #000000;font-size: 12px;'><b>%1</b></span>").arg(_moduleName));
    _ui->infoLabel->setText(QString("<span style='color: #000000;font-size: 7px;'>%1</span>").arg(_moduleBrief));
    _ui->countLabel->setText(QString("<span style='color: #000000;font-size: 7px;'>下载：%1&nbsp;&nbsp;收藏：%2&nbsp;&nbsp;&nbsp;来源：%3</span>").arg(_moduleDownload).arg(_moduleAttention).arg(_moduleSource));
    QPixmap pixmap(_iconUrl);
    auto percent_height =  _ui->widget1->height() * 0.7;
    _ui->iconLabel->resize(QSize(percent_height,percent_height));
    QPixmap pixmap_scale = pixmap.scaled(QSize(percent_height,percent_height));
    _ui->iconLabel->setPixmap(pixmap_scale);
}

void CModuleLabelWidget::_hoverStyleSheetChange(const QVariant &value) {
    //qDebug()<<"get value "<<value.toFloat()<<'\n';
    const float _value = value.toFloat()/10;
    if(_value == 0) {
        this->setStyleSheet("background-color: rgba(255, 255, 255, 255);");
    }
    else {
        QString _styleSheet = QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
                                "stop:0 rgba(150, 150, 150, 255), "
                                "stop:%1 rgba(255, 255, 255, 255))").arg(_value);
        this->setStyleSheet(_styleSheet);
    }

}

void CModuleLabelWidget::_noHoverStyleSheetChange(const QVariant &value) {
    const float _value = value.toFloat()/10;
    //qDebug()<<"no hover value: "<<_value<<'\n';
    if(_value == 1) {
        this->setStyleSheet("background-color: rgba(150, 150, 150, 255);");
    }
    else {
        QString _styleSheet = QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, "
                                "stop:%1 rgba(150, 150, 150, 255), "
                                "stop:1 rgba(255, 255, 255, 255))").arg(_value);
        this->setStyleSheet(_styleSheet);
    }
}



void CModuleLabelWidget::_initConnect() {
    connect(_animation,&QPropertyAnimation::valueChanged,this,&CModuleLabelWidget::_hoverStyleSheetChange);
    connect(_animation2,&QPropertyAnimation::valueChanged,this,&CModuleLabelWidget::_noHoverStyleSheetChange);
    connect(this,&CModuleLabelWidget::clicked,_connectWidget,&CModulesWidget::moduleClicked);

}

void CModuleLabelWidget::_initAnimation() {
    _animation = new QPropertyAnimation(this);
    _animation->setKeyValueAt(0,10);
    _animation->setKeyValueAt(1,0);
    _animation->setDuration(300);
    _animation->setLoopCount(1);
    _animation->setTargetObject(this);

    _animation2 = new QPropertyAnimation(this);
    _animation2->setKeyValueAt(0,0);
    _animation2->setKeyValueAt(1,10);
    _animation2->setDuration(300);
    _animation2->setLoopCount(1);
    _animation2->setTargetObject(this);
}




