//
// Created by Rahul Pardeshi on 10/11/2024.
//

#ifndef GLOP_H
#define GLOP_H
#include "typedefs.h"
#include "err.h"
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
    u32 size;
} unif;

typedef struct shdr
{
    u32 id;
    unif *unifs;
} shdr;

shdr shdr_new(u32 n_shdr, ...);
u32 shdr_compile(u32 type, char *path);

void shdr_bind(shdr *s);

void shdr_check(u32 id, char *path);
void prog_check(u32 id);

#endif //GLOP_H
