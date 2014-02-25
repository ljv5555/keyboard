#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usb_keyboard.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

int main(void)
{
    uint8_t number_keys[10] =
        {KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9};
    uint8_t column, row, x, i, z, column_mask, row_mask, b;
    uint8_t previous_rows[8];

    CPU_PRESCALE(0);

    // DDRx 0 input 1 output
    // PORTx
    //  DDRx 0 - 0 normal 1 pullup resistor
    //  DDRx 1 - 0 low output 1 high output
    // PINx read the pin

    // high output
    DDRD = 0xFF;
    PORTD = 0xFF;

    // pullup resistor input
    DDRB = 0x00;
    PORTB = 0xFF;

    usb_init();
    while (!usb_configured());
    _delay_ms(1000);

    // works perfectly with one column "D"
    // and multiple rows "B"
    // but one row and multiple columns multiple keypresses don't work

    while (1) {
        for (column = 0; column < 8; column++) {
            PORTD &= ~(1<<column);
            _delay_ms(2);
            b = PINB;
            PORTD |= (1<<column);
            for (row = 0; row < 8; row++) {
                if ((b & (1<<row)) == 0) {
                    if ((previous_rows[column] & (1<<row)) != 0) {
                        usb_keyboard_press(number_keys[column], 0);
                        usb_keyboard_press(number_keys[row], 0);
                        usb_keyboard_press(KEY_ENTER, 0);
                        usb_keyboard_press(KEY_ENTER, 0);
                    }
                }
            }
            previous_rows[column] = b;
        }
        for (column = 0; column < 8; column++) {
            x = previous_rows[column];
            for (row = 0; row < 8; row++) {
                if ((x & (1<<row)) == 0) {
                    i = 0;
                } else {
                    i = 1;
                }
                usb_keyboard_press(number_keys[i], 0);
            }
            usb_keyboard_press(KEY_ENTER, 0);
        }
        usb_keyboard_press(KEY_ENTER, 0);
        usb_keyboard_press(KEY_ENTER, 0);
        _delay_ms(1000);
    }
}

