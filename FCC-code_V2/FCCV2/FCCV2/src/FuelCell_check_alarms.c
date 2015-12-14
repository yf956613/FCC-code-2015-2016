/*
 * FuelCell_check_alarms.c
 *
 * Created: 2015-12-13 4:16:34 PM
 *  Author: Reegan
 */ 

#include "asf.h"
#include "FuelCell_Functions.h"
#include "FC_error_codes.h"
#include "digital_IO_defs.h"
#include "FuelCell_ADC.h"
#include "FuelCell_PWM.h"
#include <math.h>
#include "pid.h"
#include "millis_function.h"
#include "FuelCell_check_alarms.h"

unsigned int FC_check_alarms(unsigned int fc_state)
{
	unsigned int error_msg = 0;
	//check fccon sysok capcon always
	if(gpio_get_pin_value(FCCON) == 0)
	{
		error_msg |= FC_ERR_FC_DISC;
	}
	if(gpio_get_pin_value(CAPCON) == 0)
	{
		error_msg |= FC_ERR_CAP_DISC;
	}
	if(gpio_get_pin_value(SYSOK) == 0)
	{
		error_msg |= FC_ERR_H2OK_LOW;
	}
	//check temp H L and pressure H always
	if((get_FCTEMP1() > HIGH_TEMP_THRES) | (get_FCTEMP2() > HIGH_TEMP_THRES))
	{
		error_msg |= FC_ERR_TEMP_H;
	}
	if((get_FCTEMP1() < LOW_TEMP_THRES) | (get_FCTEMP2() < LOW_TEMP_THRES))
	{
		error_msg |= FC_ERR_TEMP_L;
	}
	if(get_FCPRES() > FC_HIGH_PRES_THRES)
	{
		error_msg |= FC_ERR_PRES_H;
	}
	switch (fc_state)
	{
		//case FC_STATE_STANDBY:
		//case FC_STATE_SHUTDOWN:
		
		case FC_STATE_STARTUP_FANS:
		if(gpio_get_pin_value(RESCON) == 0)
		{
			error_msg |= FC_ERR_RES_DISC;
		}
		
		case FC_STATE_STARTUP_H2:
		if(gpio_get_pin_value(RESCON) == 0)
		{
			error_msg |= FC_ERR_RES_DISC;
		}
		
		case FC_STATE_STARTUP_PURGE:
		if(gpio_get_pin_value(RESCON) == 0)
		{
			error_msg |= FC_ERR_RES_DISC;
		}
		if(get_FCPRES() < FC_LOW_PRES_THRES)
		{
			error_msg |= FC_ERR_PRES_L;
		}
		if(get_FCCURR() < UNDER_CUR_THRES)
		{
			error_msg |= FC_ERR_UND_CUR;
		}
		if(get_FCCURR() > OVER_CUR_THRES)
		{
			error_msg |= FC_ERR_OVER_CUR;
		}
		
		
		case FC_STATE_STARTUP_CHARGE:
		if(gpio_get_pin_value(RESCON) == 0)
		{
			error_msg |= FC_ERR_RES_DISC;
		}
		
		case FC_STATE_RUN:
		if(get_FCPRES() < FC_LOW_PRES_THRES)
		{
			error_msg |= FC_ERR_PRES_L;
		}
		if(get_FCCURR() < UNDER_CUR_THRES)
		{
			error_msg |= FC_ERR_UND_CUR;
		}
		if(get_FCCURR() > OVER_CUR_THRES)
		{
			error_msg |= FC_ERR_OVER_CUR;
		}
		
		case FC_STATE_ALARM:
		if(get_FCPRES() < FC_LOW_PRES_THRES)
		{
			error_msg |= FC_ERR_PRES_L;
		}
		if(get_FCCURR() < UNDER_CUR_THRES)
		{
			error_msg |= FC_ERR_UND_CUR;
		}
		if(get_FCCURR() > OVER_CUR_THRES)
		{
			error_msg |= FC_ERR_OVER_CUR;
		}
	}

	return(error_msg);
}
