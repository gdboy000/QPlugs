//
// Created by gao on 24-10-27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_QModulesWidget.h" resolved
#include <QVBoxLayout>
#include <QtWebEngineWidgets/qwebengineview.h>
#include <QPushButton>

#include "qui_debug_log_window.h"
#include "q_modules_widget.h"
#include "qui_module_label.h"
#include "qui_modules_web_widget.h"




/**
 * 基本默认构造，初始化了默认的模块路径为当前目录
 * @param parent (QWidget*)父类
 */
CModulesWidget::CModulesWidget(QWidget *parent) :
    QScrollArea(parent) {
    _modulesPath = QString("./");
    _findModules();
}

/**
 * 构造函数，可传参模块存在路径
 * @param modulesPath (r QString&)模块所在路径
 * @param parent (QWidget*)父类
 */
CModulesWidget::CModulesWidget(const QString &modulesPath, QWidget *parent):
    QScrollArea(parent) {
    //this->setStyleSheet("background-color:transparent;");
    _modulesPath = modulesPath;
    if(!_modulesPath.exists()) {
        _modulesPath = QString("./");
    }
    _findModules();

}

/**
 * 析构函数
 */
CModulesWidget::~CModulesWidget() {
    if(nullptr == _loadingWidget) {
        delete _loadingWidget;
    }
    if(nullptr == _webView) {
        delete _webView;
    }
}

/**
 * 当构造函数没有传入模块路径时，此函数可以初始化模块所在路径
 * @param dirPath (r QDir&)模块路径。可以通过QString转QDir
 * @return 返回路径是否存在
 */
bool CModulesWidget::FindModules(const QDir &dirPath) {
    if(dirPath.exists()) {
        _modulesPath = dirPath;
        _findModules();
        return true;
    }
    return false;
}

void CModulesWidget::GetLoadingWidget(QWidget *widget) {
    _loadingWidget = widget;
}


void CModulesWidget::moduleClicked(CModuleLabelWidget *module) {
    //判断如果未初始化web，第一次初始化
    if(nullptr != module) {
        if(nullptr == _webWidget)
            _initWebWidget();
        _loadingWebWidget(module->GetWebUrl());

    }
    else {

    }
}


/**
 * 重写显示事件，当窗口显示时，初始化模块列表
 * @param event (QShowEvent*)显示事件
 */
void CModulesWidget::showEvent(QShowEvent *event) {
    _initWidget();
    QWidget::showEvent(event);
}

/**
 * 初始化widget \n
 * 初始化模块放到指定位置
 */
void CModulesWidget::_initWidget() {
    SHOWMESSAGE(QString("modules list have %1 ").arg(_modulesList.size()));
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(0);//设置布局间隔为0
    vLayout->setContentsMargins(0, 0, 0, 0);//设置布局周围margin为0
    this->setLayout(vLayout);
    if(!_modulesList.empty()) {
        for(auto fileName: _modulesList) {
            CModuleLabelWidget *label = new CModuleLabelWidget(this);
            if(label->InitModuleLabelFromXml(fileName)) {
                SHOWMESSAGE(QString("load module: %1 success").arg(fileName));
            }
            else {
                SHOWMESSAGE(QString("load module: %1 error").arg(fileName));
            }
            label->setFixedHeight(70);
            vLayout->addWidget(label);
        }
        vLayout->addStretch();
    }

}

/**
 * 查找模块 \n
 * 保存绝对路径到列表
 */
void CModulesWidget::_findModules() {
    //首先清空暂存的列表
    _modulesList.clear();
    QStringList nameFilters;
    nameFilters << "*Module.xml";//匹配文件名
    QStringList relativeFiles = _modulesPath.entryList(nameFilters, QDir::Files);
    SHOWMESSAGE(QString("查询模块路径  %1").arg(_modulesPath.path()));
    foreach (const QString &relativeFile, relativeFiles) {
        QString absolutePath = _modulesPath.absoluteFilePath(relativeFile);
        _modulesList.append(absolutePath);
    }
}

/**
 * 更新模块显示区域 \n
 * 通过重新获取list列表，与旧list进行对比 \n
 * 删除已不存在的模块，增加新的模块
 */
void CModulesWidget::_updateModules() {
    QStringList nameFilters;
    nameFilters << "*Module.xml";//匹配文件名
    QStringList relativeFiles = _modulesPath.entryList(nameFilters, QDir::Files);
    foreach (const QString &relativeFile, relativeFiles) {
        //A(最新),B(以前)集合
        //增加 A-(A&&B) 减去 B-(A&&B)
    }
}

/**
 * 对要加载的控件进行初始化
 * 首先判断控件是否为空，不为空继续执行
 * 设置layout，添加自定义
 */
void CModulesWidget::_initWebWidget() {
    if(nullptr != _loadingWidget) {
        QVBoxLayout *layout = new QVBoxLayout(_loadingWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        _loadingWidget->setLayout(layout);//这个去掉看是否还可以设置成功
        _webWidget = new CWebModulesWidget();
        layout->addWidget(_webWidget);
    }
}

void CModulesWidget::_loadingWebWidget(const QString& url) {
    if(nullptr != _webWidget) {
        _webWidget->LoadingUrl(url);
    }
}









