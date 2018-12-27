/*
 *   Handling routines for LED lightening functions
 *
 *  $Id: 8192cd_led.c,v 1.1 2012/05/04 12:49:07 jimmylin Exp $
 *
 *  Copyright (c) 2012 Realtek Semiconductor Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#define _8192CD_LED_C_

#include "./8192cd_cfg.h"
#include "./8192cd.h"
#include "./8192cd_headers.h"
#include "./8192cd_debug.h"

#if defined(AEI_WECB) && defined(AEI_WIFI)
extern int set_wlan_led_by_cpu(unsigned int is5G, unsigned int flag);
#endif

// for SW LED ----------------------------------------------------
#ifdef RTL8190_SWGPIO_LED
static void set_swGpio_LED(struct rtl8192cd_priv *priv, unsigned int ledNum, int flag)
{
	unsigned int ledItem;	/* parameter to decode GPIO item */

	if (ledNum >= SWLED_GPIORT_CNT)
		return;

	ledItem = SWLED_GPIORT_ITEM(LED_ROUTE, ledNum);

	if (ledItem & SWLED_GPIORT_ENABLEMSK)
	{
		/* get the corresponding information (GPIO number/Active high or low) of LED */
		int gpio;
		int activeMode;	/* !=0 : Active High, ==0 : Active Low */

		gpio = ledItem & SWLED_GPIORT_RTBITMSK;
		activeMode = ledItem & SWLED_GPIORT_HLMSK;

		if (flag) {	/* Turn ON LED */
			if (activeMode)	/* Active High */
				RTL_W8(0x90, RTL_R8(0x90) | BIT(gpio));
			else			/* Active Low */
				RTL_W8(0x90, RTL_R8(0x90) &~ BIT(gpio));
		}
		else {	/* Turn OFF LED */
			if (activeMode)	/* Active High */
				RTL_W8(0x90, RTL_R8(0x90) &~ BIT(gpio));
			else			/* Active Low */
				RTL_W8(0x90, RTL_R8(0x90) | BIT(gpio));
		}
	}
}
#endif // RTL8190_SWGPIO_LED


static void set_sw_LED0(struct rtl8192cd_priv *priv, int flag)
{
#if defined(AEI_WECB) && defined(AEI_WIFI)
    if(0==set_wlan_led_by_cpu(!priv->pshare->wlandev_idx, flag)){
        return;
    }
#endif
#ifdef RTL8190_SWGPIO_LED
	if (LED_ROUTE)
		set_swGpio_LED(priv, 0, flag);
#else

#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
	if (flag)
		RTL_W32(LEDCFG, (RTL_R32(LEDCFG) & 0xfffffff0) | LED0SV);
	else
		RTL_W32(LEDCFG, RTL_R32(LEDCFG) & 0xfffffff0);
#elif defined(CONFIG_RTL_88E_SUPPORT)
	if (flag)
		RTL8188E_GPIO_write(5, 0);
	else
		RTL8188E_GPIO_write(5, 1);
#endif
#endif
}


static void set_sw_LED1(struct rtl8192cd_priv *priv, int flag)
{
#if defined(AEI_WECB) && defined(AEI_WIFI)
    if(0==set_wlan_led_by_cpu(!priv->pshare->wlandev_idx, flag)){
        return;
    }
#endif
#ifdef RTL8190_SWGPIO_LED
	if (LED_ROUTE)
		set_swGpio_LED(priv, 1, flag);
#else

#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
#ifdef CONFIG_RTL_92D_SUPPORT
	if (GET_CHIP_VER(priv) == VERSION_8192D) {
		if (flag)
			RTL_W32(LEDCFG, (RTL_R32(LEDCFG) & 0xfff0ffff) | LED1SV_92D);
		else
			RTL_W32(LEDCFG, RTL_R32(LEDCFG) & 0xfff0ffff);
	} else
#endif
	{
		if (flag)
			RTL_W32(LEDCFG, (RTL_R32(LEDCFG) & 0xfffff0ff) | LED1SV);
		else
			RTL_W32(LEDCFG, RTL_R32(LEDCFG) & 0xfffff0ff);
	}
#endif
#endif
}


