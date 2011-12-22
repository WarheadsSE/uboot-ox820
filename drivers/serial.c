/*
 * (C) Copyright 2000
 * Rob Taylor, Flying Pig Systems. robt@flyingpig.com.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

#ifdef CFG_NS16550_SERIAL

#include <ns16550.h>
#ifdef CFG_NS87308
#include <ns87308.h>
#endif

#if CONFIG_CONS_INDEX == 1
static NS16550_t console = (NS16550_t) CFG_NS16550_COM1;
#elif CONFIG_CONS_INDEX == 2
static NS16550_t console = (NS16550_t) CFG_NS16550_COM2;
#elif CONFIG_CONS_INDEX == 3
static NS16550_t console = (NS16550_t) CFG_NS16550_COM3;
#elif CONFIG_CONS_INDEX == 4
static NS16550_t console = (NS16550_t) CFG_NS16550_COM4;
#else
#error no valid console defined
#endif

static int calc_divisor (void)
{
	DECLARE_GLOBAL_DATA_PTR;
#ifdef CONFIG_OMAP1510
	/* If can't cleanly clock 115200 set div to 1 */
	if ((CFG_NS16550_CLK == 12000000) && (gd->baudrate == 115200)) {
		console->osc_12m_sel = OSC_12M_SEL;	/* enable 6.5 * divisor */
		return (1);				/* return 1 for base divisor */
	}
	console->osc_12m_sel = 0;			/* clear if previsouly set */
#endif
#ifdef CONFIG_OMAP1610
	/* If can't cleanly clock 115200 set div to 1 */
	if ((CFG_NS16550_CLK == 48000000) && (gd->baudrate == 115200)) {
		return (26);		/* return 26 for base divisor */
	}
#endif

#ifdef USE_UART_FRACTIONAL_DIVIDER
	return (((CFG_NS16550_CLK << 4) / gd->baudrate) + 8) >> 4;	
#endif // USE_UART_FRACTIONAL_DIVIDER

    // Round to nearest integer
    return (((CFG_NS16550_CLK / gd->baudrate) + 8 ) / 16);
}

int serial_init (void)
{
	int clock_divisor = calc_divisor();

#ifdef CFG_NS87308
	initialise_ns87308();
#endif

	NS16550_init(console, clock_divisor);

	return (0);
}

void
serial_putc(const char c)
{
	if (c == '\n')
		NS16550_putc(console, '\r');

	NS16550_putc(console, c);
}

void
serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}


int
serial_getc(void)
{
	return NS16550_getc(console);
}

int
serial_tstc(void)
{
	return NS16550_tstc(console);
}

void
serial_setbrg (void)
{
	int clock_divisor;

    clock_divisor = calc_divisor();
	NS16550_reinit(console, clock_divisor);
}

#endif
