#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "os/os.h"
#include "os/os_trace_api.h"
#include "hal/hal_timer.h"

#include "periph_conf.h"
#include "periph/timer.h"

#define HWTIMER TIMER_DEV(1)

typedef void (*hal_timer_cb)(void *arg);

struct nrf52_hal_timer {
    uint32_t timer_isrs;
    TAILQ_HEAD(hal_timer_qhead, hal_timer) hal_timer_q;
};

static struct nrf52_hal_timer the_bsptimer; // the one and only

static void timer_callback(void *arg, int channel) {
    // this is a timer "interrupt"
    uint32_t tcntr;
    os_sr_t sr;
    struct hal_timer *timer;
    struct nrf52_hal_timer *bsptimer = &the_bsptimer;
    os_trace_isr_enter();
    OS_ENTER_CRITICAL(sr);

    /* Count # of timer isrs */
    ++bsptimer->timer_isrs;

    while ((timer = TAILQ_FIRST(&bsptimer->hal_timer_q)) != NULL) {
        tcntr = timer_read(HWTIMER);
        if ((int32_t)(tcntr - timer->expiry) >= 0) {
            TAILQ_REMOVE(&bsptimer->hal_timer_q, timer, link);
            timer->link.tqe_prev = NULL;
            timer->cb_func(timer->cb_arg);
        } else {
            break;
        }
    }

    /* Any timers left on queue? If so, we need to set OCMP */
    timer = TAILQ_FIRST(&bsptimer->hal_timer_q);
    if (timer) {
        timer_set_absolute(HWTIMER, 0, timer->expiry);
    } else {
        timer_clear(HWTIMER, 0);
    }

    OS_EXIT_CRITICAL(sr);
    os_trace_isr_exit();
}

int hal_timer_init(int timer_num, void *cfg){
    assert(timer_num == 5);

    return 0;
}

int hal_timer_config(int timer_num, uint32_t freq_hz){
    int res = 0;
    assert(timer_num == 5);

    res = timer_init(HWTIMER, freq_hz, timer_callback, NULL);

    return res;
}

int hal_timer_set_cb(int timer_num, struct hal_timer *timer, hal_timer_cb cb_func, void *arg){
    assert(timer_num == 5);

    timer->cb_func = cb_func;
    timer->cb_arg = arg;
    timer->link.tqe_prev = NULL;
    timer->bsp_timer = &the_bsptimer;

    return 0;
}

static int c = 0;

int hal_timer_start_at(struct hal_timer *timer, uint32_t tick){
    os_sr_t sr;
    struct hal_timer *entry;
    struct nrf52_hal_timer *bsptimer;

    if ((timer == NULL) || (timer->link.tqe_prev != NULL) ||
        (timer->cb_func == NULL)) {
        return OS_EINVAL;
    }
    bsptimer = (struct nrf52_hal_timer *)timer->bsp_timer;
    timer->expiry = tick;

    OS_ENTER_CRITICAL(sr);

    if (TAILQ_EMPTY(&bsptimer->hal_timer_q)) {
        TAILQ_INSERT_HEAD(&bsptimer->hal_timer_q, timer, link);
    } else {
        TAILQ_FOREACH(entry, &bsptimer->hal_timer_q, link) {
            if ((int32_t)(timer->expiry - entry->expiry) < 0) {
                TAILQ_INSERT_BEFORE(entry, timer, link);
                break;
            }
        }
        if (!entry) {
            TAILQ_INSERT_TAIL(&bsptimer->hal_timer_q, timer, link);
        }
    }

    /* If this is the head, we need to set new OCMP */
    if (timer == TAILQ_FIRST(&bsptimer->hal_timer_q)) {
        /* No OCMP here, but we still can set the timer to fire */
            unsigned int now = timer_read(HWTIMER);
            if(now < timer->expiry) {
                timer_set_absolute(HWTIMER, 0, timer->expiry);
            } else {
                // we missed a timer? not good, but let's try to schedule!
                timer_set_absolute(HWTIMER, 0, now + 2);
            }
        
    }
    
    OS_EXIT_CRITICAL(sr);

    return 0;
}

int hal_timer_stop(struct hal_timer *timer){
    os_sr_t sr;
    int reset_ocmp;
    struct hal_timer *entry;
    struct nrf52_hal_timer *bsptimer;

    if (timer == NULL) {
        return OS_EINVAL;
    }

    bsptimer = (struct nrf52_hal_timer *)timer->bsp_timer;

    OS_ENTER_CRITICAL(sr);

    if (timer->link.tqe_prev != NULL) {
        reset_ocmp = 0;
        if (timer == TAILQ_FIRST(&bsptimer->hal_timer_q)) {
            /* If first on queue, we will need to reset OCMP */
            entry = TAILQ_NEXT(timer, link);
            reset_ocmp = 1;
        }
        TAILQ_REMOVE(&bsptimer->hal_timer_q, timer, link);
        timer->link.tqe_prev = NULL;
        if (reset_ocmp) {
            if (entry) {
                timer_set_absolute(HWTIMER, 0, entry->expiry);
            } else {
                timer_clear(HWTIMER, 0);
            }
        }
    }

    OS_EXIT_CRITICAL(sr);

    return 0;
}

uint32_t hal_timer_read(int timer_num){
    assert(timer_num == 5);

    return timer_read(HWTIMER);
}