static void set_sw_LED2(struct rtl8192cd_priv *priv, int flag)
{
#if defined(AEI_WECB) && defined(AEI_WIFI)
    if(0==set_wlan_led_by_cpu(!priv->pshare->wlandev_idx, flag)){
        return;
    }
#endif
#ifdef RTL8190_SWGPIO_LED
	if (LED_ROUTE)
		set_swGpio_LED(priv, 2, flag);
#else

#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
	if (flag)
		RTL_W32(LEDCFG, (RTL_R32(LEDCFG) & 0xfff0ffff) | LED2SV);
	else
		RTL_W32(LEDCFG, RTL_R32(LEDCFG) & 0xfff0ffff);
#endif
#endif
}


static void LED_Interval_timeout(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	int led_on_time= LED_ON_TIME;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

#ifdef PCIE_POWER_SAVING
	if ((priv->pwr_state != L1) && (priv->pwr_state != L2))
#endif
	{
		if ((LED_TYPE == LEDTYPE_SW_LINKTXRX) ||
			(LED_TYPE == LEDTYPE_SW_LINKTXRXDATA) ||
			(LED_TYPE == LEDTYPE_SW_ENABLETXRXDATA) ||
			((LED_TYPE == LEDTYPE_SW_ADATA_GDATA) && (priv->pshare->curr_band == BAND_5G)) ||
			(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRX) ||
			(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ENABLETXRXDATA) ||
			(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ASOCTXRXDATA) ||  
			(LED_TYPE == LEDTYPE_SW_LED2_GPIO10_LINKTXRX) ||
			(LED_TYPE == LEDTYPE_SW_LED1_GPIO10_LINKTXRX) ||
			(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRXDATA) ||
			(LED_TYPE == LEDTYPE_SW_LED1_ENABLETXRXDATA)) 
		{
			if (!priv->pshare->set_led_in_progress) {
				if ((LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRX) ||
					(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ENABLETXRXDATA) ||
					(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ASOCTXRXDATA) || 
					(LED_TYPE == LEDTYPE_SW_LED2_GPIO10_LINKTXRX) ||
					(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRXDATA))
					set_sw_LED2(priv, priv->pshare->LED_Toggle);
				else if (LED_TYPE == LEDTYPE_SW_LED1_GPIO10_LINKTXRX || LED_TYPE == LEDTYPE_SW_LED1_ENABLETXRXDATA)
					set_sw_LED1(priv, priv->pshare->LED_Toggle);
				else
					set_sw_LED0(priv, priv->pshare->LED_Toggle);
			}
		} else {
			if (!priv->pshare->set_led_in_progress)
				set_sw_LED1(priv, priv->pshare->LED_Toggle);
		}
	}

	if( (LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ASOCTXRXDATA) && 
	    (!(OPMODE & WIFI_ASOC_STATE)))  //client not assco  , mark_led
	{		
		led_on_time = LED_NOBLINK_TIME;
	}

	if ( priv->pshare->LED_Toggle == priv->pshare->LED_ToggleStart) {
		mod_timer(&priv->pshare->LED_Timer, jiffies + priv->pshare->LED_Interval);
	} else {
		if 	(LED_TYPE == LEDTYPE_SW_CUSTOM1)
			mod_timer(&priv->pshare->LED_Timer, jiffies + priv->pshare->LED_Interval);
		else
			mod_timer(&priv->pshare->LED_Timer, jiffies + led_on_time); 
	}

	priv->pshare->LED_Toggle = (priv->pshare->LED_Toggle + 1) % 2;
}


