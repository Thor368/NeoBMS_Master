/*####################################################*
 *############# Battery Description File #############*
 *####################################################*
 *Example file - Check before use!

*/
#include "IO.h"

#define BMS_chip_count  3         // Number of BMS chips
extern uint32_t BMS_cell_count[BMS_chip_count];  // Number of Cells
extern uint32_t BMS_temp_count[BMS_chip_count];  // Number of Temperature Sensors
extern uint32_t Cell_count;
extern int32_t BMS_OV;


#define BMS_std_OV      4100000   // [µV] Standard Over Voltage Limit
#define BMS_full_OV     4250000   // [µV] Full charge Over Voltage Limit

#define BMS_OV_Delay    10000     // [ms] Soft Cell Overvoltage Delay
#define BMS_hard_OV     4300000   // [µV] Hard Cell Overvoltage Threshold
#define BMS_OV_recovery 95        // [%]  Percentage of OV Limit where charging is reenabled
#define BMS_Balance_U   3900000   // [µV] Voltage at which balancing is enabled

#define BMS_soft_UV     2500000   // [µV] Soft Cell Undervoltage Threshold
#define BMS_UV_Delay    10000     // [ms] Soft Cell Undervoltage Delay
#define BMS_hard_UV     2450000   // [µV] Hard Cell Undervoltage Threshold
#define BMS_UV_recovery 3000000   // [µV] Voltage at which discharging is reenabled

#define BMS_soft_OC     400000    // [mA] Soft Overcurrent Threshold
#define BMS_OC_Delay    3000      // [ms] Soft Overcurrent Delay
#define BMS_hard_OC     1000000   // [mA] Hard Overcurrent Threshold

#define BMS_soft_COT    42000000  // [°C] Soft Cell Overtemperature while Charging Threshold
#define BMS_COT_Delay   3000      // [ms] Soft Cell Overtemperature while Charging Delay
#define BMS_hard_COT    45000000  // [°C] Hard Cell Overtemperature while Charging Threshold

#define BMS_soft_DOT    55000000  // [°C] Soft Cell Overtemperature while Discharging Threshold
#define BMS_DOT_Delay   3000      // [ms] Soft Cell Overtemperature while Discharging Delay
#define BMS_hard_DOT    60000000  // [°C] Hard Cell Overtemperature while Discharging Threshold

#define BMS_soft_UT     -20000000 // [°C] Soft Cell Overtemperature while Discharging Threshold
#define BMS_UT_Delay    3000      // [ms] Soft Cell Overtemperature while Discharging Delay
#define BMS_hard_UT     -20000000 // [°C] Hard Cell Overtemperature while Discharging Threshold

#define BMS_ITMP_LIM    70        // [°C] Temperature limit while balancing
#define BMS_ITMP_HYST   8         // [°C] Hysteresis for temperature limit

#define BMS_I_SENSOR    1         // Index of BMS that has the current sensor
#define BMS_C_PER_MA    9.0909090909090909090909090909091  // [mA/c] Counts per mA

#define BMS_Temp_c0     25.3346507708
#define BMS_Temp_c1     0.0103492102
#define BMS_Temp_c2     0.000000121738793651066


#define BMS_fault_delay        10000            // Set the delay before tripping the following IOs
//#define BMS_latch_fault                         // Latch a fault state until reset
#define BMS_enable_charge      IO_PO_2_OFF       // Define which IO is for Charge Enable
#define BMS_disable_charge     IO_PO_2_ON
#define BMS_enable_discharge   IO_PO_1_ON       // Define which IO is for Discharge Enable
#define BMS_disable_discharge  IO_PO_1_OFF
//#define BMS_set_fault          IO_PO_2_OFF      // Define which IO is the FAULT output
//#define BMS_reset_fault        IO_PO_2_ON

void Battery_Init();
