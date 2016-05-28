#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
//#include <QObject>
#include <QAbstractSlider>
#include <QMessageBox>

#include "bzh_dell_smm_io_fan_opt.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void slot_sensor_init_ready();
    void slot_sensor_data_ready();

signals:
    void signal_duty_cycle_changed();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_hs_lv_fan_gpu_valueChanged(int value);
    void on_hs_lv_fan_cpu_valueChanged(int value);

    void on_sb_override_delay_config_valueChanged(int arg1);
    void on_sb_sensor_refresh_delay_config_valueChanged(int arg1);

    void on_pb_test_clicked();

    void on_pb_pwm_clicked();

    void on_pb_dc_refresh_clicked();


    void on_pb_disable_ec_ctrl_clicked();

    void on_hs_pwm_fan_gpu_valueChanged(int value);

    void on_rb_override_clicked();

    void on_rb_none_clicked();

    void on_sb_pwm_single_cycle_time_valueChanged(int arg1);

private:
    FanService *th_fansvc_cpu;
    FanService *th_fansvc_gpu;
    FanPWMService *th_fanpwmsvc_cpu;
    FanPWMService *th_fanpwmsvc_gpu;
    SensorService *th_sensorsvc;
    Ui::MainWindow *ui;

    QMessageBox *msgbox;

    void drv_init();
    void drv_cleanup();
    void th_init();
    void th_cleanup();
    void ui_init();
    void ui_cleanup();
};

#endif // MAINWINDOW_H
