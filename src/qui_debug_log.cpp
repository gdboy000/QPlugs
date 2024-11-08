//
// Created by gao on 24-10-22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GUIDebugMessage.h" resolved
#include <QDateTime>
#include <QMenu>
#include <QContextMenuEvent>
#include "qui_debug_log_window.h"
#include "ui_qui_debug_log_window.h"



CDebugWindow::CDebugWindow(QWidget *parent) :
    QDialog(parent), _ui(new Ui::CDebugMessage) {
    _ui->setupUi(this);
    //this->showMinimized();//直接最小化显示,也会调用show函数
    _ui->textEdit->setContextMenuPolicy(Qt::NoContextMenu);
}

CDebugWindow::~CDebugWindow() {
    delete _ui;
}


void CDebugWindow::ShowMessage(const QString msg) {
    _ui->textEdit->append(_richText(msg, "black"));
}

void CDebugWindow::ShowWarning(const QString msg) {
    _ui->textEdit->append(_richText(msg, "blue"));
}

void CDebugWindow::ShowError(const QString msg) {
    _ui->textEdit->append(_richText(msg, "red"));
}

void CDebugWindow::Clear() {
    _ui->textEdit->clear();
}

CDebugWindow *CDebugWindow::GetWindow() {
    if(nullptr == _instance) {
            _instance = new CDebugWindow(nullptr);
    }
    return _instance;
}

void CDebugWindow::DestroyWindow() {
    if(nullptr != _instance) {
        delete _instance;
        _instance = nullptr;
    }
}

void CDebugWindow::ShowWindow() {
    if(nullptr != _instance) {
        _instance->show();
    }
}

void CDebugWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    QAction *action = menu.addAction("清屏");
    // 连接信号和槽来处理菜单项的触发
    connect(action, &QAction::triggered, this, &CDebugWindow::Clear);
    // 在鼠标右键按下的位置显示菜单
    menu.exec(event->globalPos());
    event->accept();
}




QString CDebugWindow::_richText(const QString msg, const QString color) {
    //<span style='color: red;'>Red Text</span>
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString timeString = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");
    return QString("<span style='color: %1;'>%2</span>").arg(color).arg(msg)+"--"+timeString;
}

CDebugWindow *CDebugWindow::_instance = nullptr;
