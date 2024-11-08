#include "q_change_label.h"
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

CCurveLabel::CCurveLabel(QWidget *parent)
    : QLabel(parent),user_define(false)
{
    m_set = false;
}

CCurveLabel::CCurveLabel(QSize size, QWidget* parent)
    : QLabel(parent),
    size(size),user_define(true)
{
}

CCurveLabel::~CCurveLabel()
{

}

void CCurveLabel::paintEvent(QPaintEvent* e)
{
    if (!pixmap().isNull())
    {
        QPainter painter(this);
        //设置反锯齿
        painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::SmoothPixmapTransform);
        QPainterPath path;
        int round = qMin(width()-10, height()-10);
        path.addEllipse(5, 5, round, round);
        painter.setClipPath(path);
        oldMap = pixmap(); //获取原来的
        if (m_set)
        {
            painter.drawPixmap(0, 0, width() + 10, height() + 10, oldMap);
        }
        else {
            painter.drawPixmap(0, 0, width() + 10, height() + 10, oldMap);
        }


    }
    else {
        QLabel::paintEvent(e);
    }
}

void CCurveLabel::mousePressEvent(QMouseEvent* ev)
{
    m_set = true;
    update();
    QLabel::mousePressEvent(ev);
}

void CCurveLabel::mouseMoveEvent(QMouseEvent* ev)
{
    m_set = false;
    update();
    QLabel::mouseMoveEvent(ev);
}

void CCurveLabel::mouseReleaseEvent(QMouseEvent* ev)
{
    m_set = false;
    update();
    emit signClick();
    QLabel::mouseReleaseEvent(ev);
}