void enable_sw_LED(struct rtl8192cd_priv *priv, int init)
{
#ifdef AEI_WECB
    set_wlan_led(priv, LED_MODE_ON, 0, 0, 0, 0);//Radio Enabled (No Client, No Probe)
    return;
#else
#if defined(HW_ANT_SWITCH) || defined(SW_ANT_SWITCH)
		int b23 = RTL_R32(LEDCFG) & BIT(23);
#endif

	if (LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ASOCTXRXDATA) 
		if (!(OPMODE & WIFI_STATION_STATE)) // if it is not Client mode , then run orignal 12 type
			LED_TYPE = LEDTYPE_SW_LED2_GPIO8_ENABLETXRXDATA ;  
	     
	// configure mac to use SW LED
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
	if (LED_TYPE == LEDTYPE_SW_LED2_GPIO10_LINKTXRX)
		RTL_W32(LEDCFG,(RTL_R32(LEDCFG)&0xFF00FFFF) |LED2EN | LED2SV);
#ifdef CONFIG_RTL_92D_SUPPORT
	else if (LED_TYPE == LEDTYPE_SW_LED1_GPIO10_LINKTXRX || LED_TYPE == LEDTYPE_SW_LED1_ENABLETXRXDATA)
		RTL_W32(LEDCFG, LED1DIS_92D | LED1SV_92D);
#endif
	else if ((LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRX) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ENABLETXRXDATA) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ASOCTXRXDATA) || 	
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRXDATA))
		RTL_W32(LEDCFG, (RTL_R32(LEDCFG)&0xFF00FFFF) | GP8_LED | LED2EN | LED2SV);
	else
		RTL_W32(LEDCFG, LED2SV | LED1SV | LED0SV);
#elif defined(CONFIG_RTL_88E_SUPPORT)
	RTL8188E_GPIO_config(5, 0x10);
#endif

	priv->pshare->LED_Interval = LED_INTERVAL_TIME;
	priv->pshare->LED_Toggle = 0;
	priv->pshare->LED_ToggleStart = LED_OFF;
	priv->pshare->LED_tx_cnt_log = 0;
	priv->pshare->LED_rx_cnt_log = 0;
	priv->pshare->LED_tx_cnt = 0;
	priv->pshare->LED_rx_cnt = 0;

	if ((LED_TYPE == LEDTYPE_SW_ENABLE_TXRXDATA) ||
		(LED_TYPE == LEDTYPE_SW_ENABLETXRXDATA)) {
		set_sw_LED0(priv, LED_ON);
		set_sw_LED1(priv, LED_OFF);

		if (LED_TYPE == LEDTYPE_SW_ENABLETXRXDATA)
			priv->pshare->LED_ToggleStart = LED_ON;
	} else if (LED_TYPE == LEDTYPE_SW_LED1_ENABLETXRXDATA) {
		set_sw_LED1(priv, LED_ON);
		priv->pshare->LED_ToggleStart = LED_ON;
	} else if (LED_TYPE == LEDTYPE_SW_ADATA_GDATA) {
		priv->pshare->LED_ToggleStart = LED_ON;
		if (priv->pshare->curr_band == BAND_5G) {
			set_sw_LED0(priv, LED_ON);
			set_sw_LED1(priv, LED_OFF);
		}
		else {	// 11G
			set_sw_LED0(priv, LED_OFF);
			set_sw_LED1(priv, LED_ON);
		}
	}
	else if (LED_TYPE == LEDTYPE_SW_ENABLETXRXDATA_1) {
		set_sw_LED0(priv, LED_OFF);
		set_sw_LED1(priv, LED_ON);
		priv->pshare->LED_ToggleStart = LED_ON;
	}
	else if ((LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ENABLETXRXDATA) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ASOCTXRXDATA) ) {
		set_sw_LED2(priv, LED_ON);
		priv->pshare->LED_ToggleStart = LED_ON;
	}
	else if ((LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRX) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO10_LINKTXRX) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRXDATA)) {
		set_sw_LED2(priv, LED_OFF);
	}
	else {
		set_sw_LED0(priv, LED_OFF);
		set_sw_LED1(priv, LED_OFF);
		set_sw_LED2(priv, LED_OFF);
	}

#if defined(HW_ANT_SWITCH) || defined(SW_ANT_SWITCH)
	RTL_W32(LEDCFG, b23 | RTL_R32(LEDCFG));
