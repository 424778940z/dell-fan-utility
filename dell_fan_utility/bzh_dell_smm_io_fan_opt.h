#ifndef BZH_DELL_SMM_IO_FAN_OPT_H
#define BZH_DELL_SMM_IO_FAN_OPT_H

#include <QDebug>
#include <QThread>

#include <math.h>


#include "bzh_dell_smm_io_drv_fun.h"

extern Qt::HANDLE smm_lock_holder;

extern void get_smm_lock(Qt::HANDLE tid);
extern void release_smm_lock(Qt::HANDLE tid);
extern bool check_smm_lock(Qt::HANDLE tid);


class FanService : public QThread
{
    Q_OBJECT

public:

    struct bzh_dell_fan_FanService_pkg
    {
        bool loop;
        unsigned int override_delay;
        unsigned int level_fan;
        unsigned int no_fan;
    }
    bzh_dell_fan_FanService_pkg;

    void bzh_dell_fan_FanService_pkg_init(unsigned int no_fan)
    {
        bzh_dell_fan_FanService_pkg.loop = false;
        bzh_dell_fan_FanService_pkg.override_delay = 1000;
        bzh_dell_fan_FanService_pkg.level_fan = 0;
        bzh_dell_fan_FanService_pkg.no_fan = no_fan;
    }

    void run()
    {
        qDebug() << "FanService Start" << this->currentThreadId();
        if(!bzh_dell_fan_FanService_pkg.loop)
        {
            //one time code here
            dell_smm_io_set_fan_lv(bzh_dell_fan_FanService_pkg.no_fan,
                                   bzh_dell_fan_FanService_pkg.level_fan);
        }
        while(bzh_dell_fan_FanService_pkg.loop)
        {
            //delay
            this->msleep(bzh_dell_fan_FanService_pkg.override_delay);

            //get lock
            get_smm_lock(this->currentThreadId());

            //check lock
            if(check_smm_lock(this->currentThreadId()))
            {
                //loop code here
                dell_smm_io_set_fan_lv(bzh_dell_fan_FanService_pkg.no_fan,
                                       bzh_dell_fan_FanService_pkg.level_fan);
            }

            //release lock
            release_smm_lock(this->currentThreadId());
        }
        qDebug() << "FanService Stop" << this->currentThreadId();
    }

    void quit()
    {
        bzh_dell_fan_FanService_pkg.loop = false;
    }
};

class FanPWMService : public QThread
{
    Q_OBJECT

public:

    struct bzh_dell_fan_FanPWMService_pkg
    {
        bool loop;
        unsigned int duty_cycle;
        unsigned int single_cycle_time;
        unsigned int high_delay;
        unsigned int low_delay;
        unsigned int min_rpm_fan;
        unsigned int mid_rpm_fan;
        unsigned int max_rpm_fan;
        unsigned int current_rpm_fan;
        unsigned int expect_rpm_fan;
        unsigned int low_level_fan;
        unsigned int high_level_fan;
        unsigned int no_fan;
    }
    bzh_dell_fan_FanPWMService_pkg;

    void bzh_dell_fan_FanPWMService_pkg_init(unsigned int no_fan)
    {
        bzh_dell_fan_FanPWMService_pkg.loop = false;
        bzh_dell_fan_FanPWMService_pkg.duty_cycle = 50;
        bzh_dell_fan_FanPWMService_pkg.single_cycle_time = 10;
        bzh_dell_fan_FanPWMService_pkg.high_delay = 0;
        bzh_dell_fan_FanPWMService_pkg.low_delay = 0;
        bzh_dell_fan_FanPWMService_pkg.low_level_fan = 0;
        bzh_dell_fan_FanPWMService_pkg.high_level_fan = 0;
        bzh_dell_fan_FanPWMService_pkg.min_rpm_fan = 0;
        bzh_dell_fan_FanPWMService_pkg.mid_rpm_fan = 0;
        bzh_dell_fan_FanPWMService_pkg.max_rpm_fan = 0;
        bzh_dell_fan_FanPWMService_pkg.current_rpm_fan = 0;
        bzh_dell_fan_FanPWMService_pkg.expect_rpm_fan = 0;
        bzh_dell_fan_FanPWMService_pkg.no_fan = no_fan;
    }

