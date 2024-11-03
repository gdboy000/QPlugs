//
// Created by gao on 24-10-22.
//

#ifndef QUI_DEBUG_LOG_WINDOW_H
#define QUI_DEBUG_LOG_WINDOW_H

#include <QDialog>
#include <QString>

#define SHOWMESSAGE(str) CDebugWindow::GetWindow()->ShowMessage(QString(str)+QString("( at %1 %2)").arg(QString(__FILE__).split('/').last()).arg(__LINE__))


QT_BEGIN_NAMESPACE
namespace Ui { class CDebugMessage; }
QT_END_NAMESPACE

class CDebugWindow : public QDialog {
Q_OBJECT

public:
    explicit CDebugWindow(QWidget *parent = nullptr);
    ~CDebugWindow() override;
    void ShowMessage(const QString msg);
    void ShowWarning(const QString msg);
    void ShowError(const QString msg);
    void Clear();
    static CDebugWindow * GetWindow();//获取窗口单例
    static void DestroyWindow();//销毁窗口单例
    static void ShowWindow();//展示窗口单例
private:
    Ui::CDebugMessage *_ui;
    int _count = 0;
    static CDebugWindow *_instance;
    QString _richText(const QString msg, const QString color);
};




#endif //QUI_DEBUG_LOG_WINDOW_H
