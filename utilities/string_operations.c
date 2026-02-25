/**
 * @file string_operations.c
 * @brief Functions for manipulating strings.
 *
 * This file contains functions to manipulate strings.
 */

/**
 * @brief Normalizes a string by replacing spaces and periods with underscores.
 *
 * This function iterates through each character in the input string. If it
 * encounters a space (' ') or a period ('.'), it replaces that character
 * with an underscore ('_'). The normalization is performed in place.
 *
 * @param str A pointer to the string to be normalized. The string must be NULL-terminated
 * and mutable (e.g., allocated with malloc or defined as a char array).
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
