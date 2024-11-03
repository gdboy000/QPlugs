//
// Created by gao on 24-10-24.
//

#ifndef QUI_MODULE_LABEL_H
#define QUI_MODULE_LABEL_H

#include <QMouseEvent>
#include <QWidget>
#include <QtXml/QDomDocument>
#include <QtXml/QtXml>
#include <QFile>
#include <functional>

class QPropertyAnimation;
class CModulesWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class CModuleLabelWidget; }
QT_END_NAMESPACE

class CModuleLabelWidget : public QWidget {
Q_OBJECT

public:
    explicit CModuleLabelWidget(CModulesWidget *parent);
    ~CModuleLabelWidget() override;
    bool InitModuleLabelFromXml(const QString& xmlFile);
    QString GetWebUrl(){return _webUrl;}
protected:
    void showEvent(QShowEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    // void mousePressEvent(QMouseEvent *event) override;
    // void mouseMoveEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;//鼠标进入
    void leaveEvent(QEvent *event) override;//鼠标离开
private:
    Ui::CModuleLabelWidget *_ui;
    QFile _xmlFile;
    QDomDocument _xmlDoc;
    QString _moduleName;
    QString _moduleBrief;
    QString _moduleDownload;
    QString _moduleAttention;
    QString _moduleSource;
    QString _webUrl;
    QString _iconUrl;
    QString _iconUrlFrom;
    QString _originalStyleSheet;
    QPropertyAnimation* _animation = nullptr;
    QPropertyAnimation* _animation2 = nullptr;
    CModulesWidget* _connectWidget;
    bool _initModuleLabelFromXml();
    void _initLoadLabel();//初始化加载label
    void _initConnect();//初始化所有连接
    void _initAnimation();//初始化所有动画

private slots:
    void _hoverStyleSheetChange(const QVariant& );
    void _noHoverStyleSheetChange(const QVariant& );

signals:
    void clicked(CModuleLabelWidget*);

};


#endif //GUIMODULELABELWIDGET_H
