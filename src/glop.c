//
// Created by iamro on 10/11/2024.
//

#include "glop.h"

#include <stdlib.h>

buf buf_new(u32 type)
{
    buf b = {.id = 0, .type = type};
    glCreateBuffers(1, &b.id);
    return b;
}

buf vbo_new()
{
    return buf_new(GL_ARRAY_BUFFER);
}

buf ebo_new()
{
    return buf_new(GL_ELEMENT_ARRAY_BUFFER);
}

void buf_data(buf *b, u32 buf_size, void *data, u32 usage)
{
    glNamedBufferStorage(b->id, buf_size, data, usage);
}
void buf_bind(buf *b)
{
    glBindBuffer(b->type, b->id);
}
void buf_del(buf *b)
{
    glDeleteBuffers(1, &b->id);
}


vao vao_new(buf *vbo, buf *ebo, u32 n_attribs, ...)
{
    u32 id;
    glCreateVertexArrays(1, &id);

    attrib *attribs = malloc(sizeof(attrib) * n_attribs);

    va_list attrib_d;

    va_start(attrib_d, n_attribs * 2);

    for (int i = 0; i < n_attribs; i++)
    {
        attrib *a = &attribs[i];
        a->len = va_arg(attrib_d, u32);
        a->type = va_arg(attrib_d, u32);
    }

    va_end(attrib_d);

    u32 stride = 0;
    for (int i = 0; i < n_attribs; i++)
    {
        attrib a = attribs[i];
        u32 size = a.type == GL_FLOAT ? sizeof (float) : sizeof (int);
        stride += a.len * size;
    }

    vao v;
    v.id = id;
    v.n_attribs = n_attribs;
    v.attribs = attribs;

    glVertexArrayVertexBuffer(v.id, 0, vbo->id, 0, stride);

    if (ebo)
        glVertexArrayElementBuffer(v.id, ebo->id);

    for (int i = 0; i < n_attribs; i++)
        glEnableVertexArrayAttrib(v.id, i);

    u32 rel_offset = 0;
    for (int i = 0; i < n_attribs; i++)
    {
        attrib a = attribs[i];
        glVertexArrayAttribFormat(v.id, i, a.len, a.type, GL_FALSE, rel_offset);

        u32 size = a.type == GL_FLOAT ? sizeof (float) : sizeof (int);
        rel_offset += a.len * size;

        glVertexArrayAttribBinding(v.id, i, 0);
    }

    return v;
}

void vao_del(vao *v)
{
    glDeleteVertexArrays(1, &v->id);
}

void vao_bind(vao *v)
{
    glBindVertexArray(v->id);
}

shdr shdr_new(u32 n_shdr, ...)
{
    u32 id = glCreateProgram();
    va_list shdrs;

    va_start(shdrs, n_shdr);
    for (int i = 0; i < n_shdr; i++)
    {
        u32 type = va_arg(shdrs, u32);
        char *path = va_arg(shdrs, char *);

        u32 shdr = shdr_compile(type, path);
        glAttachShader(id, shdr);
        glDeleteShader(shdr);
    }

    glLinkProgram(id);
    prog_check(id);

    u32 unif_count = 0;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &unif_count);
    unif *unifs = 0;
    map m;

    if (unif_count != 0)
    {
        u32 max_name_len = 0;
        u32 len = 0;
        u32 count = 0;
        u32 type = 0;
        glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

        unifs = arr_new_size(unif, unif_count);
        m = map_new(unif_count);

        char *name = malloc(sizeof(char) * max_name_len);

        for (int i = 0; i < unif_count; i++)
        {
            glGetActiveUniform(id, i, max_name_len, &len, &count, &type, name);

            unif u = (unif){.count = count, .loc = glGetUniformLocation(id, name)};

            map_add(&m, name, &unifs, &u);
        }
    }
    shdr s = {.id = id, .unifs = unifs, .m = m};
    return s;
}

u32 shdr_compile(u32 type, char *path)
{
    u32 id = glCreateShader(type);

    char *src = read_txt_file(path);
    if (!src)
        return -1;

    glShaderSource(id, 1, &src, 0);
    glCompileShader(id);

    shdr_check(id, path);
    free(src);

    return id;
}

void shdr_bind(shdr *s)
{
    glUseProgram(s->id);
}

void shdr_check(u32 id, char *path)
{
    i32 success = -1;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char info_log[2048];
        glGetShaderInfoLog(id, sizeof(info_log), 0, info_log);
        log_err("failed to compile shader at %s:\n%s\n", path, info_log);
    }
}

void prog_check(u32 id)
{
    i32 success = -1;
    glGetProgramiv(id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char info_log[2048];
        glGetProgramInfoLog(id, sizeof(info_log), 0, info_log);
        log_err("failed to link program:\n%s\n", info_log);
    }

}

u32 shdr_get_loc(shdr *s, char *unif_name)
{
    unif u = s->unifs[map_lookup(&s->m, unif_name)];
    u32 loc = u.loc;
    if (loc == -1)
        log_err("uniform %s does not exist in shader!", unif_name);

    return loc;
}

void shdr_m4f(shdr *s, char *unif_name, m4 *val)
{
    int loc = shdr_get_loc(s, unif_name);
    glProgramUniformMatrix4fv(s->id, loc, 1, GL_TRUE, &val->m[0]);
}
void shdr_m3f(shdr *s, char *unif_name, m3 *val)
{
    int loc = shdr_get_loc(s, unif_name);
    glProgramUniformMatrix3fv(s->id, loc, 1, GL_TRUE, &val->m[0]);
}
void shdr_m2f(shdr *s, char *unif_name, m2 *val)
{
    int loc = shdr_get_loc(s, unif_name);
    glProgramUniformMatrix2fv(s->id, loc, 1, GL_TRUE, &val->m[0]);
}

void shdr_1f(shdr *s, char *unif_name, const float *val)
{
    int loc = shdr_get_loc(s, unif_name);
    glProgramUniform1fv(s->id, loc, 1, val);
}

void shdr_2f(shdr *s, char *unif_name, v2 *val)

{
    int loc = shdr_get_loc(s, unif_name);
    glProgramUniformMatrix2fv(s->id, loc, 1, GL_TRUE, &val->v[0]);
}
void shdr_3f(shdr *s, char *unif_name, v3 *val)
{
    int loc = shdr_get_loc(s, unif_name);
    glProgramUniformMatrix3fv(s->id, loc, 1, GL_TRUE, &val->v[0]);
}
void shdr_4f(shdr *s, char *unif_name, v4 *val)
{
    int loc = shdr_get_loc(s, unif_name);
    glProgramUniformMatrix2fv(s->id, loc, 1, GL_TRUE, &val->v[0]);
}

void shdr_1i(shdr *s, char *unif_name, const int *val)
{
    int loc = shdr_get_loc(s, unif_name);
    glProgramUniform1iv(s->id, loc, 1, val);
}
