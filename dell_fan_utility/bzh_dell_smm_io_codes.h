#ifndef BZH_DELL_SMM_IO_CODES
#define BZH_DELL_SMM_IO_CODES

/* This is some information about the codes for different functions on the Dell SMBIOS */

/*

  cmd     function  (thanks to Andrew Paprocki)
  0x00a3  get current speed indicator of a fan (args: fan)
  0x01a3  set speed of a fan (args: fan & speed)
  0x02a3  get RPM of a fan (args: fan)

  0x04a3  get nominal fan speed (2 args)



  http://brouille.tuxfamily.org/junk/fan.c

  0x03a3  ??? (1 byte)
  0x05a3  get fan tolerance speed (2 args)
  0x10a3  get sensor temperature (1 arg: sensor#)
  0x11a3  ???
  0x12a3  arg 0x0003=NBSVC-Query
          arg 0x0000=NBSVC-Clear
          arg 0x122=NBSVC-Start Trend
          arg 0x0100=NBSVC-Stop Trend
          arg 0x02??=NBSVC-Read
  0x21a3  ??? (2 args: 1 byte (oder 0x16) + 1 byte)
  0x22a3  get charger info (1 arg)
  0x23a3  ??? (4 args: 2x 1 byte, 1xword, 1xdword)
  0x24a3  get adaptor info status (1 arg oder 0x03)
  0x30a3  ??? (no args)
  0x31a3  ??? (no args)
  0x32a3  ??? (no args)
  0x33a3  ??? (no args)
  0x36a3  get hotkey scancode list (args see diags)
  0x37a3  ??? (no args)
  0x40a3  get docking state (no args)
  0xf0a3  ??? (2 args)
  0xfea3  check SMBIOS version (1 arg)
  0xffa3  check SMBIOS interface (returns:"DELLDIAG")
*/

/* The codes for DISABLE_BIOS_* were obtained experimentally on a E6420 with the
 * following algorithm:
 * probing many codes in a loop
 * putting the speed to maximum, sleeping some seconds, and checking the speed back.
 * Check the function probecodes()
 */

//CMD

#define DELL_SMM_IO_SET_FAN_LV          0x01a3
#define DELL_SMM_IO_GET_FAN_LV          0x00a3
#define DELL_SMM_IO_GET_FAN_RPM         0x02a3

#define DELL_SMM_IO_GET_TEMP            0x10a3
#define DELL_SMM_IO_TEMP_TYPE           0x11a3

#define DELL_SMM_IO_DELLSIG1            0xfea3
#define DELL_SMM_IO_DELLSIG2            0xffa3

#define DELL_SMM_IO_DISABLE_FAN_CTL1    0x30a3 //不自动调速
#define DELL_SMM_IO_ENABLE_FAN_CTL1     0x31a3
#define DELL_SMM_IO_DISABLE_FAN_CTL2    0x34a3 //彻底罢工,没指令就不转
#define DELL_SMM_IO_ENABLE_FAN_CTL2     0x35a3

#define DELL_SMM_IO_GET_FAN_TYPE        0x03a3
#define DELL_SMM_IO_GET_FAN_NOM_RPM     0x04a3

//ARG

#define DELL_SMM_IO_FAN_LV0             0x0000
#define DELL_SMM_IO_FAN_LV1             0x0001
#define DELL_SMM_IO_FAN_LV2             0x0002

#define DELL_SMM_IO_FAN1                0
#define DELL_SMM_IO_FAN2                1

/*
#define DELL_SMM_IO_FAN1_LV0    0
#define DELL_SMM_IO_FAN1_LV1    0
#define DELL_SMM_IO_FAN1_LV2    0

#define DELL_SMM_IO_FAN2_LV0    0
#define DELL_SMM_IO_FAN2_LV1    0
#define DELL_SMM_IO_FAN2_LV2    0

*/
#define DELL_SMM_IO_NO_ARG      0x0



//OLD

#define I8K_SMM_SET_TEMP	0x0

#define SMM_DISABLE_BIOS_METHOD1 0x30a3
#define SMM_ENABLE_BIOS_METHOD1  0x31a3
#define SMM_DISABLE_BIOS_METHOD2 0x34a3
#define SMM_ENABLE_BIOS_METHOD2  0x35a3
#define SMM_ENABLE_FN            0x32a3
#define SMM_FN_STATUS            0x0025
#define SMM_DELL_DIAG            0xffa3


#define I8K_SMM_GET_DELL_SIG1
#define I8K_SMM_GET_DELL_SIG2

#define I8K_SMM_GET_FAN_NOM_SPEED

#define I8K_FAN_MULT		30
#define I8K_FAN_MAX_RPM		30000
#define I8K_MAX_TEMP		127

#define I8K_FN_NONE		0x00
#define I8K_FN_UP		0x01
#define I8K_FN_DOWN		0x02
#define I8K_FN_MUTE		0x04
#define I8K_FN_MASK		0x07
#define I8K_FN_SHIFT		8
#define I8K_SMM_FN_STATUS	0x0025

#define I8K_POWER_AC		0x05
#define I8K_POWER_BATTERY	0x01

#endif // BZH_DELL_SMM_IO_CODES