#endif

	if (init) {
		init_timer(&priv->pshare->LED_Timer);
		priv->pshare->LED_Timer.expires = jiffies + priv->pshare->LED_Interval;
		priv->pshare->LED_Timer.data = (unsigned long) priv;
		priv->pshare->LED_Timer.function = &LED_Interval_timeout;
		mod_timer(&priv->pshare->LED_Timer, jiffies + priv->pshare->LED_Interval);
	}
#endif
}


void disable_sw_LED(struct rtl8192cd_priv *priv)
{
#ifdef AEI_WECB
    set_wlan_led(priv, LED_MODE_OFF, 0, 0, 0, 0);//Radio Disabled
    return;
#else
	if ((LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRX) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ENABLETXRXDATA) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO10_LINKTXRX) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRXDATA) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ASOCTXRXDATA)) {
		set_sw_LED2(priv, LED_OFF);
	} else {
	set_sw_LED0(priv, LED_OFF);
	set_sw_LED1(priv, LED_OFF);
	set_sw_LED2(priv, LED_OFF);
	}
	if (timer_pending(&priv->pshare->LED_Timer))
		del_timer_sync(&priv->pshare->LED_Timer);
#endif
}


void calculate_sw_LED_interval(struct rtl8192cd_priv *priv)
{
#ifdef AEI_WECB
#else
	unsigned int delta = 0;
	int i, scale_num=0;

	if (priv->pshare->set_led_in_progress)
		return;

	if( (LED_TYPE == LEDTYPE_SW_LED2_GPIO8_ASOCTXRXDATA) && 
	    (!(OPMODE & WIFI_ASOC_STATE)))  //client not assco  , mark_led
      {        
      	 priv->pshare->LED_Interval = LED_NOBLINK_TIME; // force one second
      	 priv->pshare->LED_tx_cnt_log = priv->pshare->LED_tx_cnt; // sync tx/rx cnt
	 priv->pshare->LED_rx_cnt_log = priv->pshare->LED_rx_cnt;      
	 return ;
      }

	// calculate counter delta
	delta += UINT32_DIFF(priv->pshare->LED_tx_cnt, priv->pshare->LED_tx_cnt_log);
	delta += UINT32_DIFF(priv->pshare->LED_rx_cnt, priv->pshare->LED_rx_cnt_log);
	priv->pshare->LED_tx_cnt_log = priv->pshare->LED_tx_cnt;
	priv->pshare->LED_rx_cnt_log = priv->pshare->LED_rx_cnt;

	// update interval according to delta
	if (delta == 0) {
		if (LED_TYPE == LEDTYPE_SW_CUSTOM1) {
			if (priv->pshare->LED_Interval != RTL_SECONDS_TO_JIFFIES(1)) {
				priv->pshare->LED_Interval = RTL_SECONDS_TO_JIFFIES(1);
				mod_timer(&priv->pshare->LED_Timer, jiffies + priv->pshare->LED_Interval);
			}
		} else {
			if (priv->pshare->LED_Interval == LED_NOBLINK_TIME)
				mod_timer(&priv->pshare->LED_Timer, jiffies + priv->pshare->LED_Interval);
			else
				priv->pshare->LED_Interval = LED_NOBLINK_TIME;
		}
	} else {
		if ((priv->pmib->dot11BssType.net_work_type & WIRELESS_11G) ||
			(priv->pmib->dot11BssType.net_work_type & WIRELESS_11A))
			scale_num = LED_MAX_PACKET_CNT_AG / LED_MAX_SCALE;
		else
			scale_num = LED_MAX_PACKET_CNT_B / LED_MAX_SCALE;

		if ((LED_TYPE == LEDTYPE_SW_LINK_TXRX) ||
			(LED_TYPE == LEDTYPE_SW_LINKTXRX) ||
			(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRX) ||
			(LED_TYPE == LEDTYPE_SW_LED2_GPIO10_LINKTXRX) ||
			(LED_TYPE == LEDTYPE_SW_LED1_GPIO10_LINKTXRX) ||
			(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRXDATA) ||
			(LED_TYPE == LEDTYPE_SW_CUSTOM1))
			scale_num = scale_num*2;

		for (i=1; i<=LED_MAX_SCALE; i++) {
			if (delta < i*scale_num)
				break;
		}

		if (priv->pshare->rf_ft_var.ledBlinkingFreq > 1) {
			i = i*priv->pshare->rf_ft_var.ledBlinkingFreq;
			if (i > LED_MAX_SCALE)
				i = LED_MAX_SCALE;
		}

		priv->pshare->LED_Interval = ((LED_MAX_SCALE-i+1)*LED_INTERVAL_TIME)/LED_MAX_SCALE;

		if (priv->pshare->LED_Interval < LED_ON_TIME)
			priv->pshare->LED_Interval = LED_ON_TIME;
	}

	if ((LED_TYPE == LEDTYPE_SW_LINKTXRX) ||
		(LED_TYPE == LEDTYPE_SW_LINKTXRXDATA) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRX) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO10_LINKTXRX) ||
		(LED_TYPE == LEDTYPE_SW_LED1_GPIO10_LINKTXRX) ||
		(LED_TYPE == LEDTYPE_SW_LED2_GPIO8_LINKTXRXDATA)) {
		if (priv->link_status)
			priv->pshare->LED_ToggleStart = LED_ON;
		else
			priv->pshare->LED_ToggleStart = LED_OFF;
	} else {
		if (priv->pshare->set_led_in_progress)
			return;

		if ((LED_TYPE == LEDTYPE_SW_LINK_TXRX) ||
			(LED_TYPE == LEDTYPE_SW_LINK_TXRXDATA)) {
			if (priv->link_status)
				set_sw_LED0(priv, LED_ON);
			else
				set_sw_LED0(priv, LED_OFF);
		} else if (LED_TYPE == LEDTYPE_SW_ADATA_GDATA) {
			if (priv->pshare->curr_band == BAND_5G) {
				set_sw_LED0(priv, LED_ON);
				set_sw_LED1(priv, LED_OFF);
			} else {	// 11A
				set_sw_LED0(priv, LED_OFF);
				set_sw_LED1(priv, LED_ON);
			}
		}
	}
