
#include <qDebug>

#ifdef __cplusplus
extern "C"
{
#endif

#include "bzh_dell_smm_io_drv_fun.h"

SMBIOS_PKG smm_pkg;

#include <stdio.h>
#include <stdlib.h>

void display_error(ULONG errorcode)
{
    qDebug("Error code:%lu \n",errorcode);
    switch (errorcode)
    {
    case 0x7a:
        qDebug("ERROR_INSUFFICIENT_BUFFER \n");
        break;
    case 0x57:
        qDebug("ERROR_INVALID_PARAMETER \n");
        break;
    case 0x1f:
        qDebug("ERROR_GEN_FAILURE \n");
        break;
    default:
        qDebug("UNKNOW ERROR \n");
        break;
    }
}

ULONG _stdcall dell_smm_io(ULONG cmd, ULONG data)
{
    if (!IsInitialized)
    {
        return 0;
    }

    smm_pkg.cmd = cmd;
    smm_pkg.data = data;
    smm_pkg.stat1 = 0;
    smm_pkg.stat2 = 0;

    ULONG result_size = 0;

    bool status_dic = DeviceIoControl(hDriver,
                                      IOCTL_BZH_DELL_SMM_RWREG,
                                      &smm_pkg,
                                      sizeof(SMBIOS_PKG),
                                      &smm_pkg,
                                      sizeof(SMBIOS_PKG),
                                      &result_size,
                                      NULL);

    if(status_dic == false)
    {
        ULONG lsterr = GetLastError();
        display_error(lsterr);
        return 0;
    }
    else
    {
        return smm_pkg.cmd;
    }
}

void dell_smm_io_set_fan_lv(ULONG fan_no, ULONG lv)
{
    ULONG arg = (lv<<8) | fan_no;
    dell_smm_io(DELL_SMM_IO_SET_FAN_LV,arg);
}

ULONG dell_smm_io_get_fan_lv(ULONG fan_no)
{
    return dell_smm_io(DELL_SMM_IO_GET_FAN_LV,fan_no);
}

ULONG dell_smm_io_get_fan_rpm(ULONG fan_no)
{
    return dell_smm_io(DELL_SMM_IO_GET_FAN_RPM,fan_no);
}

ULONG dell_smm_io_get_fan_nom_rpm(ULONG fan_no, ULONG lv)
{
    ULONG arg = (lv<<8) | fan_no;
    return dell_smm_io(DELL_SMM_IO_GET_FAN_NOM_RPM,arg);
}

#ifdef __cplusplus
}
#endif
