#ifndef BZH_DELL_FAN_UTILITY_TH_H
#define BZH_DELL_FAN_UTILITY_TH_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::th_init()
{
    th_fansvc_cpu = new FanService;
    th_fansvc_gpu = new FanService;
    th_fanpwmsvc_cpu = new FanPWMService;
    th_fanpwmsvc_gpu = new FanPWMService;
    th_sensorsvc = new SensorService;

    th_fansvc_cpu->bzh_dell_fan_FanService_pkg_init(DELL_SMM_IO_FAN1);
    th_fansvc_gpu->bzh_dell_fan_FanService_pkg_init(DELL_SMM_IO_FAN2);
    th_fanpwmsvc_cpu->bzh_dell_fan_FanPWMService_pkg_init(DELL_SMM_IO_FAN1);
    th_fanpwmsvc_gpu->bzh_dell_fan_FanPWMService_pkg_init(DELL_SMM_IO_FAN2);
    th_sensorsvc->bzh_dell_fan_SensorService_pkg_init();

    QObject::connect(this,
                     SIGNAL(signal_duty_cycle_changed()),
                     th_fanpwmsvc_gpu,
                     SLOT(slot_duty_cycle_chaned()));

    QObject::connect(th_sensorsvc,
                     SIGNAL(signal_sensor_inti_ready()),
                     this,
                     SLOT(slot_sensor_init_ready()));

    QObject::connect(th_sensorsvc,
                     SIGNAL(signal_sensor_data_ready()),
                     this,
                     SLOT(slot_sensor_data_ready()));

    th_fansvc_cpu->bzh_dell_fan_FanService_pkg.override_delay = ui->sb_override_delay_config->value();
    th_fansvc_gpu->bzh_dell_fan_FanService_pkg.override_delay = ui->sb_override_delay_config->value();
    th_sensorsvc->bzh_dell_fan_SensorService_pkg.refresh_delay = ui->sb_sensor_refresh_delay_config->value();

    th_sensorsvc->bzh_dell_fan_SensorService_pkg.loop = true;
    th_sensorsvc->start();
}

void MainWindow::th_cleanup()
{
    th_fansvc_cpu->quit();
    th_fansvc_gpu->quit();
    th_fanpwmsvc_cpu->quit();
    th_fanpwmsvc_gpu->quit();
    th_sensorsvc->quit();

    th_fansvc_cpu->wait();
    th_fansvc_gpu->wait();
    th_fanpwmsvc_cpu->wait();
    th_fanpwmsvc_gpu->wait();
    th_sensorsvc->wait();

    delete th_fansvc_cpu;
    delete th_fansvc_gpu;
    delete th_fanpwmsvc_cpu;
    delete th_fanpwmsvc_gpu;
    delete th_sensorsvc;
}

#endif // BZH_DELL_FAN_UTILITY_TH_H

