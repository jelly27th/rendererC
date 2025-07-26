#include <stdio.h>
#include "vector/vector.h"

UseVector(char)

int main() {

    vector_char vchar = vector_char_new();
    vector_char_push(&vchar, 'a');
    vector_char_push(&vchar, 'b');
    vector_char_push(&vchar, 'c');
    vector_char_push(&vchar, 'd');
    vector_char_push(&vchar, 'e');
    vector_char_push(&vchar, 'f');
    vector_char_push(&vchar, 'g');
    vector_char_push(&vchar, 'h');
    vector_char_push(&vchar, 'i');
    printf("%c\n", vector_char_at(&vchar, 0));
    printf("%c\n", vector_char_at(&vchar, 1));
    printf("%c\n", vector_char_at(&vchar, 2));
    printf("%c\n", vector_char_at(&vchar, 3));
    printf("%c\n", vector_char_at(&vchar, 4));
    printf("%c\n", vector_char_at(&vchar, 5));
    printf("%c\n", vector_char_at(&vchar, 6));
    return 0;
}