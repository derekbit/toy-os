#include <stdarg.h>
#include <stddef.h>
#include "uart.h"


int vsnprintf(char * out, size_t n, const char* s, va_list vl)
{
    int format = 0;
    int longarg = 0;
    int bigx = 0;
    size_t pos = 0;
    for( ; *s; s++) {
        if (format) {
            switch(*s) {
            case 'l': {
                longarg = 1;
                break;
            }
            case 'p': {
                longarg = 1;
                if (out && pos < n) {
                    out[pos] = '0';
                }
                pos++;
                if (out && pos < n) {
                    out[pos] = 'x';
                }
                pos++;
            }
            case 'X':
                bigx = 1;
            case 'x': {
                long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
                int hexdigits = 2*(longarg ? sizeof(long) : sizeof(int))-1;
                for(int i = hexdigits; i >= 0; i--) {
                    int d = (num >> (4*i)) & 0xF;
                    if (out && pos < n) {
                        out[pos] = (d < 10 ? '0'+d : (bigx ? 'A' : 'a')+d-10);
                    }
                    pos++;
                }
                longarg = 0;
                format = 0;
                bigx = 0;
                break;
            }
            case 'd': {
                long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
                if (num < 0) {
                    num = -num;
                    if (out && pos < n) {
                        out[pos] = '-';
                    }
                    pos++;
                }
                long digits = 1;
                for (long nn = num; nn /= 10; digits++)
                    ;
                for (int i = digits-1; i >= 0; i--) {
                    if (out && pos + i < n) {
                        out[pos + i] = '0' + (num % 10);
                    }
                    num /= 10;
                }
                pos += digits;
                longarg = 0;
                format = 0;
                break;
            }
            case 'u': {
                long num = longarg ? va_arg(vl, unsigned long) : va_arg(vl, unsigned int);
                long digits = 1;
                for (long nn = num; nn /= 10; digits++)
                    ;
                for (int i = digits-1; i >= 0; i--) {
                    if (out && pos + i < n) {
                        out[pos + i] = '0' + (num % 10);
                    }
                    num /= 10;
                }
                pos += digits;
                longarg = 0;
                format = 0;
                break;
            }
            case 's': {
                const char* s2 = va_arg(vl, const char*);
                while (*s2) {
                    if (out && pos < n) {
                        out[pos] = *s2;
                    }
                    pos++;
                    s2++;
                }
                longarg = 0;
                format = 0;
                break;
            }
            case 'c': {
                if (out && pos < n) {
                    out[pos] = (char)va_arg(vl,int);
                }
                pos++;
                longarg = 0;
                format = 0;
                break;
            }
            default:
                break;
            }
        }
        else if(*s == '%') {
          format = 1;
        }
        else {
          if (out && pos < n) {
            out[pos] = *s;
          }
          pos++;
        }
    }
    if (out && pos < n) {
        out[pos] = 0;
    }
    else if (out && n) {
        out[n-1] = 0;
    }
    return pos;
}

int vprintf(const char* s, va_list vl)
{
    // TODO: should allocate according to the length of the string
    char out[4096];
    int ret;
    
    ret = vsnprintf(NULL, -1, s, vl);
    vsnprintf(out, ret + 1, s, vl);
    uart_puts(out);
    return ret;
}

int printk(const char* s, ...)
{
    int ret = 0;
    va_list vl;

    va_start(vl, s);
    ret = vprintf(s, vl);
    va_end(vl);

    return ret;
}