#include "include/qui_login_widget.h"
#include "include/qui_debug_log_window.h"
#include "include/qui_module_label.h"
#include "include/q_modules_widget.h"
#include <QApplication>
#include <ui_qui_login_widget.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CLoginWidget w;
    w.show();

    return a.exec();
}
