
#include "stm32f4xx.h"

/* ==========================================
   GPIO INITIALIZATION
   ========================================== */

void GPIO_Init(void)
{
    // Aktifkan clock GPIOA dan GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN |
                    RCC_AHB1ENR_GPIOBEN;

    // PA3 sebagai output
    GPIOA->MODER &= ~(3U << 6);
    GPIOA->MODER |= (1U << 6);

    // Matikan LED pada awal program
    GPIOA->ODR &= ~(1U << 3);

    // PB0 sebagai input
    GPIOB->MODER &= ~(3U << 0);

    // Aktifkan pull-up internal PB0
    GPIOB->PUPDR &= ~(3U << 0);
    GPIOB->PUPDR |= (1U << 0);
}

/* ==========================================
   TIMER 2 INITIALIZATION
   ========================================== */

void TIM2_Init(void)
{
    // Aktifkan clock TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Prescaler 15999
    TIM2->PSC = 15999;

    // Auto reload 249
    TIM2->ARR = 249;

    // Counter mulai dari nol
    TIM2->CNT = 0;

    // Generate update event
    TIM2->EGR = TIM_EGR_UG;

    // Hapus update interrupt flag
    TIM2->SR = 0;

    // Aktifkan update interrupt
    TIM2->DIER |= TIM_DIER_UIE;

    // Aktifkan interrupt TIM2 di NVIC
    NVIC_EnableIRQ(TIM2_IRQn);

    // Mulai timer
    TIM2->CR1 |= TIM_CR1_CEN;
}

/* ==========================================
   TIMER 2 INTERRUPT HANDLER
   ========================================== */

void TIM2_IRQHandler(void)
{
    // Periksa update interrupt flag
    if (TIM2->SR & TIM_SR_UIF)
    {
        // Hapus interrupt flag
        TIM2->SR &= ~TIM_SR_UIF;

        // Periksa apakah tombol ditekan
        if (!(GPIOB->IDR & (1U << 0)))
        {
            // Toggle LED PA3
            GPIOA->ODR ^= (1U << 3);
        }
        else
        {
            // Matikan LED ketika tombol dilepas
            GPIOA->ODR &= ~(1U << 3);
        }
    }
}

/* ==========================================
   MAIN PROGRAM
   ========================================== */

int main(void)
{
    // Inisialisasi GPIO
    GPIO_Init();

    // Inisialisasi TIM2
    TIM2_Init();

    while (1)
    {
        // Timer mengatur proses blinking
    }
}
