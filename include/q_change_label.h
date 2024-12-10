/**
 * Gao TongHe thegao@aliyun.com
 * @data 2022/4/14 22:50:23
 * @brief 曲线标签类
*/

#ifndef QUI_CHANGE_LABEL_H
#define QUI_CHANGE_LABEL_H

#include <QLabel>


class QPixmap;

class CCurveLabel : public QLabel
{
    Q_OBJECT

public:
    CCurveLabel(QWidget *parent = Q_NULLPTR);
    CCurveLabel(QSize,QWidget* parent = Q_NULLPTR);
    ~CCurveLabel();
    void SetBorder(Qt::PenStyle style, QColor color, int width);
    int GetRadius(){return _round;}

signals:
    void signClick();
protected:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* ev) override;
    void mouseMoveEvent(QMouseEvent* ev) override;
    void mouseReleaseEvent(QMouseEvent* ev) override;

private:
    QColor _color;
    Qt::PenStyle _style;
    int _width;
    bool user_define;
    QSize size;
    int key;
    bool m_set;
    QPixmap oldMap;
    int _round;
};
#endif // QUI_CHANGE_LABEL_H
