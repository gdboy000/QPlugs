//
// Created by gao on 24-10-27.
//

#ifndef Q_MODULES_WIDGET_H
#define Q_MODULES_WIDGET_H

#include <QScrollArea>
#include <QStringList>
#include <vector>
#include <QDir>


class CModuleLabelWidget;
class QWebEngineView;
class CWebModulesWidget;

using std::vector;
QT_BEGIN_NAMESPACE

/**
 * 模块列表显示区域初始化
 * 该类应当可根据传入的xml文件，url来进行url解析
 * 并通过解析内容初始化模块显示区域
 * url的web内容保存在一个QWebEngineView对象中
 * 模块加载完毕，会加载url的web界面
 * 如果url加载异常，会加载本地error.html
 *
 * 因为需要添加webView的控件，与当前控件无关联。
 * 所以要自定义函数传入控件
 */
class CModulesWidget : public QScrollArea {
Q_OBJECT
public:
    explicit CModulesWidget(QWidget *parent = nullptr);
    explicit CModulesWidget(const QString& path,QWidget *parent = nullptr);//path初始化模块路径
    bool FindModules(const QDir& dirPath);//默认设置模块搜索路径
    void GetLoadingWidget(QWidget*);
    ~CModulesWidget() override;
protected:
    void showEvent(QShowEvent *event) override;//显示事件重写
private:
    QStringList _modulesList;
    QDir _modulesPath;
    QWidget *_loadingWidget = nullptr;//需要被加载的控件
    QWebEngineView *_webView = nullptr;//web view
    CWebModulesWidget* _webWidget = nullptr;
    //vector<CModuleLabelWidget *> _modulesVec;
    void _initWidget();//初始化所属区域
    void _updateModules();//更新模块显示区域
    void _findModules();//寻找模块文件
    void _initWebWidget();//初始化web界面
    void _loadingWebWidget(const QString&);//加载web界面
public slots:
    void moduleClicked(CModuleLabelWidget*);


};


#endif //Q_MODULES_WIDGET_H