#endif
}

void control_wireless_led(struct rtl8192cd_priv *priv, int enable)
{
	if (enable == 0) {
		priv->pshare->set_led_in_progress = 1;
		set_sw_LED0(priv, LED_OFF);
		set_sw_LED1(priv, LED_OFF);
		set_sw_LED2(priv, LED_OFF);
	}
	else if (enable == 1) {
		priv->pshare->set_led_in_progress = 1;
		set_sw_LED0(priv, LED_ON);
		set_sw_LED1(priv, LED_ON);
		set_sw_LED2(priv, LED_ON);
	}
	else if (enable == 2) {
		set_sw_LED0(priv, priv->pshare->LED_ToggleStart);
		set_sw_LED1(priv, priv->pshare->LED_ToggleStart);
		set_sw_LED2(priv, priv->pshare->LED_ToggleStart);
		priv->pshare->set_led_in_progress = 0;
	}
}


#ifdef CONFIG_RTL_ULINKER
static struct rtl8192cd_priv *root_priv = NULL;

void enable_sys_LED(struct rtl8192cd_priv *priv)
{
	RTL8188E_GPIO_config(4, 0x10);
	root_priv = priv;
}


void renable_sw_LED(void)
{
	struct rtl8192cd_priv *priv = root_priv;

	if ((LED_TYPE >= LEDTYPE_SW_LINK_TXRX) && (LED_TYPE < LEDTYPE_SW_MAX)) {
		priv->pshare->set_led_in_progress = 1;
		disable_sw_LED(priv);
		priv->pshare->set_led_in_progress = 0;
		enable_sw_LED(priv, 1);
	}
}

#endif /* #ifdef CONFIG_RTL_ULINKER */

