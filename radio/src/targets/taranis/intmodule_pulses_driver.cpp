/*
 * Copyright (C) EdgeTX
 *
 * Based on code named
 *   opentx - https://github.com/opentx/opentx
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "intmodule_pulses_driver.h"

#include "hal.h"
#include "board.h"
#include "timers_driver.h"

#if defined(INTMODULE_TIMER)

static stm32_pulse_dma_tc_cb_t _int_timer_DMA_TC_Callback;

const stm32_pulse_timer_t intmoduleTimer = {
  .GPIOx = INTMODULE_TX_GPIO,
  .GPIO_Pin = INTMODULE_TX_GPIO_PIN,
  .GPIO_Alternate = INTMODULE_TX_GPIO_AF,
  .TIMx = INTMODULE_TIMER,
  .TIM_Freq = INTMODULE_TIMER_FREQ,
  .TIM_Channel = INTMODULE_TIMER_Channel,
  .TIM_IRQn = INTMODULE_TIMER_IRQn,
  .DMAx = INTMODULE_TIMER_DMA,
  .DMA_Stream = INTMODULE_TIMER_DMA_STREAM,
  .DMA_Channel = INTMODULE_TIMER_DMA_CHANNEL,
  .DMA_IRQn = INTMODULE_TIMER_DMA_STREAM_IRQn,
  .DMA_TC_CallbackPtr = &_int_timer_DMA_TC_Callback,
};

// Make sure the timer channel is supported
static_assert(__STM32_PULSE_IS_TIMER_CHANNEL_SUPPORTED(INTMODULE_TIMER_Channel),
              "Unsupported timer channel");

// Make sure the DMA channel is supported
static_assert(__STM32_PULSE_IS_DMA_STREAM_SUPPORTED(INTMODULE_TIMER_DMA_STREAM),
              "Unsupported DMA stream");

#if !defined(INTMODULE_TIMER_DMA_IRQHandler)
#error "Missing INTMODULE_TIMER_DMA_IRQHandler definition"
#endif

extern "C" void INTMODULE_TIMER_DMA_IRQHandler()
{
  stm32_pulse_dma_tc_isr(&intmoduleTimer);
}

#if !defined(INTMODULE_TIMER_IRQHandler)
#error "Missing INTMODULE_TIMER_IRQHandler definition"
#endif

extern "C" void INTMODULE_TIMER_IRQHandler()
{
  stm32_pulse_tim_update_isr(&intmoduleTimer);
}

#endif // INTMODULE_TIMER
