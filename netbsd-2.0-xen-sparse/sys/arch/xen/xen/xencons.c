/*	$NetBSD: xencons.c,v 1.1.2.1 2004/05/22 15:59:21 he Exp $	*/

/*
 *
 * Copyright (c) 2004 Christian Limpach.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Christian Limpach.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: xencons.c,v 1.1.2.1 2004/05/22 15:59:21 he Exp $");

#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/proc.h>
#include <sys/tty.h>
#include <sys/systm.h>
#include <sys/device.h>
#include <sys/conf.h>

#include <machine/stdarg.h>
#include <machine/xen.h>
#include <machine/hypervisor.h>
#include <machine/evtchn.h>
#include <machine/ctrl_if.h>

#include <dev/cons.h>

#include <ddb/db_output.h>	/* XXX for db_max_line */

static int xencons_isconsole = 0;

#define	XENCONS_UNIT(x)	(minor(x))
#define XENCONS_BURST 128

int xencons_match (struct device *, struct cfdata *, void *);
void xencons_attach (struct device *, struct device *, void *);
/* int xencons_intr (void *); */
void xencons_init (void);

struct xencons_softc {
	struct	device sc_dev;
	struct	tty *sc_tty;
};

CFATTACH_DECL(xencons, sizeof(struct xencons_softc),
    xencons_match, xencons_attach, NULL, NULL);

extern struct cfdriver xencons_cd;

dev_type_open(xencons_open);
dev_type_close(xencons_close);
dev_type_read(xencons_read);
dev_type_write(xencons_write);
dev_type_ioctl(xencons_ioctl);
dev_type_stop(xencons_stop);
dev_type_tty(xencons_tty);
dev_type_poll(xencons_poll);

const struct cdevsw xencons_cdevsw = {
	xencons_open, xencons_close, xencons_read, xencons_write,
	xencons_ioctl, xencons_stop, xencons_tty, xencons_poll,
	NULL, ttykqfilter, D_TTY
};


static void xencons_rx(ctrl_msg_t *, unsigned long);
void xenconscn_attach(void);
int xenconscn_getc(dev_t);
void xenconscn_putc(dev_t, int);
void xenconscn_pollc(dev_t, int);

static struct consdev xencons = {
	NULL, NULL, xenconscn_getc, xenconscn_putc, xenconscn_pollc,
	NULL, NULL, NULL, NODEV, CN_NORMAL
};

void	xencons_start (struct tty *);
int	xencons_param (struct tty *, struct termios *);

int
xencons_match(struct device *parent, struct cfdata *match, void *aux)
{
	struct xencons_attach_args *xa = (struct xencons_attach_args *)aux;

	if (strcmp(xa->xa_device, "xencons") == 0)
		return 1;
	return 0;
}

void
xencons_attach(struct device *parent, struct device *self, void *aux)
{
	struct xencons_softc *sc = (void *)self;

	aprint_normal(": Xen Virtual Console Driver\n");

	if (xencons_isconsole) {
		int maj;

		/* Locate the major number. */
		maj = cdevsw_lookup_major(&xencons_cdevsw);

		/* There can be only one, but it can have any unit number. */
		cn_tab->cn_dev = makedev(maj, sc->sc_dev.dv_unit);

		aprint_verbose("%s: console major %d, unit %d\n",
		    sc->sc_dev.dv_xname, maj, sc->sc_dev.dv_unit);

		/* Set db_max_line to avoid paging. */
		db_max_line = 0x7fffffff;

		(void)ctrl_if_register_receiver(CMSG_CONSOLE, xencons_rx, 0);
	}
}

int
xencons_open(dev_t dev, int flag, int mode, struct proc *p)
{
	struct xencons_softc *sc;
	int unit = XENCONS_UNIT(dev);
	struct tty *tp;

	sc = device_lookup(&xencons_cd, unit);
	if (sc == NULL)
		return (ENXIO);

	if (!sc->sc_tty) {
		tp = sc->sc_tty = ttymalloc();
		tty_attach(tp);
	} else
		tp = sc->sc_tty;

	tp->t_oproc = xencons_start;
	tp->t_param = xencons_param;
	tp->t_dev = dev;
	if ((tp->t_state & TS_ISOPEN) == 0) {
		ttychars(tp);
		tp->t_iflag = TTYDEF_IFLAG;
		tp->t_oflag = TTYDEF_OFLAG;
		tp->t_cflag = TTYDEF_CFLAG;
		tp->t_lflag = TTYDEF_LFLAG;
		tp->t_ispeed = tp->t_ospeed = TTYDEF_SPEED;
		xencons_param(tp, &tp->t_termios);
		ttsetwater(tp);
	} else if (tp->t_state&TS_XCLUDE && p->p_ucred->cr_uid != 0)
		return (EBUSY);
	tp->t_state |= TS_CARR_ON;

	return ((*tp->t_linesw->l_open)(dev, tp));
}

int
xencons_close(dev_t dev, int flag, int mode, struct proc *p)
{
	struct xencons_softc *sc = device_lookup(&xencons_cd,
	    XENCONS_UNIT(dev));
	struct tty *tp = sc->sc_tty;

	if (tp == NULL)
		return (0);
	(*tp->t_linesw->l_close)(tp, flag);
	ttyclose(tp);
#ifdef notyet /* XXX */
	ttyfree(tp);
#endif
	return (0);
}

int
xencons_read(dev_t dev, struct uio *uio, int flag)
{
	struct xencons_softc *sc = device_lookup(&xencons_cd,
	    XENCONS_UNIT(dev));
	struct tty *tp = sc->sc_tty;

	return ((*tp->t_linesw->l_read)(tp, uio, flag));
}

