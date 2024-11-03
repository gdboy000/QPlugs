//
// Created by gao on 24-11-3.
//

#ifndef FACE_IDENTIFY_H
#define FACE_IDENTIFY_H

#include "camera_control.h"



class CFaceIdentify :public CCameraControl{
public:
    CFaceIdentify();//初始化模块
    bool GetFrame(QImage &frame);

};



#endif //FACE_IDENTIFY_H
