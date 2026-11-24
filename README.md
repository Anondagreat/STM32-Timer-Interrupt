# STM32F411 Timer & Interrupt Project

Repository ini berisi implementasi tugas mandiri mata kuliah **Mikroprosesor dan Antarmuka / Sistem Embedded** menggunakan mikrokontroler STM32F411CEU6 Black Pill.

Seluruh program dibuat menggunakan bahasa C dengan metode **Bare-Metal Programming** (akses register langsung) tanpa menggunakan HAL Library.

## Studi Kasus

### 1. Blinking Multiple LEDs at Different Frequencies
Menggunakan tiga hardware timer (TIM2, TIM3, TIM4) untuk mengendalikan tiga LED dengan frekuensi kedipan berbeda.

| Timer | GPIO | Frekuensi |
|---|---|---|
| TIM2 | PA0 | 1 Hz |
| TIM3 | PA1 | 2 Hz |
| TIM4 | PA2 | 4 Hz |

### 2. Debouncing a Pushbutton with a Timer (Semaphore)
Menggunakan EXTI0 dan TIM5 untuk mengatasi bouncing pada pushbutton dengan waktu debounce 30 ms. Variabel flag digunakan sebagai semaphore untuk komunikasi antara ISR dan program utama.

### 3. Push to Blink
Menggunakan TIM2 dan pushbutton untuk membuat LED berkedip selama tombol ditekan dan berhenti ketika tombol dilepaskan.

## Hardware yang Digunakan
- STM32F411CEU6 Black Pill
- ST-Link V2
- 3 LED eksternal
- Resistor 220–330 Ω
- Pushbutton
- Breadboard dan kabel jumper

## Software
- STM32CubeIDE
- Bahasa pemrograman C
- CMSIS / STM32F4 CMSIS Device Header

## Konfigurasi Clock
- Clock source: HSI
- Frekuensi clock: 16 MHz
- APB1 Prescaler: 1

## Struktur Repository

```text
STM32F411_Timer_Interrupt/
├── Blinking_multiple_led/
|   └──Src/
│       └── main.c
├── Debouncing_pushbutton/
|   └──Src/
│       └── main.c
├── Push_to_blink/
|   └──Src/
│       └── main.c
├── Report/
│   └── Laporan.pdf
└── README.md
```

## Pengujian
Pengujian dilakukan secara langsung pada development board STM32F411CEU6 Black Pill dengan mengamati respons LED dan pushbutton. Dokumentasi pengujian berupa foto rangkaian dan tangkapan layar kode program. Pengujian tidak menggunakan Logic Analyzer.