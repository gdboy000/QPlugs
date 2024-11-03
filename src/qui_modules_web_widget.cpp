//
// Created by gao on 24-11-2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_qui_modules_web_widget.h" resolved

#include <QVBoxLayout>
#include <QMenu>
#include <QContextMenuEvent>

#include "qui_modules_web_widget.h"
#include "ui_qui_modules_web_widget.h"


CWebModulesWidget::CWebModulesWidget(QWidget *parent) :
    QWidget(parent), _ui(new Ui::CWebModulesWidget) {
    _ui->setupUi(this);
    _ui->webView->setContextMenuPolicy(Qt::NoContextMenu);//禁言右键菜单
}

CWebModulesWidget::~CWebModulesWidget() {
    delete _ui;
}

void CWebModulesWidget::LoadingUrl(const QString &url) {
    qDebug()<<"url: "<<url<<'\n';
    _ui->webView->load(url);
    //_ui->webView->show();
}

void CWebModulesWidget::_initWidget() {

}

void CWebModulesWidget::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    QAction *action1 = menu.addAction("选项 1");
    QAction *action2 = menu.addAction("选项 2");
    // 连接信号和槽来处理菜单项的触发
    // connect(action1, &QAction::triggered, this, &MyWidget::onAction1Triggered);
    // connect(action2, &QAction::triggered, this, &MyWidget::onAction2Triggered);
    // 在鼠标右键按下的位置显示菜单
    menu.exec(event->globalPos());
    event->accept();
}



