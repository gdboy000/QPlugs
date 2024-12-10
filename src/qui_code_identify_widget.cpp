//
// Created by gao on 24-11-10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_qui_code_identify_widget.h" resolved

#include "qui_code_identify_widget.h"
#include "ui_qui_code_identify_widget.h"


CCodeIdentifyWidget::CCodeIdentifyWidget(QWidget *parent) :
    QWidget(parent), _ui(new Ui::CCodeIdentifyWidget) {
    _ui->setupUi(this);
}

CCodeIdentifyWidget::~CCodeIdentifyWidget() {
    delete _ui;
}
