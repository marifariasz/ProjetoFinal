#include <stdio.h>
#include "pico/stdlib.h"
#include <hardware/adc.h>
#include <hardware/pwm.h>

#define EIXO_Y 26  // Eixo Y do Joystick ADC 0
#define LED_MAX 12  // Pino do LED que acende no máximo do joystick
#define LED_MIN 13  // Pino do LED que acende caso contrário
#define LIMIAR 4000 // Valor do ADC próximo ao máximo

int main() {
    stdio_init_all();
    
    // Inicializa ADC
    adc_init();
    adc_gpio_init(EIXO_Y);
    adc_select_input(0);

    // Configura os pinos dos LEDs como saída
    gpio_init(LED_MIN);
    gpio_set_dir(LED_MIN, GPIO_OUT);
    
    // Configuração PWM para LED_MAX
    gpio_set_function(LED_MAX, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(LED_MAX);
    pwm_set_enabled(slice, true);
    pwm_set_gpio_level(LED_MAX, 0);

    while (true) {
        uint16_t result = adc_read(); // Lê o valor do joystick

        if (result >= LIMIAR) {
            // Acende LED_MAX (PWM no brilho máximo) e apaga LED_MIN
            pwm_set_gpio_level(LED_MAX, 4095);
            gpio_put(LED_MIN, 0);
        } else {
            // Apaga LED_MAX e acende LED_MIN
            pwm_set_gpio_level(LED_MAX, 0);
            gpio_put(LED_MIN, 1);
        }

        sleep_ms(50);
    }
}
