/*
 * @brief DCX configuration file
 * @file dcx_configuration.h
 *
 * Various parameters and settings related to the DCX converter. This file has
 * settings for the dcx converter such as freqeuncy, duty, protection limits,
 * control loop references etc.
 */

#ifndef SRC_USER_DCX_CONFIG_H_
#define SRC_USER_DCX_CONFIG_H_

/* Macros ------------------------------------------------------------------ */

       /* Converter Build Options */

/* High resolution enabled? */
#define HIGH_RESOLUTION_PWM                       ( 1 )

#ifndef SYSTEM_TEST_LEVEL
 #define OPEN_LOOP_CONTROL                        ( 0 )
 #define CLOSED_LOOP_CONTROL                      ( 1 )
 #define SYSTEM_TEST_LEVEL                        OPEN_LOOP_CONTROL
#endif

      /* DAB PWM configuration */

/* Leading and Lagging independent */
#define INDEPENDENT_CONTROL                      ( 0 )

/* Leading and Lagging symmetric */
#define SYMMETRIC_CONTROL                        ( 1 )
#define DAB_PWM_CONTROL                          ( SYMMETRIC_CONTROL )

#if ( DAB_PWM_CONTROL == SYMMETRIC_CONTROL )
  /* Make the line 1 if you want the fix to be applied */
  /* The fix is for old UDS rev1.0 board in which PWM4B was not being output on the connector */
  #define DAB_PWM_FIX                            ( 1 )
#endif

/* TODO To move PI a different file */
#define PI                                       ( 3.14159265358979323846f )

/* Converter Parameters and Limits */
#define dcxconfigDEADTIME_PRY_NS                 ( 150.0f )
#define dcxconfigDEADTIME_SEC_NS                 ( 150.0f )
#define dcxconfigDEADTIME_MAX_NS                 ( 500.0f )
#define dcxconfigDEADTIME_MIN_NS                 ( 50.0f )
#define dcxconfigCONV_SW_FREQ_KHZ                ( 100.0f )
#define dcxconfigFREQ_MAX_KHZ                    ( 1000.0f )
#define dcxconfigFREQ_MIN_KHZ                    ( 1.0f )
#define dcxconfigDCX_DUTY_RATIO                  ( 0.5f )
#define dcxconfigDUTY_MAX                        ( 0.5f )
#define dcxconfigDUTY_MIN                        ( 0.5f )
// #define dcxconfigINITIAL_PHASE_RAD               ( 0.3451f )
#define dcxconfigINITIAL_PHASE_RAD               ( 0.4f ) /* for COMPEL */
#define dcxconfigPHASE_SHIFT_MAX_RAD             ( PI/3.0f )
#define dcxconfigPHASE_SHIFT_MIN_RAD             ( -PI/3.0f )

/* Converter Control References: Default */
#define dcxconfigMRATIO_REF                      ( commonDCX2_RATIO )

/*
 * Controller sampling frequency must be a divider of the switching frequency
 * and cannot be smaller than 15 times the switching freq (ADC Trigger limit).
 * It also cannot be larger than the converter switching frequency.
 */
#define dcxconfigSAMPLING_FREQ_KHZ               ( dcxconfigCONV_SW_FREQ_KHZ )
#define dcxconfigSAMPLING_FREQ_DIV               (( uint16_t )( dcxconfigCONV_SW_FREQ_KHZ / dcxconfigSAMPLING_FREQ_KHZ ))

/* Converter Sensor Gains
 * Vadc = TrueValue * Gain + Offset */

/* Vout */
#define dcxconfigVOUT_SENSOR_GAIN               ( 0.00125f )
#define dcxconfigVOUT_SENSOR_OFFSET             ( 0.375f )

/* Vin */
#define dcxconfigVIN_SENSOR_GAIN                ( 0.00125f )
#define dcxconfigVIN_SENSOR_OFFSET              ( 0.375f )

/* Itank */
#define dcxconfigITANK_SENSOR_GAIN              ( 0.0025f )
#define dcxconfigITANK_SENSOR_OFFSET            ( 0.0f )

/* Converter Protection Limits */
#define dcxconfigDCX_OVER_VOLTAGE_LIMIT          ( 800.0f )
#define dcxconfigMIN_VIN_THRESHOLD               ( commonDCX2_VHYST * dcxconfigVIN_SENSOR_GAIN - dcxconfigVOUT_SENSOR_OFFSET )

#endif /* SRC_USER_DCX_CONFIG_H_ */
