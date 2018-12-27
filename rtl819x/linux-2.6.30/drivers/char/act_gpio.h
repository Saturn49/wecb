#if defined(AEI_WECB)

#ifndef _ACT_GPIO_H_
#define _ACT_GPIO_H_

#define BTN_STAT_RELEASED 0x0
#define BTN_STAT_PRESSED  0x1
#define BTN_STAT_PRESSED_ON_BOOT 0x2
#define BTN_STAT_NO_STAT 0x3

#define MSG_NETLINK_RESET_STATUS_CHANGE 0x4000
#define MSG_NETLINK_WPS_STATUS_CHANGE	0xa000

#define WIFI_ONOFF_PIN_IOBASE PABCD_CNR 	//RTL_GPIO_PABCD_CNR
#define WIFI_ONOFF_PIN_DIRBASE PABCD_DIR	//RTL_GPIO_PABCD_DIR
#define WIFI_ONOFF_PIN_DATABASE PABCD_DAT //RTL_GPIO_PABCD_DATA
#define WIFI_ONOFF_PIN_NO 19/*umber of the ABCD)*/
#define WIFI_ONOFF_PIN_IMR PAB_IMR
#define RTL_GPIO_WIFI_ONOFF     19


#define GPIOA(x) (x)
#define GPIOB(x) (8 * 1 + x)
#define GPIOC(x) (8 * 2 + x)
#define GPIOD(x) (8 * 3 + x)
#define GPIOE(x) (8 * 4 + x)
#define GPIOF(x) (8 * 5 + x)
#define GPIOG(x) (8 * 6 + x)
#define GPIOH(x) (8 * 7 + x)

enum LED_CONTROL{
    LED_GPIO = 1 << 0,
    LED_CHIP = 1 << 1,
};


#if defined(AEI_WECB_CUSTOMER_COMCAST)
#if defined(CONFIG_AEI_HW_WECB3000) || defined(CONFIG_AEI_HW_JNR3000)

#define BOARD_GPP_RESET_BUTTON GPIOH(1)
#define BOARD_GPP_WPS_BUTTON GPIOH(0)

#define BOARD_GPP_POWER_WHITE_LED GPIOA(1) // By HW design, should be GPIOC(3). Because HW issue #35349, added this fixed temporarily
#define BOARD_GPP_POWER_RED_LED GPIOC(3) // By HW design, should be GPIOA(1). Because HW issue #35349, added this fixed temporarily
#define BOARD_GPP_MOCA_LED GPIOB(2)
#define BOARD_GPP_WPS_WHITE_LED GPIOH(2)
#define BOARD_GPP_WPS_RED_LED GPIOB(3)
#define BOARD_GPP_WIFI2G_LED GPIOA(2)
#define BOARD_GPP_WIFI5G_LED GPIOA(3)
#define BOARD_GPP_LED_SEL GPIOA(0)

#define DEFAULT_74HCT_VALUE LED_GPIO

#elif defined(CONFIG_AEI_HW_ECB3000)

#define BOARD_GPP_RESET_BUTTON GPIOH(1)
#define BOARD_GPP_POWER_RED_LED GPIOC(0)
#define BOARD_GPP_MOCA_LED GPIOB(2)

#endif

#else // AEI_WECB_CUSTOMER_VERIZON, AEI_WECB_CUSTOMER_NCS, default

#if defined(CONFIG_AEI_HW_WECB3000) || defined(CONFIG_AEI_HW_JNR3000)

#define BOARD_GPP_RESET_BUTTON GPIOH(1)
#define BOARD_GPP_WPS_BUTTON GPIOH(0)

#define BOARD_GPP_POWER_GREEN_LED GPIOA(1) // By HW design, should be GPIOC(3). Because HW issue #35349, added this fixed temporarily
#define BOARD_GPP_POWER_RED_LED GPIOC(3) // By HW design, should be GPIOA(1). Because HW issue #35349, added this fixed temporarily
#define BOARD_GPP_MOCA_LED GPIOB(2)
#define BOARD_GPP_WPS_GREEN_LED GPIOH(2)
#define BOARD_GPP_WPS_RED_LED GPIOB(3)

#define BOARD_GPP_WIFI2G_LED GPIOA(2)
#define BOARD_GPP_WIFI5G_LED GPIOA(3)
#define BOARD_GPP_LED_SEL GPIOA(0)

#define DEFAULT_74HCT_VALUE LED_GPIO

#elif defined(CONFIG_AEI_HW_ECB3000)

#define BOARD_GPP_RESET_BUTTON GPIOH(1)
#define BOARD_GPP_POWER_RED_LED GPIOC(0)
#define BOARD_GPP_MOCA_LED GPIOB(2)

#endif
#endif

#endif
#endif
