//#include "GUILoginwidget.h"
#include "qui_manage_widget.h"
#include "qui_debug_log_window.h"
#include "qui_module_label.h"
#include "q_modules_widget.h"
#include <QApplication>


// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     CLoginWidget w;
//     w.show();
//     return a.exec();
// }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CFunctionWidget w;
    w.show();

    return a.exec();
}
