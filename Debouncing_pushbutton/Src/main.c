
#include "stm32f4xx.h"

// Flag semaphore untuk komunikasi ISR dan main
volatile uint8_t button_event = 0;

// Menyimpan kondisi tombol sebelumnya
volatile uint8_t button_state = 0;

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

    // Matikan LED
    GPIOA->ODR &= ~(1U << 3);

    // PB0 sebagai input
    GPIOB->MODER &= ~(3U << 0);

    // Mengaktifkan internal pull-up PB0
    GPIOB->PUPDR &= ~(3U << 0);
    GPIOB->PUPDR |= (1U << 0);
}

/* ==========================================
   EXTI INITIALIZATION
   ========================================== */

void EXTI_Init(void)
{
    // Aktifkan clock SYSCFG
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Hubungkan EXTI0 ke PB0
    SYSCFG->EXTICR[0] &= ~(0xFU);
    SYSCFG->EXTICR[0] |= (1U << 0);

    // Deteksi falling edge
    EXTI->FTSR |= (1U << 0);

    // Deteksi rising edge
    EXTI->RTSR |= (1U << 0);

    // Hapus pending interrupt
    EXTI->PR = (1U << 0);

    // Aktifkan EXTI line 0
    EXTI->IMR |= (1U << 0);

    // Aktifkan interrupt EXTI0 di NVIC
    NVIC_EnableIRQ(EXTI0_IRQn);
}

/* ==========================================
   TIMER 5 INITIALIZATION
   ========================================== */

void TIM5_Init(void)
{
    // Aktifkan clock TIM5
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

    // Timer counter 1 kHz
    TIM5->PSC = 15999;

    // Periode 30 ms
    TIM5->ARR = 29;

    // Counter mulai dari nol
    TIM5->CNT = 0;

    // Generate update event
    TIM5->EGR = TIM_EGR_UG;

    // Hapus interrupt flag
    TIM5->SR = 0;

    // Aktifkan update interrupt
    TIM5->DIER |= TIM_DIER_UIE;

    // Aktifkan one-pulse mode
    TIM5->CR1 |= TIM_CR1_OPM;

    // Aktifkan interrupt TIM5
    NVIC_EnableIRQ(TIM5_IRQn);
}

/* ==========================================
   EXTI INTERRUPT HANDLER
   ========================================== */

void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (1U << 0))
    {
        // Hapus pending EXTI
        EXTI->PR = (1U << 0);

        // Nonaktifkan EXTI sementara
        EXTI->IMR &= ~(1U << 0);

        // Reset timer
        TIM5->CR1 &= ~TIM_CR1_CEN;
        TIM5->CNT = 0;
        TIM5->SR = 0;

        // Jalankan TIM5
        TIM5->CR1 |= TIM_CR1_CEN;
    }
}

/* ==========================================
   TIM5 INTERRUPT HANDLER
   ========================================== */

void TIM5_IRQHandler(void)
{
    if (TIM5->SR & TIM_SR_UIF)
    {
        // Hapus update flag
        TIM5->SR &= ~TIM_SR_UIF;

        // Baca kondisi tombol
        uint8_t current_state =
            !(GPIOB->IDR & (1U << 0));

        // Periksa perubahan kondisi tombol
        if (current_state != button_state)
        {
            button_state = current_state;

            // Jika tombol ditekan
            if (button_state == 1)
            {
                button_event = 1;
            }
        }

        // Bersihkan pending EXTI
        EXTI->PR = (1U << 0);

        // Aktifkan kembali EXTI
        EXTI->IMR |= (1U << 0);
    }
}

/* ==========================================
   MAIN PROGRAM
   ========================================== */

int main(void)
{
    // Inisialisasi GPIO
    GPIO_Init();

    // Inisialisasi EXTI
    EXTI_Init();

    // Inisialisasi timer
    TIM5_Init();

    while (1)
    {
        // Periksa semaphore
        if (button_event == 1)
        {
            // Reset semaphore
            button_event = 0;

            // Toggle LED
            GPIOA->ODR ^= (1U << 3);
        }
    }
}
