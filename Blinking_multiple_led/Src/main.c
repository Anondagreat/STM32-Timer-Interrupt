
#include "stm32f4xx.h"

/* ==========================================
   GPIO INITIALIZATION
   ========================================== */

void GPIO_Init(void)
{
    // Mengaktifkan clock GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // PA3, PA4, PA5 diatur sebagai output
    GPIOA->MODER &= ~((3U << 6) |
                     (3U << 8) |
                     (3U << 10));

    GPIOA->MODER |= ((1U << 6) |
                    (1U << 8) |
                    (1U << 10));

    // Mematikan semua LED pada awal program
    GPIOA->ODR &= ~((1U << 3) |
                    (1U << 4) |
                    (1U << 5));
}

/* ==========================================
   TIMER INITIALIZATION
   ========================================== */

void TIM_Init(void)
{
    // Mengaktifkan clock TIM2, TIM3, TIM4
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN |
                    RCC_APB1ENR_TIM3EN |
                    RCC_APB1ENR_TIM4EN;

    // TIM2: interval 500 ms
    TIM2->PSC = 15999;
    TIM2->ARR = 499;

    // Update event untuk memuat PSC
    TIM2->EGR = TIM_EGR_UG;

    // Menghapus update interrupt flag
    TIM2->SR = 0;

    // Mengaktifkan update interrupt
    TIM2->DIER |= TIM_DIER_UIE;

    // TIM3: interval 250 ms
    TIM3->PSC = 15999;
    TIM3->ARR = 249;

    TIM3->EGR = TIM_EGR_UG;
    TIM3->SR = 0;
    TIM3->DIER |= TIM_DIER_UIE;

    // TIM4: interval 100 ms
    TIM4->PSC = 15999;
    TIM4->ARR = 99;

    TIM4->EGR = TIM_EGR_UG;
    TIM4->SR = 0;
    TIM4->DIER |= TIM_DIER_UIE;

    // Mengaktifkan interrupt TIM2 pada NVIC
    NVIC_EnableIRQ(TIM2_IRQn);

    // Mengaktifkan interrupt TIM3 pada NVIC
    NVIC_EnableIRQ(TIM3_IRQn);

    // Mengaktifkan interrupt TIM4 pada NVIC
    NVIC_EnableIRQ(TIM4_IRQn);

    // Menjalankan TIM2
    TIM2->CR1 |= TIM_CR1_CEN;

    // Menjalankan TIM3
    TIM3->CR1 |= TIM_CR1_CEN;

    // Menjalankan TIM4
    TIM4->CR1 |= TIM_CR1_CEN;
}

/* ==========================================
   INTERRUPT HANDLERS
   ========================================== */

// Interrupt TIM2 untuk LED PA3
void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        // Menghapus interrupt flag
        TIM2->SR &= ~TIM_SR_UIF;

        // Toggle LED PA3
        GPIOA->ODR ^= (1U << 3);
    }
}

// Interrupt TIM3 untuk LED PA4
void TIM3_IRQHandler(void)
{
    if (TIM3->SR & TIM_SR_UIF)
    {
        TIM3->SR &= ~TIM_SR_UIF;

        // Toggle LED PA4
        GPIOA->ODR ^= (1U << 4);
    }
}

// Interrupt TIM4 untuk LED PA5
void TIM4_IRQHandler(void)
{
    if (TIM4->SR & TIM_SR_UIF)
    {
        TIM4->SR &= ~TIM_SR_UIF;

        // Toggle LED PA5
        GPIOA->ODR ^= (1U << 5);
    }
}

/* ==========================================
   MAIN PROGRAM
   ========================================== */

int main(void)
{
    // Inisialisasi GPIO
    GPIO_Init();

    // Inisialisasi timer
    TIM_Init();

    while (1)
    {
        // LED dikendalikan oleh interrupt timer
    }
}
