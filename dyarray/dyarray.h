#ifndef _DYARRAY_H
#define _DYARRAY_H

#include <stdio.h>
#include <stdlib.h>

#define UseVector(T)                                                    \
    typedef struct {                                                    \
        int len;                                                        \
        int cap;                                                        \
        T* data;                                                        \
    } vector_##T;                                                       \
    vector_##T vector_##T##_new(void) {                                 \
        vector_##T vec;                                                 \
        vec.len = 0;                                                    \
        vec.cap = 0;                                                    \
        vec.data = NULL;                                                \
        return vec;                                                     \
    }                                                                   \
    void vector_##T##_free(vector_##T* vec) {                           \
        free(vec->data);                                                \
    }                                                                   \
    T vector_##T##_at(vector_##T* vec, int index) {                     \
        return vec->data[index];                                        \
    }                                                                   \
    void vector_##T##_push(vector_##T* vec, T value) {                  \
        if (NULL == vec->data) {                                        \
            vec->len = 1;                                               \
            vec->cap = 2;                                               \
            vec->data = (T*)malloc(sizeof(T) * 2);                      \
        } else if (vec->len + 1 <= vec->cap) {                          \
            vec->len += 1;                                              \
        } else {                                                        \
            vec->len += 1;                                              \
            vec->cap = vec->cap * 2;                                    \
            vec->data = (T*)realloc(vec->data, sizeof(T) * vec->cap);   \
        }                                                               \
        vec->data[vec->len - 1] = value;                                \
    }                                                                   \

#endif