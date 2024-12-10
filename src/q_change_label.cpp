#include "q_change_label.h"
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

CCurveLabel::CCurveLabel(QWidget *parent)
    : QLabel(parent),user_define(false)
{
    qDebug()<<"size:  "<<this->width()<<"   "<<this->height();
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

    QPainter painter(this);//
    //设置反锯齿
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPen pen;//
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setBrush(Qt::black);
    painter.setPen(pen);
    QPainterPath path;
    _round = qMin(width(), height()) - 10;//
    path.addEllipse(5, 5, _round, _round);
    painter.drawEllipse(5, 5, _round, _round);
    if (!pixmap().isNull())
    {
        painter.setClipPath(path);
        oldMap = pixmap(); //获取原来的
        painter.drawPixmap(0, 0, width(), height(), oldMap);
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
