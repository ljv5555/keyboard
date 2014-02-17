#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usb_keyboard.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

int main(void)
{
    uint8_t d, mask, i;
    uint8_t d_prev=0xFF;

    CPU_PRESCALE(0);

    // DDRx 0 input 1 output
    // PORTx
    //  DDRx 0 - 0 normal 1 pullup resistor
    //  DDRx 1 - 0 low output 1 high output
    // PINx read the pin

    //DDRD = 0x00;
    //PORTD = 0xFF;
    DDRD &= ~(1<<2);
    PORTD |= (1<<2);

    DDRB |= (1<<2);
    // low output
    //PORTB &= ~(1<<2);
    // high output
    PORTB |= (1<<2);

    usb_init();
    while (!usb_configured()) /* wait */ ;
    _delay_ms(1000);

    while (1) {
        d = PIND;
        if (((d & (1<<2)) == 0) && (d_prev & (1<<2)) != 0) {
        // this would work as well (without previous) but would
        // type a lot of characters on single press. pullup resistor
        // is 0 when connected to ground
        //if ((d & (1<<2)) == 0) {
            usb_keyboard_press(KEY_D, KEY_SHIFT);
        }
        d_prev = d;
        _delay_ms(2);
    }
}
