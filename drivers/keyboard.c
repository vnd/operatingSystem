#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "monitor.h"

static void keyboard_callback(registers_t regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    u8_int scancode = in_byte(0x60);
    s8_int* sc_ascii;
    i_t_o_a(scancode, sc_ascii);
    k_print("Keyboard scancode: ");
    k_print(sc_ascii);
    k_print(", ");
    print_letter(scancode);
    k_print("\n");
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 
}

void print_letter(u8_int scancode) {
    switch (scancode) {
        case 0x0:
            k_print("ERROR");
            break;
        case 0x1:
            k_print("ESC");
            break;
        case 0x2:
            k_print("1");
            break;
        case 0x3:
            k_print("2");
            break;
        case 0x4:
            k_print("3");
            break;
        case 0x5:
            k_print("4");
            break;
        case 0x6:
            k_print("5");
            break;
        case 0x7:
            k_print("6");
            break;
        case 0x8:
            k_print("7");
            break;
        case 0x9:
            k_print("8");
            break;
        case 0x0A:
            k_print("9");
            break;
        case 0x0B:
            k_print("0");
            break;
        case 0x0C:
            k_print("-");
            break;
        case 0x0D:
            k_print("+");
            break;
        case 0x0E:
            k_print("Backspace");
            break;
        case 0x0F:
            k_print("Tab");
            break;
        case 0x10:
            k_print("Q");
            break;
        case 0x11:
            k_print("W");
            break;
        case 0x12:
            k_print("E");
            break;
        case 0x13:
            k_print("R");
            break;
        case 0x14:
            k_print("T");
            break;
        case 0x15:
            k_print("Y");
            break;
        case 0x16:
            k_print("U");
            break;
        case 0x17:
            k_print("I");
            break;
        case 0x18:
            k_print("O");
            break;
        case 0x19:
            k_print("P");
            break;
		case 0x1A:
			k_print("[");
			break;
		case 0x1B:
			k_print("]");
			break;
		case 0x1C:
			k_print("ENTER");
			break;
		case 0x1D:
			k_print("LCtrl");
			break;
		case 0x1E:
			k_print("A");
			break;
		case 0x1F:
			k_print("S");
			break;
        case 0x20:
            k_print("D");
            break;
        case 0x21:
            k_print("F");
            break;
        case 0x22:
            k_print("G");
            break;
        case 0x23:
            k_print("H");
            break;
        case 0x24:
            k_print("J");
            break;
        case 0x25:
            k_print("K");
            break;
        case 0x26:
            k_print("L");
            break;
        case 0x27:
            k_print(";");
            break;
        case 0x28:
            k_print("'");
            break;
        case 0x29:
            k_print("`");
            break;
		case 0x2A:
			k_print("LShift");
			break;
		case 0x2B:
			k_print("\\");
			break;
		case 0x2C:
			k_print("Z");
			break;
		case 0x2D:
			k_print("X");
			break;
		case 0x2E:
			k_print("C");
			break;
		case 0x2F:
			k_print("V");
			break;
        case 0x30:
            k_print("B");
            break;
        case 0x31:
            k_print("N");
            break;
        case 0x32:
            k_print("M");
            break;
        case 0x33:
            k_print(",");
            break;
        case 0x34:
            k_print(".");
            break;
        case 0x35:
            k_print("/");
            break;
        case 0x36:
            k_print("Rshift");
            break;
        case 0x37:
            k_print("Keypad *");
            break;
        case 0x38:
            k_print("LAlt");
            break;
        case 0x39:
            k_print("Spc");
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80 
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                k_print("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                k_print("key up ");
                print_letter(scancode - 0x80);
            } else k_print("Unknown key up");
            break;
    }
}