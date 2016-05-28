
#include "bzh_dell_smm_io_fan_opt.h"

Qt::HANDLE smm_lock_holder = false;

void get_smm_lock(Qt::HANDLE tid)
{
    if(smm_lock_holder == 0)
    {
        smm_lock_holder = tid;
    }
}

void release_smm_lock(Qt::HANDLE tid)
{
    if(smm_lock_holder == tid)
    {
        smm_lock_holder = 0;
    }
}

bool check_smm_lock(Qt::HANDLE tid)
{
    if(smm_lock_holder == tid)
    {
        return true;
    }
    else
    {
        return false;
    }
}
