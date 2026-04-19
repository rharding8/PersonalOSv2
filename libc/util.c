#include "util.h"
#include "./../libc/global_macros.h"


void memcpy_pos(char * source, char * dest, int bytes){
    int i;
    for(i=0;i<bytes;i++) *(dest + i) = *(source + i);
    return;
}

void memset_pos(uchar_8* dest, uchar_8 value, uint_32 length){
    uchar_8 * temp = (uchar_8*)dest;
    for(;length!=0;length--)*(temp++) = value;

}
int strlen(char s[]){
    int i =0;
    while (s[i]!='\0')i++;
    return i;    
}

void reverse(char s[]){
    int ch, i,j;
    for(i=0,j=strlen(s)-1;i<j;i++,j--){
        ch = s[i];
        s[i]=s[j];
        s[j]=ch;
    }
}

void int_to_ascii(int n, char str[]){
    int i = 0, sign = FALSE;
    if(n<0){
        sign = TRUE;
        n=-n;
    }
    do{
        str[i++] = n%10 + '0';
    }while((n/=10)>0);

    if(sign)str[i++]='-';
    str[i]='\0';
    reverse(str);
}

void ascii_to_int(char str[], int* n){
    int i = 0, sign = FALSE;
    if(str[0]=='-'){
        sign = TRUE;
        i++;
    }
    *n=0;
    
    for(;str[i]!='\0' && str[i] >= '0' && str[i] <= '9';i++){
        *n = *n*10 + (str[i]-'0');
    }
    if(sign)*n=-*n;
}

int strcmp(char* a, char* b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

int strncmp(char* a, char* b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) return a[i] - b[i];
        if (a[i] == 0) return 0;
    }
    return 0;
}

void strcpy(char* dest, char* src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}