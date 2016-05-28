#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "bzh_dell_fan_utility_drv.h"
#include "bzh_dell_fan_utility_ui.h"
#include "bzh_dell_fan_utility_th.h"

#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //init
    drv_init();
    ui_init();
    th_init();
}

MainWindow::~MainWindow()
{
    //cleanup
    th_cleanup();
    ui_cleanup();
    drv_cleanup();

    delete ui;
}

void MainWindow::slot_sensor_init_ready()
{
    if(th_sensorsvc->bzh_dell_fan_SensorService_pkg.init_ready)
    {
        //qDebug() << "slot_sensor_init_ready";
        ui->pbar_rpm_fan_cpu->setMinimum(th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_min);
        ui->pbar_rpm_fan_cpu->setMaximum(th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_max);
        ui->pbar_rpm_fan_gpu->setMinimum(th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_min);
        ui->pbar_rpm_fan_gpu->setMaximum(th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_max);

        //th_fanpwmsvc_gpu->bzh_dell_fan_FanPWMService_pkg.min_rpm_fan = th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_min;
        //th_fanpwmsvc_gpu->bzh_dell_fan_FanPWMService_pkg.max_rpm_fan = th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_max;
    }
}

void MainWindow::slot_sensor_data_ready()
{
    if(th_sensorsvc->bzh_dell_fan_SensorService_pkg.data_ready)
    {
        if(!th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_only_mode)
        {
            ui->hs_lv_fan_cpu->setValue(th_sensorsvc->bzh_dell_fan_SensorService_pkg.lv_cpu_fan);
            ui->hs_lv_fan_gpu->setValue(th_sensorsvc->bzh_dell_fan_SensorService_pkg.lv_gpu_fan);
        }
        ui->pbar_rpm_fan_cpu->setValue(th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_current);
        ui->pbar_rpm_fan_gpu->setValue(th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_current);

        //th_fanpwmsvc_gpu->bzh_dell_fan_FanPWMService_pkg.current_rpm_fan = th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_current;
    }
}

void MainWindow::on_pb_pwm_clicked()
{
    if(th_fanpwmsvc_gpu->isRunning())
    {
        th_fanpwmsvc_gpu->quit();
    }
    else
    {
        if(th_sensorsvc->isRunning())
        {
            th_sensorsvc->quit();
        }
        if(th_fansvc_cpu->isRunning())
        {
            th_fansvc_cpu->quit();
        }
        if(th_fansvc_gpu->isRunning())
        {
            th_fansvc_gpu->quit();
        }

        th_fanpwmsvc_gpu->bzh_dell_fan_FanPWMService_pkg.loop = true;
        th_fanpwmsvc_gpu->start();
    }
}

void MainWindow::on_pb_dc_refresh_clicked()
{
    emit signal_duty_cycle_changed();
}

void MainWindow::on_hs_lv_fan_cpu_valueChanged(int value)
{
    th_fansvc_cpu->bzh_dell_fan_FanService_pkg.level_fan = value;
}

void MainWindow::on_hs_lv_fan_gpu_valueChanged(int value)
{
    th_fansvc_gpu->bzh_dell_fan_FanService_pkg.level_fan = value;
}

void MainWindow::on_pb_test_clicked()
{

    qDebug() << dell_smm_io(0x05a3 , 0x0002);

    //qDebug() << dell_smm_io_get_fan_nom_rpm(1,2);
}

void MainWindow::on_sb_override_delay_config_valueChanged(int arg1)
{
    th_fansvc_cpu->bzh_dell_fan_FanService_pkg.override_delay = arg1;
    th_fansvc_gpu->bzh_dell_fan_FanService_pkg.override_delay = arg1;
}

void MainWindow::on_sb_sensor_refresh_delay_config_valueChanged(int arg1)
{
    th_sensorsvc->bzh_dell_fan_SensorService_pkg.refresh_delay = arg1;
}


void MainWindow::on_pb_disable_ec_ctrl_clicked()
{
    if(msgbox->exec() == QMessageBox::Yes)
    {
        ui->gb_override_protect->setEnabled(false);
        ui->pb_disable_ec_ctrl->setEnabled(false);
        ui->pb_disable_ec_ctrl->setText("EC control disabled");

        //shutdonw ec control code here

        dell_smm_io(DELL_SMM_IO_DISABLE_FAN_CTL1,DELL_SMM_IO_NO_ARG);
        dell_smm_io_set_fan_lv(DELL_SMM_IO_FAN1,DELL_SMM_IO_FAN_LV1);

    }
}

void MainWindow::on_hs_pwm_fan_gpu_valueChanged(int value)
{
    th_fanpwmsvc_gpu->bzh_dell_fan_FanPWMService_pkg.duty_cycle = value;
    emit signal_duty_cycle_changed();
}

void MainWindow::on_rb_override_clicked()
{
    th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_only_mode = true;

    th_fansvc_cpu->bzh_dell_fan_FanService_pkg.loop = true;
    if(!th_fansvc_cpu->isRunning())
    {
        th_fansvc_cpu->start();
    }
    th_fansvc_gpu->bzh_dell_fan_FanService_pkg.loop = true;
    if(!th_fansvc_gpu->isRunning())
    {
        th_fansvc_gpu->start();
    }
}

void MainWindow::on_rb_none_clicked()
{
    th_sensorsvc->bzh_dell_fan_SensorService_pkg.rpm_only_mode = false;

    if(th_fansvc_cpu->isRunning())
    {
        th_fansvc_cpu->quit();
    }
    if(th_fansvc_gpu->isRunning())
    {
        th_fansvc_gpu->quit();
    }

}


void MainWindow::on_sb_pwm_single_cycle_time_valueChanged(int arg1)
{
    th_fanpwmsvc_gpu->bzh_dell_fan_FanPWMService_pkg.single_cycle_time = arg1;
    emit signal_duty_cycle_changed();
}
