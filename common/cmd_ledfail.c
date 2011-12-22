
#include <common.h>
#include <command.h>

#if (CONFIG_COMMANDS & CFG_CMD_LEDFAIL)
#define FAILURE_LED (1 << (34-32))

#define GPIO_B	0x44100000
#define WARN_GPIO_OUT_REG			(GPIO_B + 0x10)
#define WARN_GPIO_OUT_ENABLE_SET	(GPIO_B + 0x1C)
#define WARN_GPIO_OUT_ENABLE_CLR	(GPIO_B + 0x20)

static void ledfail_light(void)
{
	printf("Light LED\n");
	/* Light the failure LED - assumes active low drive */
	u_int32_t led_state = *((volatile u_int32_t *)WARN_GPIO_OUT_REG);
	led_state = led_state & ~FAILURE_LED;
	*((volatile u_int32_t *)WARN_GPIO_OUT_REG) = led_state;

	/* Enable GPIO for output */
	*((volatile u_int32_t *)WARN_GPIO_OUT_ENABLE_SET) = FAILURE_LED;
}

static void ledfail_extinguish(void)
{
	printf("Extinguish LED\n");
	/* Extinguish the failure LED - assumes active low drive */
	u_int32_t led_state = *((volatile u_int32_t *)WARN_GPIO_OUT_REG);
	led_state = led_state | FAILURE_LED;
	*((volatile u_int32_t *)WARN_GPIO_OUT_REG) = led_state;

    /* Clear the failure bit output enable in GPIO's */
	*((volatile u_int32_t *)WARN_GPIO_OUT_ENABLE_CLR) = FAILURE_LED;
}

int do_ledfail(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	ulong arg = simple_strtoul(argv[1], NULL, 10);
	switch (arg) {
		case 0:
			ledfail_extinguish();
			break;
		case 1:
			ledfail_light();
			break;
	}

	return 0;
}

U_BOOT_CMD(ledfail, 2, 2, do_ledfail, "ledfail - Extinguish (0) or light (1) failure LED\n", NULL);
#endif	/* CFG_CMD_LEDFAIL */
