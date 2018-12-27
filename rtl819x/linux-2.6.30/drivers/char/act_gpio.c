#ifdef AEI_WECB

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/reboot.h>
#include <linux/kmod.h>
#include <linux/proc_fs.h>
#include  "bspchip.h"

#include <linux/netlink.h>
#include <net/sock.h>
#include "act_gpio.h"
#include "drivers/net/rtl819x/AsicDriver/rtl865xc_asicregs.h"

#define DEBUG

enum LED_STATE{
	LED_ON			= 1 << 0,
	LED_OFF			= 1 << 1,
	LED_BLINK		= 1 << 2,
	LED_UNBLINK		= 1 << 3,
	LED_SLOW_BLINK		= 1 << 4,
	LED_FAST_BLINK		= 1 << 5,
};

enum LED_STATE_SWAP{
    STATE_NOSWAP   = 1 << 0,
    STATE_SWAP   = 1 << 1,
};

typedef struct hw_button_s{
	int gpio;
	int pin;
	char *name;
	__u32 message_type;
	__u32 message;
    __u32 last_stat;
    struct timer_list isr_timer;
} hw_button_t;

typedef struct  hw_led_s{
	int gpio;
	char *name;
	unsigned short state;
    unsigned short swap; // 0: don't need swap, 1: need swap on/off, because HW connect not same as others
	read_proc_t *read_proc;
	write_proc_t *write_proc;
    unsigned int ms_on;
    unsigned int ms_off;
	struct timer_list on_timer;
	struct timer_list off_timer;
} hw_led_t;

static int led_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data);
static int led_write_proc(struct file *file, const char *buffer, unsigned long count, void *data);
static int gpio_set_val(int gpio, int val);
static int gpio_get_val(int gpio);
static int inline led_blink(hw_led_t *hled);
static int inline led_unblink(hw_led_t *hled);

#define MAX_PAYLOAD (sizeof(int))


#if defined(AEI_WECB)
#if defined(AEI_WECB_CUSTOMER_COMCAST)
#if defined(CONFIG_AEI_HW_WECB3000) || defined(CONFIG_AEI_HW_JNR3000)

static hw_button_t hw_buttons[] = {
	{BOARD_GPP_RESET_BUTTON, BOARD_GPP_RESET_BUTTON, "Reset button", MSG_NETLINK_RESET_STATUS_CHANGE},
	{BOARD_GPP_WPS_BUTTON, BOARD_GPP_WPS_BUTTON, "WPS button", MSG_NETLINK_WPS_STATUS_CHANGE},
};

