#include "screen.h"
#include "./../cpu/vga_port.h"
#include "./../cpu/cpu_ports.h"
#include "./../libc/util.h"
#include "./../libc/global_macros.h"


int END_OFFSET = (2*VGA_MAX_COLS*VGA_MAX_ROWS)-1;

/************************************************ 
 * Private functions of the driver
 * Function declaration excluded from header file
 * **********************************************/

int get_offset(int col, int row){return 2*( row*VGA_MAX_COLS + col ); }
int get_row_from_offset(int offset){ return (int)(offset/ (2 * VGA_MAX_COLS)) ;}
int get_col_from_offset(int offset){ return (offset/2 - get_row_from_offset(offset)*VGA_MAX_COLS ); }

int get_cursor_offset(){
    port_byte_out(REG_VGA_COTRL, OFFSET_HIGH_BYTE);
    int offset= port_byte_in(REG_VGA_DATA) << BYTE_LENGTH;
    port_byte_out(REG_VGA_COTRL,OFFSET_LOW_GYTE);
    offset += port_byte_in(REG_VGA_DATA);
    return offset*2;
}

void set_cursor_offset(int offset){
    offset /= 2;
    port_byte_out(REG_VGA_COTRL, OFFSET_HIGH_BYTE);
    port_byte_out(REG_VGA_DATA,(unsigned char)(offset >> BYTE_LENGTH));
    port_byte_out(REG_VGA_COTRL,OFFSET_LOW_GYTE);
    port_byte_out(REG_VGA_DATA,(unsigned char)(offset & 0xff));
}

int print_error(int bottom){
    unsigned char *vidmem = ( unsigned char *) VGA_START_ADDR;
    if(!bottom){
        // Print ERROR at top left position
        vidmem[0]='E';
        vidmem[1]=RED_ON_BLACK;
        vidmem[2]='R';
        vidmem[3]=RED_ON_BLACK;
        vidmem[4]='R';
        vidmem[5]=RED_ON_BLACK;
        vidmem[6]='O';
        vidmem[7]=RED_ON_BLACK;
        vidmem[8]='R';
        vidmem[9]=RED_ON_BLACK;

    }else{
        // Print ERROR at bottom right position
        vidmem[END_OFFSET-9]='E';
        vidmem[END_OFFSET-8]=RED_ON_BLACK;
        vidmem[END_OFFSET-7]='R';
        vidmem[END_OFFSET-6]=RED_ON_BLACK;
        vidmem[END_OFFSET-5]='R';
        vidmem[END_OFFSET-4]=RED_ON_BLACK;
        vidmem[END_OFFSET-3]='O';
        vidmem[END_OFFSET-2]=RED_ON_BLACK;
        vidmem[END_OFFSET-1]='R';
        vidmem[END_OFFSET]=RED_ON_BLACK;
    }
    

}

int scroll_screen(int offset){
    // scrool the screen one line up.
    // Row 0 will be deleted
    int i;
    for(i=1; i<VGA_MAX_ROWS;i++){
        memcpy_pos((char *)(get_offset(0,i)+VGA_START_ADDR), (char *) (get_offset(0,i-1)+VGA_START_ADDR), VGA_MAX_COLS*2);
    }
    //Erase last line
    char *last_line =(char *) (get_offset(0,VGA_MAX_ROWS - 1) + VGA_START_ADDR );
    for(i=0;i< VGA_MAX_COLS*2;i++) last_line[i]=0;
    offset -= 2 * VGA_MAX_COLS; // Setting offset at the beginning of last line
    return offset;
        
}




int print_char( char c, int col, int row, char attr){
    /**
     * directly access video memory to print a character
     * col and row indicate positino of printed character
     * if col and row negative, print at current cursor location
     * update cursor position for next character
     * attr(attribute) is related to color. zero means write on black as default
     * return value: offset for next character 
    **/
    unsigned char *vidmem = ( unsigned char *) VGA_START_ADDR;
    if(!attr) attr = WHITE_ON_BLACK;
    // Error control: print error
    if(col>=VGA_MAX_COLS || row >=VGA_MAX_ROWS){
        print_error(TRUE);
        return get_offset(col,row);
    }

    int offset;
    if(col>=0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if(c =='\n'){
        row = get_row_from_offset(offset);
        col = get_col_from_offset(offset);
    }else{
        vidmem[offset]= c;
        vidmem[offset+1] = attr;
        offset +=2;
    }
    // handle when offset crossed the screen boundry 
    if(offset >= VGA_MAX_COLS*VGA_MAX_ROWS*2){
        offset = scroll_screen(offset);
    }

    set_cursor_offset(offset);
    return offset;

}




/************************************************
 * Publuc functions (API) of the driver
 * Function declaration exist in header file
 * **********************************************/

void clear_screen(){
    int screen_size = VGA_MAX_COLS*VGA_MAX_ROWS;
    char *screen = (char *)VGA_START_ADDR;
    int i;
    for(i = 0;i<screen_size;i++){
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0,0));
}

void clear_one_character(){
    int offset = get_cursor_offset();
    int row = get_row_from_offset(offset);
    int col = get_col_from_offset(offset);
    if(col==0){
        if(row == 0) return;
        row -= 1;
        col = VGA_MAX_COLS -2;
    }else{
        col -= 1;
    }
    print_char(' ',col,row,BLACK_ON_BLACK);
    offset = get_cursor_offset();
    row = get_row_from_offset(offset);
    col = get_col_from_offset(offset);
    if(col==0){
        if(row == 0) return;
        row -= 1;
        col = VGA_MAX_COLS -2;
    }else{
        col -= 1;
    }

    set_cursor_offset(get_offset(col,row));
}

void kprint_at(char *data, int col, int row){
    /**
     * Will check value for col and row to set proper offset
     * iterate over all value in data and print it
     * TODO: may be add attribute set feature in near future
     * */
    int offset;
    if(col>=0 && row >= 0){
        offset = get_offset(col,row);
    }else{
        offset = get_cursor_offset();
        row = get_row_from_offset(offset);
        col = get_col_from_offset(offset);
    }

    int i=0;
    while(data[i] != 0){
        offset = print_char(data[i++],col, row, WHITE_ON_BLACK);
        row = get_row_from_offset(offset);
        col = get_col_from_offset(offset);
        
    }

}


void kprint(void *data){kprint_at(data,-1,-1);};

void kprint_char(char c) {
    char data[2];
    data[0] = c;
    data[1] = '\0';
    kprint(data);
}

void kline_break(){
    int offset = get_cursor_offset();
    int col = 0;
    int row = get_row_from_offset(offset) + 1;
    offset = get_offset(col,row);
    if(row  >= VGA_MAX_ROWS){
        offset = scroll_screen(offset);
    }
    set_cursor_offset(offset);
}
