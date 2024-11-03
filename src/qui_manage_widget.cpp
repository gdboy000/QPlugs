#include <QMouseEvent>
#include <QResizeEvent>
#include <QEvent>
#include <QWindowStateChangeEvent>
#include <QScrollArea>
#include "qui_manage_widget.h"

#include "qui_debug_log_window.h"
#include "qui_module_label.h"
#include "q_modules_widget.h"
#include "ui_qui_manage_widget.h"

CFunctionWidget::CFunctionWidget(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::CFunctionWidget){
    _ui->setupUi(this);
    // _normalSize = this->size();
    _ui->widget2_2->hide();//初始化隐藏分割窗口
    _ui->widget2_1->hide();//菜单窗口隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    _ui->splitter->setSizes(QList<int>() << 1 << 4);//设置分割器初始化分割比例
    _initConnect();
}

CFunctionWidget::~CFunctionWidget(){
    delete _ui;
    if(nullptr == _menuWidget) {
        delete _menuWidget;
    }
    if(nullptr == _currentModuleWidget) {
        delete _currentModuleWidget;
    }
}

void CFunctionWidget::_initConnect() {
    connect(_ui->closeBtn, &QPushButton::clicked, this, &CFunctionWidget::closeBtnClicked);
    connect(_ui->menuBtn, &QPushButton::clicked, this, &CFunctionWidget::menuBtnClicked);
    connect(_ui->forderBtn, &QPushButton::clicked, this, &CFunctionWidget::forderBtnClicked);
    connect(_ui->minBtn, &QPushButton::clicked, this, &CFunctionWidget::minBtnClicked);
    connect(_ui->sizeBtn, &QPushButton::clicked, this, &CFunctionWidget::sizeBtnClicked);
    connect(_ui->moduleBtn,&QPushButton::clicked,this,&CFunctionWidget::moduleBtnClicked);
}


void CFunctionWidget::mousePressEvent(QMouseEvent *event) {

}

void CFunctionWidget::mouseReleaseEvent(QMouseEvent *event) {

}

void CFunctionWidget::resizeEvent(QResizeEvent *event) {
    const int width = event->size().width();
    const int maxWidth = width / 3;
    const int minWidth = width / 12;
    _ui->widget2_2->setMinimumWidth(minWidth);
    _ui->widget2_2->setMaximumWidth(maxWidth);
    QWidget::resizeEvent(event);
}

void CFunctionWidget::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if(QEvent::WindowStateChange == event->type()) {
        QWindowStateChangeEvent *stateEvent =  dynamic_cast<QWindowStateChangeEvent*>(event);
        if (stateEvent->oldState() == Qt::WindowMinimized) {
            _updateWidgetState(_widgetState);
        }
    }

}


void CFunctionWidget::_updateWidgetState(WidgetState state) {
    switch (state) {
        case WidgetState::MAXSTATE:
            _ui->sizeBtn->setIcon(QIcon("../resources/crop-w.png"));
            this->setWindowState(Qt::WindowMaximized);
            _widgetState = WidgetState::MAXSTATE;
            break;
        case WidgetState::MINSTATE:
            // = this->size();
            this->setWindowState(Qt::WindowMinimized);
            break;
        case WidgetState::NORMALSTATE:
            this->setWindowState(Qt::WindowNoState);
            //this->resize(_normalSize);
            _ui->sizeBtn->setIcon(QIcon("../resources/qr-code-w.png"));
            _widgetState = WidgetState::NORMALSTATE;
            break;
        default:
            break;
    }
}

bool CFunctionWidget::_findModule(const QDir &path) {
    if(!path.exists()) return false;
    QStringList nameFilters;
    nameFilters << "*Module.xml";
    QStringList relativeFiles = path.entryList(nameFilters, QDir::Files);
    foreach (const QString &relativeFile, relativeFiles) {
        QString absolutePath = path.absoluteFilePath(relativeFile);
        _moduleFileList.append(absolutePath);
        //qDebug() << absolutePath; // 打印绝对路径
    }
    //qDebug()<<"list: "<<_moduleFileList<<'\n';
    return true;
}

