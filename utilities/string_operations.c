/*
 * string_operations.c
 *
 *  Created on: Feb 23, 2026
 *      Author: abba
 */


void normalize_string(char *str) {
    while (*str) {
        if ((*str == ' ') ||
        		(*str == '.'))	{
            *str = '_';
        }
        str++;
    }
}
