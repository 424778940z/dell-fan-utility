#ifndef BZH_DELL_FAN_UTILITY_DRV
#define BZH_DELL_FAN_UTILITY_DRV

#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::drv_init()
{
    if(BDSID_Initialize())
    {
        qDebug() << "Initialize driver succeed";
    }
    else
    {
        qDebug() << "Initialize driver failed";
    }
}

void MainWindow::drv_cleanup()
{
    BDSID_Shutdown();
    qDebug() << "Shutdown driver succeed";
}

#endif // BZH_DELL_FAN_UTILITY_DRV

