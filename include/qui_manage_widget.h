#ifndef QUI_MANAGE_WIDGET_H
#define QUI_MANAGE_WIDGET_H

#include <QWidget>
#include <QDir>


namespace Ui {
class CFunctionWidget;
}

class CFunctionWidget : public QWidget
{
    Q_OBJECT
private:
    enum class WidgetState {
        MAXSTATE = 0,
        MINSTATE = 1,
        NORMALSTATE = 2
    };
    Ui::CFunctionWidget *_ui;
    QSize _normalSize = {0,0};
    // bool _menuBtnOn = false;
    QWidget *_menuWidget = nullptr;
    QDir _modulePath = QString("../xml/");
    QStringList _moduleFileList;
    WidgetState _widgetState = WidgetState::NORMALSTATE;
    QWidget* _currentModuleWidget = nullptr;
    void _initConnect();//初始化所有信号与槽的连接
    void _updateWidgetState(WidgetState state);//处理和更新窗口状态
    bool _findModule(const QDir&);//通过dir查找所有本地module配置文件
    void _initForderSubArea();//初始化forder功能管辖空间。(待优化，widget是否可以减少渲染次数)
    void _initModulesSubArea();//初始化module功能管辖空间
public:
    explicit CFunctionWidget(QWidget *parent = nullptr);
    ~CFunctionWidget() final;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *) override;

private slots:
    void closeBtnClicked(bool);
    void menuBtnClicked(bool);
    void forderBtnClicked(bool);
    void sizeBtnClicked(bool);
    void minBtnClicked(bool);
    void moduleBtnClicked(bool);


};

#endif // QUI_MANAGE_WIDGET_H