#ifdef AEI_WECB
static void LED_ctrl_timeout(unsigned long task_priv)
{//used to control led to end temporary blinking state
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
    int sta_num = 0;
    int i = 0;

    switch(priv->pshare->LED_Mode){
        case LED_MODE_BLINK:
            if(priv->pshare->LED_Interval){//led works under blink-stop mode (LED_MODE_BLINKSTOP)
                set_wlan_led(priv, LED_MODE_OFF, 0, 0, 0, RTL_JIFFIES_TO_MILISECONDS(priv->pshare->LED_Interval));
                mod_timer(&priv->pshare->LED_CTRL_Timer, jiffies + priv->pshare->LED_Interval);
            }else{//led works under blink mode (LED_MODE_BLINK)
                if(RTL_MILISECONDS_TO_JIFFIES(FAST_BLINK_INTERVAL_ON)==priv->pshare->LED_ON_Interval){
                    //if led works under fast bink mode (with client traffic)
                    sta_num += get_assoc_sta_num(priv);
                    for(i=0; i<RTL8192CD_NUM_VWLAN; i++){
                        sta_num += get_assoc_sta_num(priv->pvap_priv[i]);
                    }
                    if(!sta_num){
                        set_wlan_led(priv, LED_MODE_ON, 0, 0, 0, 0);//Radio Enabled (No Client, No Probe)
                    }else{
#if defined(AEI_WECB_CUSTOMER_COMCAST) && !defined(AEI_WECB_CUSTOMER_TELUS)
                        set_wlan_led(priv, LED_MODE_BLINK, SLOW_BLINK_INTERVAL_ON, SLOW_BLINK_INTERVAL_OFF, 0, 0);//Radio Enabled (No Traffic, with Clients)
#else
                        set_wlan_led(priv, LED_MODE_ON, 0, 0, 0, 0);//Radio Enabled (No Traffic, with Clients)
#endif
                    }

                }else{//if led works under slow bink mode (probe without client)
                    set_wlan_led(priv, LED_MODE_ON, 0, 0, 0, 0);//Radio Enabled (No Client, No Probe)
                }
            }
            break;
        case LED_MODE_OFF:
            if(priv->pshare->LED_Interval){//led works under blink-stop mode (LED_MODE_OFF)
                set_wlan_led(priv, LED_MODE_BLINK, 
                    RTL_JIFFIES_TO_MILISECONDS(priv->pshare->LED_ON_Interval), 
                    RTL_JIFFIES_TO_MILISECONDS(priv->pshare->LED_OFF_Interval), 
                    priv->pshare->LED_Blink_Cycles, 
                    RTL_JIFFIES_TO_MILISECONDS(priv->pshare->LED_Interval));
            }
            break;
        default:
            break;
    }
}

static void LED_blink_timeout(unsigned long task_priv)
{//used to control led blinking (on-off-on-off-...)
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
    if (LED_MODE_BLINK!=priv->pshare->LED_Mode){
        return;
    }

    priv->pshare->LED_Toggle = !(priv->pshare->LED_Toggle );
    if(!priv->pshare->LED_Toggle){
        mod_timer(&priv->pshare->LED_Timer, jiffies + priv->pshare->LED_ON_Interval);
    }else{
        mod_timer(&priv->pshare->LED_Timer, jiffies + priv->pshare->LED_OFF_Interval);
    }
    if (priv->pshare->wlandev_idx) {
        set_sw_LED0(priv, priv->pshare->LED_Toggle);
    }else{
        set_sw_LED1(priv, priv->pshare->LED_Toggle);
    }
}

void init_wlan_led(struct rtl8192cd_priv *priv, unsigned int mode, unsigned int on_ms, unsigned int off_ms)
{
    if (priv->pshare->wlandev_idx) {
        RTL_W32(LEDCFG, LED2SV | LED1SV | LED0SV);
    }else{
        RTL_W32(LEDCFG, LED1DIS_92D | LED1SV_92D);
    }
    init_timer(&priv->pshare->LED_CTRL_Timer);
    priv->pshare->LED_CTRL_Timer.data = (unsigned long) priv;
    priv->pshare->LED_CTRL_Timer.function = &LED_ctrl_timeout;

    init_timer(&priv->pshare->LED_Timer);
    priv->pshare->LED_Timer.data = (unsigned long) priv;
    priv->pshare->LED_Timer.function = &LED_blink_timeout;

    set_wlan_led(priv, mode, on_ms, off_ms, 0, 0);
}