    void run()
    {
        qDebug() << "FanSPWMervice Start" << this->currentThreadId();

        bzh_dell_fan_FanPWMService_pkg.min_rpm_fan = dell_smm_io_get_fan_nom_rpm(bzh_dell_fan_FanPWMService_pkg.no_fan,0);
        bzh_dell_fan_FanPWMService_pkg.mid_rpm_fan = dell_smm_io_get_fan_nom_rpm(bzh_dell_fan_FanPWMService_pkg.no_fan,1);
        bzh_dell_fan_FanPWMService_pkg.max_rpm_fan = dell_smm_io_get_fan_nom_rpm(bzh_dell_fan_FanPWMService_pkg.no_fan,2);

        if(!bzh_dell_fan_FanPWMService_pkg.loop)
        {
            //one time code here
        }
        while(bzh_dell_fan_FanPWMService_pkg.loop)
        {
            //get lock
            get_smm_lock(this->currentThreadId());

            //check lock
            if(check_smm_lock(this->currentThreadId()))
            {
                //loop code here
                bzh_dell_fan_FanPWMService_pkg.current_rpm_fan = dell_smm_io_get_fan_rpm(bzh_dell_fan_FanPWMService_pkg.no_fan);
                if(bzh_dell_fan_FanPWMService_pkg.current_rpm_fan != 0)
                {

                    qDebug() << bzh_dell_fan_FanPWMService_pkg.current_rpm_fan;

                    if(bzh_dell_fan_FanPWMService_pkg.current_rpm_fan < bzh_dell_fan_FanPWMService_pkg.expect_rpm_fan)
                    {
                        dell_smm_io_set_fan_lv(bzh_dell_fan_FanPWMService_pkg.no_fan,bzh_dell_fan_FanPWMService_pkg.high_level_fan);
                        //high delay
                        this->usleep(bzh_dell_fan_FanPWMService_pkg.high_delay);
                    }

                    if(bzh_dell_fan_FanPWMService_pkg.current_rpm_fan > bzh_dell_fan_FanPWMService_pkg.expect_rpm_fan)
                    {
                        dell_smm_io_set_fan_lv(bzh_dell_fan_FanPWMService_pkg.no_fan,bzh_dell_fan_FanPWMService_pkg.low_level_fan);
                        //low delay
                        this->usleep(bzh_dell_fan_FanPWMService_pkg.low_delay);
                    }
                }
                else
                {
                    for(uint i; i < 10;i++)
                    {
                        if(dell_smm_io_get_fan_rpm(bzh_dell_fan_FanPWMService_pkg.no_fan) == 0)
                        {
                            qDebug() << "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
                            i++;
                        }
                    }
                    dell_smm_io_set_fan_lv(bzh_dell_fan_FanPWMService_pkg.no_fan,bzh_dell_fan_FanPWMService_pkg.high_level_fan);
                    msleep(10);
                }
            }

            //release lock
            release_smm_lock(this->currentThreadId());
        }
        qDebug() << "FanSPWMervice Stop" << thread()->currentThreadId();
    }

    void quit()
    {
        bzh_dell_fan_FanPWMService_pkg.loop = false;
    }

public slots:
    void slot_duty_cycle_chaned()
    {
        bzh_dell_fan_FanPWMService_pkg.high_delay = round(bzh_dell_fan_FanPWMService_pkg.single_cycle_time*1000*bzh_dell_fan_FanPWMService_pkg.duty_cycle/100);
        bzh_dell_fan_FanPWMService_pkg.low_delay = round(bzh_dell_fan_FanPWMService_pkg.single_cycle_time*1000*(100-bzh_dell_fan_FanPWMService_pkg.duty_cycle)/100);
        bzh_dell_fan_FanPWMService_pkg.expect_rpm_fan = round((bzh_dell_fan_FanPWMService_pkg.max_rpm_fan-bzh_dell_fan_FanPWMService_pkg.min_rpm_fan)*bzh_dell_fan_FanPWMService_pkg.duty_cycle/100);

        if(bzh_dell_fan_FanPWMService_pkg.expect_rpm_fan > bzh_dell_fan_FanPWMService_pkg.mid_rpm_fan)
        {
            bzh_dell_fan_FanPWMService_pkg.high_level_fan = DELL_SMM_IO_FAN_LV2;
            bzh_dell_fan_FanPWMService_pkg.low_level_fan = DELL_SMM_IO_FAN_LV1;
        }
        if(bzh_dell_fan_FanPWMService_pkg.expect_rpm_fan < bzh_dell_fan_FanPWMService_pkg.mid_rpm_fan)
        {
            bzh_dell_fan_FanPWMService_pkg.high_level_fan = DELL_SMM_IO_FAN_LV1;
            bzh_dell_fan_FanPWMService_pkg.low_level_fan = DELL_SMM_IO_FAN_LV0;
        }

        qDebug() << bzh_dell_fan_FanPWMService_pkg.high_delay
                 << bzh_dell_fan_FanPWMService_pkg.low_delay
                 << bzh_dell_fan_FanPWMService_pkg.high_level_fan
                 << bzh_dell_fan_FanPWMService_pkg.low_level_fan
                 << bzh_dell_fan_FanPWMService_pkg.single_cycle_time
                 << bzh_dell_fan_FanPWMService_pkg.expect_rpm_fan;
    }
};

class SensorService : public QThread
{
    Q_OBJECT

signals:

    void signal_sensor_inti_ready();
    void signal_sensor_data_ready();

public:

    struct bzh_dell_fan_SensorService_pkg
    {
        bool loop;
        bool init_ready;
        bool data_ready;
        bool rpm_only_mode;
        unsigned int refresh_delay;
        unsigned int lv_cpu_fan;
        unsigned int lv_gpu_fan;
        unsigned int rpm_cpu_fan_min;
        unsigned int rpm_cpu_fan_mid;
        unsigned int rpm_cpu_fan_max;
        unsigned int rpm_cpu_fan_current;
        unsigned int rpm_gpu_fan_min;
        unsigned int rpm_gpu_fan_mid;
        unsigned int rpm_gpu_fan_max;
        unsigned int rpm_gpu_fan_current;
        unsigned int temp_cpu;
        unsigned int temp_gpu;
    }
    bzh_dell_fan_SensorService_pkg;

