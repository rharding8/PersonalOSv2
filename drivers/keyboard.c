#include "keyboard.h"
#include "./../cpu/vga_port.h"
#include "./../cpu/isr.h"
#include "./../cpu/types.h"
#include "./../libc/util.h"
#include "screen.h"

#define BUFFERSIZE 128

static int shift = 0;

static char buffer[BUFFERSIZE];
static int head = 0;
static int tail = 0;

char convert_char(uchar_8 scancode) {
    switch (scancode) {
        case 0x0:
            kline_break();
            kprint("ERR");
            kline_break();
            return 0;
        case 0x1:
            kprint("ESC");
            kline_break();
            return 0;

        case 0x2:
            return '1';
        case 0x3:
            return '2';
        case 0x4:
            return '3';
        case 0x5:
            return '4';
        case 0x6:
            return '5';
        case 0x7:
            return '6';
        case 0x8:
            return '7';
        case 0x9:
            return '8';
        case 0x0A:
            return '9';
        case 0x0B:
            return '0';


        case 0x0C:
            return '-';
        case 0x0D:
            return shift ? '+' : '=';
        case 0x0E:
            return '\b';
        case 0x0F:
            return '\t';
        

        case 0x10:
            return 'Q';
        case 0x11:
            return 'W';
        case 0x12:
            return 'E';
        case 0x13:
            return 'R';
        case 0x14:
            return 'T';
        case 0x15:
            return 'Y';
        case 0x16:
            return 'U';
        case 0x17:
            return 'I';
        case 0x18:
            return 'O';
        case 0x19:
            return 'P';


		case 0x1A:
			return '[';
		case 0x1B:
			return ']';
		case 0x1C:
			return '\n';


		case 0x1E:
			return 'A';
		case 0x1F:
			return 'S';
        case 0x20:
            return 'D';
        case 0x21:
            return 'F';
        case 0x22:
            return 'G';
        case 0x23:
            return 'H';
        case 0x24:
            return 'J';
        case 0x25:
            return 'K';
        case 0x26:
            return 'L';

        
        case 0x27:
            return ';';
        case 0x28:
            return '\'';
        case 0x29:
            return '`';
		case 0x2B:
			return '\\';


		case 0x2C:
			return 'Z';
		case 0x2D:
			return 'X';
		case 0x2E:
			return 'C';
		case 0x2F:
			return 'V';
        case 0x30:
            return 'B';
        case 0x31:
            return 'N';
        case 0x32:
            return 'M';


        case 0x33:
            return ',';
        case 0x34:
            return '.';
        case 0x35:
            return '/';
        case 0x37:
            return '*';
        case 0x39:
            return ' ';
        default: return 0;
    }
}

static void keyboard_callback(registers_t reg){
    uchar_8 scancode = port_byte_in(0x60);

    if (scancode == 0x2A || scancode == 0x36) {
        shift = 1;
        return;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
        shift = 0;
        return;
    }
    if (scancode & 0x80) return;

    char c = convert_char(scancode);
    if (!c) return;

    buffer[head] = c;
    head = (head + 1) % BUFFERSIZE;
}


void init_keyboard(){
    register_interrupt_handler(IRQ1, keyboard_callback);
}

char to_hex_char(uchar_8 nibble) {
    return (nibble < 10) ? (nibble + '0') : (nibble - 10 + 'A');
}

char get_char() {
    if (head == tail) return 0;

    char c = buffer[tail];
    tail = (tail + 1) % BUFFERSIZE;

    return c;
}

void print_letter(uchar_8 scancode) {
    switch (scancode) {
        case 0x0:
            kprint("ERROR");
            break;
        case 0x1:
            kprint("ESC");
            break;


        case 0x2:
            kprint("1");
            break;
        case 0x3:
            kprint("2");
            break;
        case 0x4:
            kprint("3");
            break;
        case 0x5:
            kprint("4");
            break;
        case 0x6:
            kprint("5");
            break;
        case 0x7:
            kprint("6");
            break;
        case 0x8:
            kprint("7");
            break;
        case 0x9:
            kprint("8");
            break;
        case 0x0A:
            kprint("9");
            break;
        case 0x0B:
            kprint("0");
            break;


        case 0x0C:
            kprint("-");
            break;
        case 0x0D:
            kprint("=");
            break;
        case 0x0E:
            // kprint("BackSpace");
            clear_one_character();
            break;
        case 0x0F:
            // kprint("Tab");
            kprint("    ");
            break;
        case 0x10:

        
            kprint("Q");
            break;
        case 0x11:
            kprint("W");
            break;
        case 0x12:
            kprint("E");
            break;
        case 0x13:
            kprint("R");
            break;
        case 0x14:
            kprint("T");
            break;
        case 0x15:
            kprint("Y");
            break;
        case 0x16:
            kprint("U");
            break;
        case 0x17:
            kprint("I");
            break;
        case 0x18:
            kprint("O");
            break;
        case 0x19:
            kprint("P");
            break;


		case 0x1A:
			kprint("[");
			break;
		case 0x1B:
			kprint("]");
			break;
		case 0x1C:
			// kprint("ENTER");
            kline_break();
			break;
		case 0x1D:
			kprint("LCtrl");
			break;


		case 0x1E:
			kprint("A");
			break;
		case 0x1F:
			kprint("S");
			break;
        case 0x20:
            kprint("D");
            break;
        case 0x21:
            kprint("F");
            break;
        case 0x22:
            kprint("G");
            break;
        case 0x23:
            kprint("H");
            break;
        case 0x24:
            kprint("J");
            break;
        case 0x25:
            kprint("K");
            break;
        case 0x26:
            kprint("L");
            break;


        case 0x27:
            kprint(";");
            break;
        case 0x28:
            kprint("'");
            break;
        case 0x29:
            kprint("`");
            break;
		case 0x2A:
			kprint("LShift");
			break;
		case 0x2B:
			kprint("\\");
			break;


		case 0x2C:
			kprint("Z");
			break;
		case 0x2D:
			kprint("X");
			break;
		case 0x2E:
			kprint("C");
			break;
		case 0x2F:
			kprint("V");
			break;
        case 0x30:
            kprint("B");
            break;
        case 0x31:
            kprint("N");
            break;
        case 0x32:
            kprint("M");
            break;


        case 0x33:
            kprint(",");
            break;
        case 0x34:
            kprint(".");
            break;
        case 0x35:
            kprint("/");
            break;
        case 0x36:
            kprint("Rshift");
            break;
        case 0x37:
            kprint("Keypad *");
            break;
        case 0x38:
            kprint("LAlt");
            break;
        case 0x39:
            kprint(" ");
            break;
        default: ;
            /* 'keuyp' event corresponds to the 'keydown' + 0x80 
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                char buffer[20] = "Unknown Key: ";
                buffer[13] = '0';
                buffer[14] = 'x';
                buffer[15] = to_hex_char((scancode >> 4) & 0x0F);
                buffer[16] = to_hex_char(scancode & 0x0F);
                buffer[17] = '\0';
                kprint(buffer);
                // kprint("Unknown key down");
            } /* else if (scancode <= 0x39 + 0x80) {
                // kprint("key up ");
                // print_letter(scancode - 0x80);
            } else kprint("Unknown key up"); */
        break;
    }
}