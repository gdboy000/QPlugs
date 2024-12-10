//
// Created by gao on 24-11-2.
//

#ifndef QUI_MODULES_WEB_WIDGET_H
#define QUI_MODULES_WEB_WIDGET_H

#include <QWidget>
#include <ui_qui_modules_web_widget.h>


QT_BEGIN_NAMESPACE
namespace Ui { class CWebModulesWidget; }
QT_END_NAMESPACE

/**
 * 该类是对QWebEnginerView的封装
 * 添加：右键选项
 */
class CWebModulesWidget : public QWidget {
Q_OBJECT

public:
    explicit CWebModulesWidget(QWidget *parent = nullptr);
    ~CWebModulesWidget() override;
    void LoadingUrl(const QString&);
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
private:
    Ui::CWebmodulesWidget *_ui;
    void _initWidget();//加载widget,当前不需要
};


#endif //QUI_MODULES_WEB_WIDGET_H