    void bzh_dell_fan_SensorService_pkg_init()
    {
        bzh_dell_fan_SensorService_pkg.loop = false;
        bzh_dell_fan_SensorService_pkg.init_ready = false;
        bzh_dell_fan_SensorService_pkg.data_ready = false;
        bzh_dell_fan_SensorService_pkg.rpm_only_mode = false;
        bzh_dell_fan_SensorService_pkg.refresh_delay = 0;
        bzh_dell_fan_SensorService_pkg.lv_cpu_fan = 0;
        bzh_dell_fan_SensorService_pkg.lv_gpu_fan = 0;
        bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_min = 0;
        bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_mid = 0;
        bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_max = 0;
        bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_current = 0;
        bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_min = 0;
        bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_mid = 0;
        bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_max = 0;
        bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_current = 0;
        bzh_dell_fan_SensorService_pkg.temp_cpu = 0;
        bzh_dell_fan_SensorService_pkg.temp_gpu = 0;
    }

    void run()
    {
        qDebug() << "SensorService Start" << thread()->currentThreadId();

        //init
        bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_min = dell_smm_io_get_fan_nom_rpm(DELL_SMM_IO_FAN1,DELL_SMM_IO_FAN_LV0);
        bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_mid = dell_smm_io_get_fan_nom_rpm(DELL_SMM_IO_FAN1,DELL_SMM_IO_FAN_LV1);
        bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_max = dell_smm_io_get_fan_nom_rpm(DELL_SMM_IO_FAN1,DELL_SMM_IO_FAN_LV2);

        bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_min = dell_smm_io_get_fan_nom_rpm(DELL_SMM_IO_FAN2,DELL_SMM_IO_FAN_LV0);
        bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_mid = dell_smm_io_get_fan_nom_rpm(DELL_SMM_IO_FAN2,DELL_SMM_IO_FAN_LV1);
        bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_max = dell_smm_io_get_fan_nom_rpm(DELL_SMM_IO_FAN2,DELL_SMM_IO_FAN_LV2);

        bzh_dell_fan_SensorService_pkg.init_ready = true;
        emit signal_sensor_inti_ready();

        //main fun
        if(!bzh_dell_fan_SensorService_pkg.loop)
        {
            //one time code here
        }
        while(bzh_dell_fan_SensorService_pkg.loop)
        {
            //delay
            this->msleep(bzh_dell_fan_SensorService_pkg.refresh_delay);

            //get lock
            get_smm_lock(this->currentThreadId());

            //check lock
            if(check_smm_lock(this->currentThreadId()))
            {
                //loop code here
                if(bzh_dell_fan_SensorService_pkg.rpm_only_mode)
                {
                    bzh_dell_fan_SensorService_pkg.data_ready = false;
                    bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_current = dell_smm_io_get_fan_rpm(DELL_SMM_IO_FAN1);
                    bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_current = dell_smm_io_get_fan_rpm(DELL_SMM_IO_FAN2);

                    if(     bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_current != 0 &&
                            bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_current != 0)
                    {
                        bzh_dell_fan_SensorService_pkg.data_ready = true;
                        emit signal_sensor_data_ready();
                    }
                }
                else
                {
                    bzh_dell_fan_SensorService_pkg.data_ready = false;
                    bzh_dell_fan_SensorService_pkg.lv_cpu_fan = dell_smm_io_get_fan_lv(DELL_SMM_IO_FAN1);
                    bzh_dell_fan_SensorService_pkg.lv_gpu_fan = dell_smm_io_get_fan_lv(DELL_SMM_IO_FAN2);
                    bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_current = dell_smm_io_get_fan_rpm(DELL_SMM_IO_FAN1);
                    bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_current = dell_smm_io_get_fan_rpm(DELL_SMM_IO_FAN2);

                    if(
                            (bzh_dell_fan_SensorService_pkg.lv_cpu_fan == 0 &&
                            bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_current == 0) ||
                            (bzh_dell_fan_SensorService_pkg.lv_gpu_fan == 0 &&
                            bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_current == 0) ||

                            (bzh_dell_fan_SensorService_pkg.rpm_cpu_fan_current != 0 ||
                            bzh_dell_fan_SensorService_pkg.rpm_gpu_fan_current !=0)
                            )
                    {
                        bzh_dell_fan_SensorService_pkg.data_ready = true;
                        emit signal_sensor_data_ready();
                    }
                }

            }

            //release lock
            release_smm_lock(this->currentThreadId());
        }

        qDebug() << "SensorService Stop" << thread()->currentThreadId();
    }

    void quit()
    {
        bzh_dell_fan_SensorService_pkg.loop = false;
    }
};



#endif // BZH_DELL_SMM_IO_FAN_OPT_H

