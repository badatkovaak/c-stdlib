#ifndef VECTOR_H
#define VECTOR_H

#define Vec_Decl(type)                                                         \
    typedef struct Vector_##type {                                             \
        type *data;                                                            \
        unsigned long len;                                                     \
        unsigned long capacity;                                                \
    } Vec_##type;                                                              \
                                                                               \
    unsigned long closest_pow_2_##type(unsigned long num);                     \
    Vec_##type create_with_capacity_##type(unsigned long capacity);            \
    Vec_##type create_from_array_##type(type *array, unsigned long len);       \
    Vec_##type *create_with_capacity_into_##type(unsigned long capacity,       \
                                                 void *dest);                  \
    Vec_##type *create_from_array_into_##type(type *array, unsigned long len,  \
                                              void *dest);                     \
    void push_##type(Vec_##type *v, type value);                               \
    int insert_##type(Vec_##type *v, type value, unsigned long index);         \
    type pop_##type(Vec_##type *v);                                            \
    type remove_##type(Vec_##type *v, unsigned long index);                    \
    type get_##type(Vec_##type *v, unsigned long index);                       \
    void delete_vec_##type(Vec_##type *v);

#include <stdlib.h>
#include <string.h>

#define Vec_Impl(type)                                                         \
    unsigned long closest_pow_2_##type(unsigned long num) {                    \
        unsigned long res = 8;                                                 \
                                                                               \
        while (res < num) {                                                    \
            res *= 2;                                                          \
        }                                                                      \
                                                                               \
        return res;                                                            \
    }                                                                          \
                                                                               \
    Vec_##type create_with_capacity_##type(unsigned long capacity) {           \
        unsigned long temp = closest_pow_2_##type(capacity);                   \
        void *data = malloc(temp * sizeof(type));                              \
        memset(data, 0, temp * sizeof(type));                                  \
        return (Vec_##type){(type *)data, 0, temp};                            \
    }                                                                          \
                                                                               \
    Vec_##type create_from_array_##type(type *array, unsigned long len) {      \
        unsigned long temp = closest_pow_2_##type(len);                        \
        type *data = malloc(temp * sizeof(type));                              \
                                                                               \
        for (unsigned long i = 0; i < len; i++) {                              \
            data[i] = array[i];                                                \
        }                                                                      \
                                                                               \
        return (Vec_##type){data, len, temp};                                  \
    }                                                                          \
                                                                               \
    Vec_##type *create_with_capacity_into_##type(unsigned long capacity,       \
                                                 void *dest) {                 \
        unsigned long temp = closest_pow_2_##type(capacity);                   \
        Vec_##type *v = (Vec_##type *)dest;                                    \
        v->data = malloc(temp * sizeof(type));                                 \
        memset(v->data, 0, temp * sizeof(type));                               \
        v->len = 0;                                                            \
        v->capacity = temp;                                                    \
        return v;                                                              \
    }                                                                          \
                                                                               \
    Vec_##type *create_from_array_into_##type(type *array, unsigned long len,  \
                                              void *dest) {                    \
        unsigned long temp = closest_pow_2_##type(len);                        \
        Vec_##type *v = (Vec_##type *)dest;                                    \
        v->data = malloc(temp * sizeof(type));                                 \
        v->len = len;                                                          \
        v->capacity = len;                                                     \
                                                                               \
        for (unsigned long i = 0; i < len; i++) {                              \
            v->data[i] = array[i];                                             \
        }                                                                      \
                                                                               \
        return v;                                                              \
    }                                                                          \
                                                                               \
    void push_##type(Vec_##type *v, type value) {                              \
        if (!v->data) {                                                        \
            v->data = (type *)malloc(8 * sizeof(type));                        \
            v->capacity = 8;                                                   \
            v->data[0] = value;                                                \
            v->len = 1;                                                        \
            return;                                                            \
        }                                                                      \
                                                                               \
        if (v->capacity > v->len) {                                            \
            v->data[v->len] = value;                                           \
            v->len += 1;                                                       \
            return;                                                            \
        }                                                                      \
                                                                               \
        void *old_data = v->data;                                              \
        v->data = (type *)malloc(2 * v->len * sizeof(type));                   \
        memcpy(v->data, old_data, v->len * sizeof(type));                      \
        free(old_data);                                                        \
        v->capacity = v->len * 2;                                              \
                                                                               \
        memset((void *)((unsigned long)v->data + v->len * sizeof(type)), 0,    \
               v->len);                                                        \
        v->data[v->len] = value;                                               \
        v->len += 1;                                                           \
                                                                               \
        return;                                                                \
    }                                                                          \
                                                                               \
    int insert_##type(Vec_##type *v, type value, unsigned long index) {        \
        if (index > v->len) {                                                  \
            return -1;                                                         \
        }                                                                      \
                                                                               \
        if (!v->data && index) {                                               \
            return -1;                                                         \
        }                                                                      \
                                                                               \
        if (!v->data) {                                                        \
            v->data = (type *)malloc(8 * sizeof(type));                        \
            v->capacity = 8;                                                   \
            v->data[0] = value;                                                \
            v->len = 1;                                                        \
            return 0;                                                          \
        }                                                                      \
                                                                               \
        if (v->capacity - v->len == 0) {                                       \
            void *old_data = v->data;                                          \
            v->data = (type *)malloc(2 * v->len * sizeof(type));               \
            memcpy(v->data, old_data, v->len * sizeof(type));                  \
            free(old_data);                                                    \
            v->capacity = v->len * 2;                                          \
        }                                                                      \
                                                                               \
        void *src = (void *)((unsigned long)v->data + index * sizeof(type));   \
        void *dest = (void *)((unsigned long)src + sizeof(type));              \
        memmove(dest, src, (v->len - index) * sizeof(type));                   \
        v->data[index] = value;                                                \
        v->len += 1;                                                           \
        return 0;                                                              \
    }                                                                          \
                                                                               \
    type pop_##type(Vec_##type *v) {                                           \
        if (v->len == 0) {                                                     \
            __builtin_trap();                                                  \
        }                                                                      \
                                                                               \
        type res = v->data[v->len - 1];                                        \
        v->len -= 1;                                                           \
                                                                               \
        if (v->len == 0) {                                                     \
            v->data = (void *)0;                                               \
        }                                                                      \
                                                                               \
        return res;                                                            \
    }                                                                          \
                                                                               \
    type remove_##type(Vec_##type *v, unsigned long index) {                   \
        if (index <= 0 || index >= v->len) {                                   \
            __builtin_trap();                                                  \
        }                                                                      \
                                                                               \
        type res = v->data[index];                                             \
        void *src =                                                            \
            (void *)((unsigned long)v->data + (index + 1) * sizeof(type));     \
        memmove((void *)((unsigned long)src - sizeof(type)), src,              \
                v->len - index);                                               \
        v->len -= 1;                                                           \
        return res;                                                            \
    }                                                                          \
                                                                               \
    type get_##type(Vec_##type *v, unsigned long index) {                      \
        if (index >= v->len) {                                                 \
            __builtin_trap();                                                  \
        }                                                                      \
                                                                               \
        return v->data[index];                                                 \
    }                                                                          \
                                                                               \
    void delete_vec_##type(Vec_##type *v) { free(v->data); }

#define Vec_to_Box(vec_type, box_type)                                         \
    Box_##box_type vec_##vec_type_to_box_##box_type(Vec_##vec_type *vector) {  \
        return (Box_##box_type){vector.data, vector.len};                      \
    }

#endif // !VECTOR_H
