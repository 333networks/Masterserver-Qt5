#include <QCoreApplication>
#include <Core/core.h>

int main(int argc, char *argv[])
{
    QCoreApplication QApplication(argc, argv);

    // run the masterserver
    Core MasterServerApplication( QApplication.applicationDirPath() );
    MasterServerApplication.run();
    return QApplication.exec();
}
