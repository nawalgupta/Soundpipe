#include <stdlib.h>
#include "soundpipe.h"
#include "dr_wav.h"

#define WAVIN_BUFSIZE 1024

struct sp_wavin {
    SPFLOAT buf[WAVIN_BUFSIZE];
    int count;
    drwav wav;
    unsigned long pos;
};

int sp_wavin_create(sp_wavin **p)
{
    *p = malloc(sizeof(sp_wavin));
    return SP_OK;
}

int sp_wavin_destroy(sp_wavin **p)
{
    drwav_uninit(&(*p)->wav);
    free(*p);
    return SP_OK;
}

int sp_wavin_init(sp_data *sp, sp_wavin *p, const char *filename)
{
    p->count = 0;
    p->pos = 0;
    drwav_init_file(&p->wav, filename);
    return SP_OK;
}

int sp_wavin_compute(sp_data *sp, sp_wavin *p, SPFLOAT *in, SPFLOAT *out)
{
    if(p->pos > p->wav.totalSampleCount) {
        *out = 0;
        return SP_OK;
    }
    if(p->count == 0) {
        drwav_read_f32(&p->wav, WAVIN_BUFSIZE, p->buf);
    }

    *out = p->buf[p->count];
    p->count = (p->count + 1) % WAVIN_BUFSIZE;
    p->pos++;
    return SP_OK;
}

int sp_wavin_seek(sp_data *sp, sp_wavin *p, unsigned long sample)
{
    drwav_seek_to_sample(&p->wav, sample);
    return SP_OK;
}