static hw_led_t hw_led[] = {
    {BOARD_GPP_POWER_WHITE_LED, "PowerWhiteLED", LED_ON, STATE_NOSWAP, led_read_proc, led_write_proc},
	{BOARD_GPP_POWER_RED_LED, "PowerRedLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
    {BOARD_GPP_MOCA_LED, "MoCALED", LED_OFF, STATE_SWAP, led_read_proc, led_write_proc},
    {BOARD_GPP_WPS_WHITE_LED, "WPSWhiteLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
	{BOARD_GPP_WPS_RED_LED, "WPSRedLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
    {BOARD_GPP_WIFI2G_LED, "WiFi2GLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
    {BOARD_GPP_WIFI5G_LED, "WiFi5GLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
};
#elif defined(CONFIG_AEI_HW_ECB3000)

static hw_button_t hw_buttons[] = {
	{BOARD_GPP_RESET_BUTTON, BOARD_GPP_RESET_BUTTON, "Reset button", MSG_NETLINK_RESET_STATUS_CHANGE},
};

static hw_led_t hw_led[] = {
	{BOARD_GPP_POWER_RED_LED, "PowerRedLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
};

#endif

#else // AEI_WECB_CUSTOMER_VERIZON, AEI_WECB_CUSTOMER_NCS, default
#if defined(CONFIG_AEI_HW_WECB3000) || defined(CONFIG_AEI_HW_JNR3000)

static hw_button_t hw_buttons[] = {
    {BOARD_GPP_RESET_BUTTON, BOARD_GPP_RESET_BUTTON, "Reset button", MSG_NETLINK_RESET_STATUS_CHANGE},
    {BOARD_GPP_WPS_BUTTON, BOARD_GPP_WPS_BUTTON, "WPS button", MSG_NETLINK_WPS_STATUS_CHANGE},
};

static hw_led_t hw_led[] = {
    {BOARD_GPP_POWER_GREEN_LED, "PowerGreenLED", LED_ON, STATE_NOSWAP, led_read_proc, led_write_proc},
    {BOARD_GPP_POWER_RED_LED, "PowerRedLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
    {BOARD_GPP_MOCA_LED, "MoCALED", LED_OFF, STATE_SWAP, led_read_proc, led_write_proc},
    {BOARD_GPP_WPS_GREEN_LED, "WPSGreenLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
    {BOARD_GPP_WPS_RED_LED, "WPSRedLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
    {BOARD_GPP_WIFI2G_LED, "WiFi2GLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
    {BOARD_GPP_WIFI5G_LED, "WiFi5GLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
};
#elif defined(CONFIG_AEI_HW_ECB3000)

static hw_button_t hw_buttons[] = {
	{BOARD_GPP_RESET_BUTTON, BOARD_GPP_RESET_BUTTON, "Reset button", MSG_NETLINK_RESET_STATUS_CHANGE},
};

static hw_led_t hw_led[] = {
	{BOARD_GPP_POWER_RED_LED, "PowerRedLED", LED_OFF, STATE_NOSWAP, led_read_proc, led_write_proc},
};

#endif

#endif
#endif




/* If interrupt occurs on init (immediately after set IRQ), then button is
 * pressed on the boot stage */
static int is_on_boot;
static int irq_number = -1;

#if defined(AEI_WECB)

static int hw_set_gpio_mode(unsigned int gpio)
{
    switch(gpio)
    {
        case GPIOA(0):
        case GPIOA(1):
        case GPIOA(2):
        case GPIOA(3):
            RTL_W32(PIN_MUX_SEL, RTL_R32(PIN_MUX_SEL) | 0x3 << 3); // set jtag as GPIO
            break;
        case GPIOB(0):
            RTL_W32(PIN_MUX_SEL2, RTL_R32(PIN_MUX_SEL2) | 0x3 << 1); // set gpios0 as GPIO
            break;
        case GPIOB(1):
            RTL_W32(PIN_MUX_SEL2, RTL_R32(PIN_MUX_SEL2) | 0x3 << 4); // set gpios1 as GPIO
            break;
        case GPIOB(2):
            RTL_W32(PIN_MUX_SEL2, RTL_R32(PIN_MUX_SEL2) | 0x3 << 7); // set gpios2 as GPIO
            break;
        case GPIOB(3):
            RTL_W32(PIN_MUX_SEL2, RTL_R32(PIN_MUX_SEL2) | 0x3 << 10); // set gpios3 as GPIO
            break;
        case GPIOC(0):
            RTL_W32(PIN_MUX_SEL2, RTL_R32(PIN_MUX_SEL2) | 0x3 << 13); // set gpiop0 as GPIO
            break;
        case GPIOC(2):
            RTL_W32(PIN_MUX_SEL2, RTL_R32(PIN_MUX_SEL2) | 0x3 << 19); // set gpiop2 as GPIO
            break;
        case GPIOC(3):
            RTL_W32(PIN_MUX_SEL2, RTL_R32(PIN_MUX_SEL2) | 0x3 << 22); // set gpiop3 as GPIO
            break;
        case GPIOH(0):
        case GPIOH(1):
        case GPIOH(2):
            RTL_W32(PIN_MUX_SEL, RTL_R32(PIN_MUX_SEL) | 0x3 << 1); // set gpioh as GPIO
            break;
        default:
            printk("Set GPIO Mode error\n");
            return -1;
    }

    return 0;
}
#endif

static int inline hw_button_isr_timer_func(int data)
{
    hw_button_t *btn = (hw_button_t *)data;

    if(btn->last_stat != btn->message)
    {
        if((btn->last_stat == BTN_STAT_PRESSED_ON_BOOT || btn->last_stat == BTN_STAT_PRESSED) &&
            (btn->message == BTN_STAT_PRESSED_ON_BOOT || btn->message == BTN_STAT_PRESSED))
        {
            return 0;
        }

        btn->last_stat = btn->message;
        msg_netlink_broadcast(&btn->message, btn->message_type, sizeof(btn->message));
#ifdef DEBUG
        printk("send interrupt to APP: btn \"%s\". Status %s\n", btn->name, btn->message == BTN_STAT_PRESSED ? "pressed" : "released");
#endif
    }

	return 0;
}

static irqreturn_t hw_button_isr(int irq, void *dev_id)
{
	unsigned int status;
	hw_button_t *btn = NULL;
	int is_pressed = BTN_STAT_NO_STAT;
	int i;

	if(irq == BSP_GPIO_ABCD_IRQ)
	{
		status = REG32(PABCD_ISR);
		for (i = 0; i < 32; i++)
		{
			if(status & 1 << i)
			{
#ifdef DEBUG
				printk("interrupt 0x%x\n", i);
#endif
				irq_number = i;
				RTL_W32(PABCD_ISR, 1 << i);
				break;
			}
		}
		
		if(i == 32)
		{
			printk("no BSP_GPIO_ABCD_IRQ interrupt\n");
			goto DONE;
		}

		if(RTL_R32(PABCD_DAT) & 1 << irq_number)
		{
			is_pressed = BTN_STAT_RELEASED;
		}
		else
		{
			is_pressed = BTN_STAT_PRESSED;
		}
		
#ifdef DEBUG		
		printk("irq_number =%d, is_pressed=%d \n", irq_number, is_pressed);
#endif

	}
	else if(irq == BSP_GPIO_EFGH_IRQ)
	{
		status = REG32(PEFGH_ISR);
		for (i = 0; i < 32; i++)
		{
			if(status & 1 << i)
			{
#ifdef DEBUG
				printk("interrupt 0x%x\n", i);
#endif
				irq_number = i + 32;
				RTL_W32(PEFGH_ISR, 1 << i);
				break;
			}
		}
		
		if(i == 32)
		{
			printk("no BSP_GPIO_EFGH_IRQ interrupt\n");
			goto DONE;
		}

		if(RTL_R32(PEFGH_DAT) & 1 << (irq_number - 32))
		{
			is_pressed = BTN_STAT_RELEASED;
		}
		else
		{
			is_pressed = BTN_STAT_PRESSED;
		}

#ifdef DEBUG		
		printk("irq_number=%d, is_pressed=%d \n", irq_number, is_pressed);
#endif
		
	}
	
	for(i = 0; i < sizeof(hw_buttons) / sizeof(hw_buttons[0]); i++)
	{
		if(irq_number == hw_buttons[i].gpio)
		{
			btn = &hw_buttons[i];
			break;
		}
	}

	if(i == sizeof(hw_buttons) / sizeof(hw_buttons[0]))
	{
		printk("Error: Interrupt received irq %d\n", irq_number);
		goto DONE;
	}

#ifdef DEBUG
	printk("Interrupt received. irq %d, btn \"%s\". Status %s %s\n", irq_number,
	btn->name, is_pressed == BTN_STAT_PRESSED ? "pressed" : "released",
	is_on_boot ? "on boot stage" : "");
#endif

	if (is_pressed == BTN_STAT_PRESSED)
		btn->message = is_on_boot ? BTN_STAT_PRESSED_ON_BOOT : BTN_STAT_PRESSED;
	else
		btn->message  = BTN_STAT_RELEASED;

    mod_timer(&btn->isr_timer,jiffies + ((500)*HZ)/1000);

	// tasklet_schedule(&hw_button_tasklet);
DONE:
	return IRQ_HANDLED;
}

int hw_buttons_init(void)
{
	int i;
	hw_button_t *btn;

	is_on_boot = 1;

	if (request_irq(BSP_GPIO_ABCD_IRQ, hw_button_isr, IRQF_SHARED, "abcd_gpio", &hw_buttons)) {
		printk("gpio request_irq(%d) error!\n", BSP_GPIO_ABCD_IRQ);
   	}
   	if (request_irq(BSP_GPIO_EFGH_IRQ, hw_button_isr, IRQF_SHARED, "efgh_gpio", &hw_buttons)) {
		printk("gpio request_irq(%d) error!\n", BSP_GPIO_EFGH_IRQ);
   	}

	for (i = 0; i < ARRAY_SIZE(hw_buttons); i++)
	{
		btn = &hw_buttons[i];

        hw_set_gpio_mode(btn->gpio);
		if(btn->gpio < 32 && btn->gpio >= 0)
		{
			RTL_W32(PABCD_CNR, RTL_R32(PABCD_CNR) & ~(1 << btn->gpio)); // Configured as GPIO pin
			RTL_W32(PABCD_DIR, RTL_R32(PABCD_DIR) & ~(1 << btn->gpio)); // Configured as input pin
			RTL_W32(PABCD_ISR, RTL_R32(PABCD_ISR) | (1 << btn->gpio)); // Configured as input pin
			if(btn->gpio < 16)
			{
				RTL_W32(PAB_IMR, RTL_R32(PAB_IMR) | (0x3 << (btn->gpio) * 2)); // Configured as falling & rising edge interrupt
			}
			else
			{
				RTL_W32(PCD_IMR, RTL_R32(PCD_IMR) | (0x3 << (btn->gpio - 16) * 2)); // Configured as falling & rising edge interrupt
			}
		}
		else if(btn->gpio >= 32 && btn->gpio <64)
		{
			RTL_W32(PEFGH_CNR, RTL_R32(PEFGH_CNR) & ~(1 << (btn->gpio - 32))); // Configured as GPIO pin
			RTL_W32(PEFGH_DIR, RTL_R32(PEFGH_DIR) & ~(1 << (btn->gpio - 32))); // Configured as input pin
			RTL_W32(PEFGH_ISR, RTL_R32(PEFGH_ISR) | (1 << (btn->gpio - 32))); // Configured as input pin
			if(btn->gpio < 16 + 32)
			{
				RTL_W32(PEF_IMR, RTL_R32(PEF_IMR) | (0x3 << ((btn->gpio - 32)) * 2)); // Configured as falling & rising edge interrupt
			}
			else
			{
				RTL_W32(PGH_IMR, RTL_R32(PGH_IMR) | (0x3 << ((btn->gpio - 32) - 16) * 2)); // Configured as falling & rising edge interrupt
			}
		}
		else
		{
			printk("Realtek GPIO Driver, init interrupt failed!\n");
			return -ENOENT;
		}

        init_timer(&btn->isr_timer);
        btn->isr_timer.function = (void (*)(unsigned long))hw_button_isr_timer_func;
        btn->isr_timer.data = (unsigned long)btn;
	}
	is_on_boot = 0;

	return 0;
}

static void hw_buttons_uninit(void)
{
	return;
}

static int led_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	hw_led_t *hled = (hw_led_t *)data;
	return sprintf(page, "0x%x\n", hled->state);
}

static int led_set_state(hw_led_t *hled, unsigned int val)
{
	val = val & (LED_ON | LED_OFF | LED_BLINK | LED_UNBLINK | LED_SLOW_BLINK | LED_FAST_BLINK);
	if(((val & LED_ON) && (val & LED_OFF)) || ((val & (LED_BLINK | LED_SLOW_BLINK  | LED_FAST_BLINK)) && (val & LED_UNBLINK)))
	{
		return -EINVAL;
	}

    hled->state |= val;

	if(val & LED_ON)
	{
        if(hled->swap & STATE_SWAP)
        {
		    gpio_set_val(hled->gpio, 1);
        }
        else
        {
            gpio_set_val(hled->gpio, 0);
        }
		hled->state &= ~LED_OFF;
	}

	if(val & LED_OFF)
	{
        if(hled->swap & STATE_SWAP)
        {
            gpio_set_val(hled->gpio, 0);
        }
        else
        {
		    gpio_set_val(hled->gpio, 1);
        }
        hled->state &= ~LED_ON;
	}

	if(val & LED_BLINK)
	{
        hled->ms_on = 50;
        hled->ms_off = 50;
		led_blink(hled);
		hled->state &= ~LED_UNBLINK;
	}

    if(val & LED_SLOW_BLINK)
	{
        hled->ms_on = 500;
        hled->ms_off = 500;
		led_blink(hled);
		hled->state &= ~LED_UNBLINK;
	}

    if(val & LED_FAST_BLINK)
	{
        hled->ms_on = 167;
        hled->ms_off = 167;
		led_blink(hled);
		hled->state &= ~LED_UNBLINK;
	}

	if(val & LED_UNBLINK)
	{
		led_unblink(hled);
        hled->state &= ~(LED_BLINK | LED_SLOW_BLINK | LED_FAST_BLINK);
	}

	return hled->state;
}

static int led_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	hw_led_t *hled = (hw_led_t *)data;
	unsigned int val;

	if (sscanf(buffer, "%x", &val) < 0)
		return -EINVAL;

	led_set_state(hled, val);

	return count;
}

static int gpio_set_val(int gpio, int val)
{
	if(gpio < 32)
	{
		if(val)
		{
			RTL_W32(PABCD_DAT, RTL_R32(PABCD_DAT) | (1 << gpio));
		}
		else
		{
			RTL_W32(PABCD_DAT, RTL_R32(PABCD_DAT) & (~(1 << gpio)));
		}
	}
	else
	{
		if(val)
		{
			RTL_W32(PEFGH_DAT, RTL_R32(PEFGH_DAT) | (1 << (gpio - 32)));
		}
		else
		{
			RTL_W32(PEFGH_DAT, RTL_R32(PEFGH_DAT) & (~(1 << (gpio - 32))));
		}
	}

	return 0;
}

static int gpio_get_val(int gpio)
{
	if(gpio < 32)
	{
		return RTL_R32(PABCD_DAT) & 1 << gpio;
	}
	else
	{
		return RTL_R32(PEFGH_DAT) & (1 << (gpio - 32));
	}
}

static int inline led_on_timer_func(int data)
{
	hw_led_t *hled = (hw_led_t *)data;
    if(hled->swap & STATE_SWAP)
    {
        gpio_set_val(hled->gpio, 1);
    }
    else
    {
	    gpio_set_val(hled->gpio, 0);
    }
	mod_timer(&hled->off_timer,jiffies + ((hled->ms_on)*HZ)/1000);

	return 0;
}

static int inline led_off_timer_func(int data)
{
	hw_led_t *hled = (hw_led_t *)data;
    if(hled->swap & STATE_SWAP)
    {
        gpio_set_val(hled->gpio, 0);
    }
    else
    {
	    gpio_set_val(hled->gpio, 1);
    }
	mod_timer(&hled->on_timer, jiffies + ((hled->ms_off)*HZ)/1000);

	return 0;
}

static int inline led_blink(hw_led_t *hled)
{
	if(hled->state & LED_ON)
	{
        if(hled->swap & STATE_SWAP)
        {
            gpio_set_val(hled->gpio, 0);
        }
        else
        {
		    gpio_set_val(hled->gpio, 1);
        }
		mod_timer(&hled->off_timer,jiffies + ((hled->ms_on)*HZ)/1000);
		// add_timer(&hled->off_timer);
	}
	else
	{
        if(hled->swap & STATE_SWAP)
        {
            gpio_set_val(hled->gpio, 1);
        }
        else
        {
		    gpio_set_val(hled->gpio, 0);
        }
		mod_timer(&hled->on_timer, jiffies + ((hled->ms_off)*HZ)/1000);
		// add_timer(&hled->on_timer);
	}

	return 0;
}

// stop blinking
static int inline led_unblink(hw_led_t *hled)
{
	del_timer(&hled->on_timer);
	del_timer(&hled->off_timer);
	if(hled->state & LED_ON)
	{
        if(hled->swap & STATE_SWAP)
        {
            gpio_set_val(hled->gpio, 1);
        }
        else
        {
		    gpio_set_val(hled->gpio, 0);
        }
	}
	else
	{
        if(hled->swap & STATE_SWAP)
        {
            gpio_set_val(hled->gpio, 0);
        }
        else
        {
		    gpio_set_val(hled->gpio, 1);
        }
	}

	return 0;
}

#if defined(CONFIG_AEI_HW_WECB3000)

static int led_74hct_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    if(gpio_get_val(BOARD_GPP_LED_SEL) == 0)
    {
        return sprintf(page, "Control by %s\n", "GPIO");
    }
    else
    {
        return sprintf(page, "Control by %s\n", "CHIP");
    }
}

static int led_74hct_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char val[256];
    memset(&val, 0, sizeof(val));

	if (sscanf(buffer, "%s", val) < 0)
		return -EINVAL;

    if(strcmp(val, "GPIO") == 0)
    {
        gpio_set_val(BOARD_GPP_LED_SEL, 0);
    }

    if(strcmp(val, "CHIP") == 0)
    {
        gpio_set_val(BOARD_GPP_LED_SEL, 1);
    }

	return count;
}


static int led_74hct_init(struct proc_dir_entry *act_config)
{
    static struct proc_dir_entry *res=NULL;
    int gpio = BOARD_GPP_LED_SEL;

    if(act_config == NULL)
    {
        return -1;
    }

    if(res)
    {
        return 0;
    }

    hw_set_gpio_mode(gpio);

	if(gpio < 32)
	{
		RTL_W32(PABCD_CNR, RTL_R32(PABCD_CNR) & ~(1 << gpio)); // Configured as GPIO pin
		RTL_W32(PABCD_DIR, RTL_R32(PABCD_DIR) | (1 << gpio)); // Configured as output pin
	}
	else
	{
		RTL_W32(PEFGH_CNR, RTL_R32(PEFGH_CNR) & ~(1 << (gpio - 32))); // Configured as GPIO pin
		RTL_W32(PEFGH_DIR, RTL_R32(PEFGH_DIR) | (1 << (gpio - 32))); // Configured as output pin
	}

    if(DEFAULT_74HCT_VALUE == LED_GPIO)
    {
        gpio_set_val(gpio, 0); // Default set MOCA/WiFi LED control by GPIO
    }
    else if(DEFAULT_74HCT_VALUE == LED_CHIP)
    {
        gpio_set_val(gpio, 1); // Default set MOCA/WiFi LED control by CHIP
    }

    res = create_proc_entry("led_sel", 0644, act_config);
    if (res) {
		res->read_proc = led_74hct_read_proc;
		res->write_proc = led_74hct_write_proc;
		res->data = NULL;
	}
	else
	{
		printk("Realtek GPIO Driver, create proc led_swap failed!\n");
		return -ENOENT;
	}

    return 0;
}

#endif

static int hw_led_init(void)
{
	int i;
	hw_led_t *hled;
	struct proc_dir_entry *res=NULL;
	struct proc_dir_entry *act_config = NULL;

	act_config = proc_mkdir("act_config", NULL);
	if(!act_config)
	{
		return -ENOENT;
	}

	for(i = 0; i < sizeof(hw_led) / sizeof(hw_led[0]); i++)
	{
		hled = &hw_led[i];
#if defined(CONFIG_AEI_HW_WECB3000)
        if((hled->gpio == BOARD_GPP_MOCA_LED) || (hled->gpio == BOARD_GPP_WIFI2G_LED) || (hled->gpio ==BOARD_GPP_WIFI5G_LED))
        {
            led_74hct_init(act_config);
        }
#endif
        hw_set_gpio_mode(hled->gpio);

		if(hled->gpio < 32)
		{
			RTL_W32(PABCD_CNR, RTL_R32(PABCD_CNR) & ~(1 << hled->gpio)); // Configured as GPIO pin
			RTL_W32(PABCD_DIR, RTL_R32(PABCD_DIR) | (1 << hled->gpio)); // Configured as output pin
		}
		else
		{
			RTL_W32(PEFGH_CNR, RTL_R32(PEFGH_CNR) & ~(1 << (hled->gpio - 32))); // Configured as GPIO pin
			RTL_W32(PEFGH_DIR, RTL_R32(PEFGH_DIR) | (1 << (hled->gpio - 32))); // Configured as output pin
		}
		led_set_state(hled, hled->state); // init vaule
		res = create_proc_entry(hled->name, 0644, act_config);
		if (res) {
			res->read_proc = hled->read_proc;
			res->write_proc = hled->write_proc;
			res->data = hled;
		}
		else 
		{
			printk("Realtek GPIO Driver, create proc failed!\n");
			return -ENOENT;
		}

		// init blink_timer
		init_timer(&hled->on_timer);
		hled->on_timer.function = (void (*)(unsigned long))led_on_timer_func;
		hled->on_timer.data = (unsigned long)hled;
		init_timer(&hled->off_timer);
		hled->off_timer.function = (void (*)(unsigned long))led_off_timer_func;
		hled->off_timer.data = (unsigned long)hled;
	}

	return 0;
}

static void hw_led_uninit(void)
{
	return;
}

#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE

#define SYSTEM_CONTRL_DUMMY_REG 0xb8003504

int is_bank2_root(void)
{
	//boot code will steal System's dummy register bit0 (set to 1 ---> bank2 booting
	//for 8198 formal chip 
	if ((RTL_R32(SYSTEM_CONTRL_DUMMY_REG)) & (0x00000001))  // steal for boot bank idenfy
		return 1;

	return 0;
}
static int read_bootbank_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	char flag='1';

	if (is_bank2_root())  // steal for boot bank idenfy
		flag='2';
		
	len = sprintf(page, "%c\n", flag);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;
	return len;
}
#endif

static int rf_switch_read_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int len;
	char flag;

	if (RTL_R32(WIFI_ONOFF_PIN_DATABASE) & (1<<WIFI_ONOFF_PIN_NO)){
		flag = '1';
	}else{
		flag = '0';
	}
	len = sprintf(page, "%c\n", flag);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len > count) len = count;
	if (len < 0) len = 0;
	return len;
}



#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE	
int bootbank_init(void)
{
	struct proc_dir_entry *res=NULL;
	res = create_proc_entry("bootbank", 0, NULL);
	if (res) {
		res->read_proc = read_bootbank_proc;
		//res->write_proc = write_bootbank_proc;
	}
	else {
		printk("read bootbank, create proc failed!\n");
	}

	return 0;
}
#endif

#if defined(CONFIG_AEI_HW_WECB3000)

int set_wlan_led_by_cpu(unsigned int is5G, unsigned int flag)//0-On, 1-Off
{
    if(gpio_get_val(BOARD_GPP_LED_SEL)){
        return -1;
    }
    if(is5G){
        gpio_set_val(BOARD_GPP_WIFI5G_LED, flag);
    }else{
        gpio_set_val(BOARD_GPP_WIFI2G_LED, flag);
    }
    return 0;
}
#endif

int __init hw_gpio_init(void)
{
	hw_buttons_init();
	hw_led_init();
#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
	bootbank_init();
#endif
	return 0;
}


void __exit hw_gpio_uninit(void)
{
	hw_buttons_uninit();
	hw_led_uninit();
	return;
}

module_init(hw_gpio_init);
module_exit(hw_gpio_uninit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO driver for Reload default");

#endif