void CFunctionWidget::_initModulesSubArea() {

}

void CFunctionWidget::_initForderSubArea() {
    QWidget *widget = new QWidget(_ui->widget2_2);
    widget->setObjectName("forder");
    widget->setStyleSheet("background-color:red;");
    _ui->widget2_2->layout()->addWidget(widget);
    _currentModuleWidget = widget;
    _ui->widget2_2->layout()->setAlignment(Qt::AlignTop);//设置顶部对齐
    widget->setFixedHeight(50);
}


void CFunctionWidget::closeBtnClicked(bool check) {
    this->close();
}

void CFunctionWidget::menuBtnClicked(bool check) {
    static bool _menuBtnOn = false;
    if(!_menuBtnOn) {
        _ui->menuBtn->setStyleSheet(":hover{border:0 px;}"
            ":!hover{background: rgb(36, 31, 49);border-style:none;border-radius:0px;}");
        _ui->widget2_1->show();
        _menuBtnOn = true;
    }
    else {
        if(!_ui->widget2_2->isHidden()) {
            forderBtnClicked(true);
        }
        _ui->menuBtn->setStyleSheet(":hover{background: rgb(36, 31, 49);border:0 px;} "
            ":!hover{border-style:none;border-radius:0px;}");
        _ui->widget2_1->hide();
        _menuBtnOn = false;
    }

}

void CFunctionWidget::forderBtnClicked(bool check) {
    //qDebug()<<"forder btn clicked\n";
    if(nullptr == _currentModuleWidget) {
        //qDebug()<<"before _currentModuleWidget is null\n";
        _initForderSubArea();
        _ui->widget2_2->show();
    }
    else if(_currentModuleWidget->objectName() != "forder"){
        //qDebug()<<_currentModuleWidget->objectName()<<'\n';
        _ui->widget2_2->layout()->removeWidget(_currentModuleWidget);
        delete _currentModuleWidget;
        _initForderSubArea();
    }
    else {
        _ui->widget2_2->hide();
        _ui->widget2_2->layout()->removeWidget(_currentModuleWidget);
        delete _currentModuleWidget;
        _currentModuleWidget = nullptr;
    }
}

void CFunctionWidget::sizeBtnClicked(bool check) {
    if(_widgetState == WidgetState::MAXSTATE) {
        _updateWidgetState(WidgetState::NORMALSTATE);
    }
    else {
        _updateWidgetState(WidgetState::MAXSTATE);
    }
}

void CFunctionWidget::minBtnClicked(bool check) {
    this->_updateWidgetState(WidgetState::MINSTATE);
}

void CFunctionWidget::moduleBtnClicked(bool) {
    if(nullptr == _currentModuleWidget) {
        //读取文件夹下所有配置文件
        CModulesWidget *modules_widget = new CModulesWidget(QString("../xml/"),_ui->widget2_2);
        modules_widget->GetLoadingWidget(_ui->widget2_3);
        _ui->widget2_2->layout()->addWidget(modules_widget);
        _ui->widget2_2->layout()->setAlignment(Qt::AlignTop);//设置顶部对齐
        _currentModuleWidget = modules_widget;
        _ui->widget2_2->show();
    }
    else if(!_currentModuleWidget->inherits("CModulesWidget")){
        delete _currentModuleWidget;
        CModulesWidget *modules_widget = new CModulesWidget(QString("../xml/"),_ui->widget2_2);
        modules_widget->GetLoadingWidget(_ui->widget2_3);
        _ui->widget2_2->layout()->addWidget(modules_widget);
        _ui->widget2_2->layout()->setAlignment(Qt::AlignTop);//设置顶部对齐
        _currentModuleWidget = modules_widget;
    }
    else {
        _ui->widget2_2->hide();
        _ui->widget2_2->layout()->removeWidget(_currentModuleWidget);
        delete _currentModuleWidget;
        _currentModuleWidget = nullptr;
    }
}








