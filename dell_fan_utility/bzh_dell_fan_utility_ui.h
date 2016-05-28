#ifndef BZH_DELL_FAN_UTILITY_UI_H
#define BZH_DELL_FAN_UTILITY_UI_H


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

void MainWindow::ui_init()
{
    this->setFixedSize(this->width(),this->height());
    msgbox = new QMessageBox;
    msgbox->setWindowTitle("Warning!!!");
    msgbox->setIcon(QMessageBox::Warning);
    msgbox->setText("\n"
                    "Disable EC control is a kind of dangero perate. \n"
                    "That means if this program is not running \n"
                    "the fan will completely STOP at all \n"
                    "\n"
                    "There is only one way to restore: completely shutdonw then reboot \t \n");
    msgbox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgbox->setInformativeText("Are you SURE?");
}

void MainWindow::ui_cleanup()
{

}

#endif // BZH_DELL_FAN_UTILITY_UI_H

