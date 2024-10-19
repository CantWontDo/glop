//
// Created by Rahul Pardeshi on 10/11/2024.
//

#ifndef GLOP_H
#define GLOP_H
#include "typedefs.h"
#include "err.h"
#include "map.h"
#include "lib/glad/glad.h"

typedef struct buf
{
    u32 id;
    u32 type;
} buf;

buf buf_new(u32 type);
buf vbo_new();
buf ebo_new();

void buf_data(buf *b, u32 buf_size, void *data, u32 usage);
void buf_bind(buf *b);
void buf_del(buf *b);

typedef struct attrib
{
    u32 len;
    u32 type;
} attrib;

typedef struct vao
{
    u32 id;
    attrib *attribs;
    u32 n_attribs;
} vao;

vao vao_new(buf *vbo, buf *ebo, u32 n_attribs, ...);
void vao_del(vao *v);
void vao_bind(vao *v);

attrib* desc_attribs(u32 n_attribs, ...);

typedef struct unif
{
    u32 loc;
    u32 count;
} unif;

typedef struct shdr
{
    u32 id;
    unif *unifs;
    map m;
} shdr;

shdr shdr_new(u32 n_shdr, ...);
u32 shdr_compile(u32 type, char *path);

void shdr_bind(shdr *s);

void shdr_check(u32 id, char *path);
void prog_check(u32 id);

u32 shdr_get_loc(shdr *s, char *unif_name);

void shdr_m4f(shdr *s, char *unif_name, m4 *val);
void shdr_m3f(shdr *s, char *unif_name, m3 *m);
void shdr_m2f(shdr *s, char *unif_name, m2 *m);

void shdr_1f(shdr *s, char *unif_name, const float *m);
void shdr_2f(shdr *s, char *unif_name, v2 *m);
void shdr_3f(shdr *s, char *unif_name, v3 *m);
void shdr_4f(shdr *s, char *unif_name, v4 *m);

void shdr_1i(shdr *s, char *unif_name, const int *m);
#endif //GLOP_H
