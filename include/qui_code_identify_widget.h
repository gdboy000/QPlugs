//
// Created by gao on 24-11-10.
//

#ifndef QUI_CODE_IDENTIFY_WIDGET_H
#define QUI_CODE_IDENTIFY_WIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class CCodeIdentifyWidget; }
QT_END_NAMESPACE

class CCodeIdentifyWidget : public QWidget {
Q_OBJECT

public:
    explicit CCodeIdentifyWidget(QWidget *parent = nullptr);
    ~CCodeIdentifyWidget() override;

private:
    Ui::CCodeIdentifyWidget *_ui;
};


#endif //QUI_CODE_IDENTIFY_WIDGET_H