int
xencons_write(dev_t dev, struct uio *uio, int flag)
{
	struct xencons_softc *sc = device_lookup(&xencons_cd,
	    XENCONS_UNIT(dev));
	struct tty *tp = sc->sc_tty;

	return ((*tp->t_linesw->l_write)(tp, uio, flag));
}

int
xencons_poll(dev_t dev, int events, struct proc *p)
{
	struct xencons_softc *sc = device_lookup(&xencons_cd,
	    XENCONS_UNIT(dev));
	struct tty *tp = sc->sc_tty;
 
	return ((*tp->t_linesw->l_poll)(tp, events, p));
}

struct tty *
xencons_tty(dev_t dev)
{
	struct xencons_softc *sc = device_lookup(&xencons_cd,
	    XENCONS_UNIT(dev));
	struct tty *tp = sc->sc_tty;

	return (tp);
}

int
xencons_ioctl(dev_t dev, u_long cmd, caddr_t data, int flag, struct proc *p)
{
	struct xencons_softc *sc = device_lookup(&xencons_cd,
	    XENCONS_UNIT(dev));
	struct tty *tp = sc->sc_tty;
	int error;

	error = (*tp->t_linesw->l_ioctl)(tp, cmd, data, flag, p);
	if (error != EPASSTHROUGH)
		return (error);

	error = ttioctl(tp, cmd, data, flag, p);
	if (error != EPASSTHROUGH)
		return (error);

	switch (cmd) {
	default:
		return (EPASSTHROUGH);
	}

#ifdef DIAGNOSTIC
	panic("xencons_ioctl: impossible");
#endif
}

void
xencons_start(struct tty *tp)
{
	struct clist *cl;
	int s, len;

	s = spltty();
	if (tp->t_state & (TS_TIMEOUT | TS_BUSY | TS_TTSTOP))
		goto out;
	tp->t_state |= TS_BUSY;
	splx(s);

	/*
	 * We need to do this outside spl since it could be fairly
	 * expensive and we don't want our serial ports to overflow.
	 */
	cl = &tp->t_outq;
	if (xen_start_info.flags & SIF_INITDOMAIN) {
		u_char buf[XENCONS_BURST+1];

		len = q_to_b(cl, buf, XENCONS_BURST);
		(void)HYPERVISOR_console_io(CONSOLEIO_write, len, buf);
	} else {
		ctrl_msg_t msg;

		len = q_to_b(cl, msg.msg, sizeof(msg.msg));
		msg.type = CMSG_CONSOLE;
		msg.subtype = CMSG_CONSOLE_DATA;
		msg.length = len;
		ctrl_if_send_message_noblock(&msg, NULL, 0);
		/* XXX check return value and queue wait for space
		 * thread/softint */
	}

	s = spltty();
	tp->t_state &= ~TS_BUSY;
	if (cl->c_cc) {
		tp->t_state |= TS_TIMEOUT;
		callout_reset(&tp->t_rstrt_ch, 1, ttrstrt, tp);
	}
	if (cl->c_cc <= tp->t_lowat) {
		if (tp->t_state & TS_ASLEEP) {
			tp->t_state &= ~TS_ASLEEP;
			wakeup(cl);
		}
		selwakeup(&tp->t_wsel);
	}
out:
	splx(s);
}

void
xencons_stop(struct tty *tp, int flag)
{

}


/* Non-privileged receive callback. */
static void
xencons_rx(ctrl_msg_t *msg, unsigned long id)
{
	int i;
	int s;
	// unsigned long flags;
	struct xencons_softc *sc;
	struct tty *tp;

	sc = device_lookup(&xencons_cd, XENCONS_UNIT(cn_tab->cn_dev));
	if (sc == NULL)
		goto out;

	tp = sc->sc_tty;
	if (tp == NULL)
		goto out;

	s = spltty();
	// save_and_cli(flags);
	// simple_lock(&xencons_lock);
	for (i = 0; i < msg->length; i++)
		(*tp->t_linesw->l_rint)(msg->msg[i], tp);
	// simple_unlock(&xencons_lock);
	// restore_flags(flags);
	splx(s);

 out:
	msg->length = 0;
	ctrl_if_send_response(msg);
}

void ctrl_if_early_init(void);
void
xenconscn_attach()
{

	cn_tab = &xencons;

	ctrl_if_early_init();

	xencons_isconsole = 1;
}

int
xenconscn_getc(dev_t dev)
{

	printf("\n");
	for (;;);
}

void
xenconscn_putc(dev_t dev, int c)
{
	extern int ctrl_if_evtchn;

	if (xen_start_info.flags & SIF_INITDOMAIN ||
		ctrl_if_evtchn == -1) {
		u_char buf[1];

		buf[0] = c;
		(void)HYPERVISOR_console_io(CONSOLEIO_write, 1, buf);
	} else {
		ctrl_msg_t msg;

		msg.type = CMSG_CONSOLE;
		msg.subtype = CMSG_CONSOLE_DATA;
		msg.length = 1;
		msg.msg[0] = c;
		while (ctrl_if_send_message_noblock(&msg, NULL, 0) == EAGAIN) {
			HYPERVISOR_yield();
			/* XXX check return value and queue wait for space
			 * thread/softint */
		}
	}
}

void
xenconscn_pollc(dev_t dev, int on)
{
	
}

/*
 * Set line parameters.
 */
int
xencons_param(struct tty *tp, struct termios *t)
{

	tp->t_ispeed = t->c_ispeed;
	tp->t_ospeed = t->c_ospeed;
	tp->t_cflag = t->c_cflag;
	return (0);
}