void set_wlan_led(struct rtl8192cd_priv *priv, unsigned int mode, 
    unsigned int on_ms, unsigned int off_ms, unsigned int cycles, unsigned int stop_ms)
{//set led mode
    const int min_interval = 50;
    
    if(NULL==priv||mode>=LED_MODE_MAX){
        return;
    }

    switch(mode){
        case LED_MODE_OFF://off
            del_timer(&priv->pshare->LED_Timer);
            if(!stop_ms){
                del_timer(&priv->pshare->LED_CTRL_Timer);
            }
            priv->pshare->LED_Mode = mode;
            priv->pshare->LED_Toggle = LED_OFF;
            set_sw_LED0(priv, priv->pshare->LED_Toggle);
            set_sw_LED1(priv, priv->pshare->LED_Toggle);
            set_sw_LED2(priv, priv->pshare->LED_Toggle);
            return;
        case LED_MODE_ON://on
            del_timer(&priv->pshare->LED_Timer);
            del_timer(&priv->pshare->LED_CTRL_Timer);
            priv->pshare->LED_Mode = mode;
            priv->pshare->LED_Toggle = LED_ON;
            set_sw_LED0(priv, priv->pshare->LED_Toggle);
            set_sw_LED1(priv, priv->pshare->LED_Toggle);
            set_sw_LED2(priv, priv->pshare->LED_Toggle);
            return;
        case LED_MODE_BLINK://blink
            if(on_ms<min_interval){
                on_ms = min_interval;
            }
            if(off_ms<min_interval){
                off_ms = min_interval;
            }
            if(cycles){
                mod_timer(&priv->pshare->LED_CTRL_Timer, jiffies 
                    + (priv->pshare->LED_ON_Interval+priv->pshare->LED_OFF_Interval)*cycles);
            }else{
                del_timer(&priv->pshare->LED_CTRL_Timer);
            }
            if(mode==priv->pshare->LED_Mode
                && RTL_MILISECONDS_TO_JIFFIES(on_ms)==priv->pshare->LED_ON_Interval
                && RTL_MILISECONDS_TO_JIFFIES(off_ms)==priv->pshare->LED_OFF_Interval){
                return;
            }
            priv->pshare->LED_Mode = mode;
            priv->pshare->LED_ToggleStart = priv->pshare->LED_Toggle;
            priv->pshare->LED_Blink_Cycles = cycles;
            priv->pshare->LED_ON_Interval = RTL_MILISECONDS_TO_JIFFIES(on_ms);
            priv->pshare->LED_OFF_Interval = RTL_MILISECONDS_TO_JIFFIES(off_ms);
            priv->pshare->LED_Interval = RTL_MILISECONDS_TO_JIFFIES(stop_ms);
            mod_timer(&priv->pshare->LED_Timer, jiffies + RTL_MILISECONDS_TO_JIFFIES(min_interval));
            return;
        case LED_MODE_BLINKSTOP://blink-stop, blink-stop, ...
            priv->pshare->LED_Mode = mode;
            priv->pshare->LED_ToggleStart = priv->pshare->LED_Toggle;
            priv->pshare->LED_Blink_Cycles = cycles;
            priv->pshare->LED_ON_Interval = RTL_MILISECONDS_TO_JIFFIES(on_ms);
            priv->pshare->LED_OFF_Interval = RTL_MILISECONDS_TO_JIFFIES(off_ms);
            priv->pshare->LED_Interval = RTL_MILISECONDS_TO_JIFFIES(stop_ms);
            mod_timer(&priv->pshare->LED_CTRL_Timer, jiffies + RTL_MILISECONDS_TO_JIFFIES(min_interval));
            set_wlan_led(priv, LED_MODE_BLINK, 
                RTL_JIFFIES_TO_MILISECONDS(priv->pshare->LED_ON_Interval), 
                RTL_JIFFIES_TO_MILISECONDS(priv->pshare->LED_OFF_Interval), 
                priv->pshare->LED_Blink_Cycles, 
                RTL_JIFFIES_TO_MILISECONDS(priv->pshare->LED_Interval));
            break;
        default:
            return;
    }

}
#endif
