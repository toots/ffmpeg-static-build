#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void require_decoder(enum AVCodecID id, const char *name)
{
    if (!avcodec_find_decoder(id)) {
        fprintf(stderr, "FAIL: decoder not found: %s\n", name);
        exit(1);
    }
    printf("OK decoder:  %s\n", name);
}

static void require_decoder_by_name(const char *name)
{
    if (!avcodec_find_decoder_by_name(name)) {
        fprintf(stderr, "FAIL: decoder not found: %s\n", name);
        exit(1);
    }
    printf("OK decoder:  %s\n", name);
}

static void require_encoder(enum AVCodecID id, const char *name)
{
    if (!avcodec_find_encoder(id)) {
        fprintf(stderr, "FAIL: encoder not found: %s\n", name);
        exit(1);
    }
    printf("OK encoder:  %s\n", name);
}

static void require_encoder_by_name(const char *name)
{
    if (!avcodec_find_encoder_by_name(name)) {
        fprintf(stderr, "FAIL: encoder not found: %s\n", name);
        exit(1);
    }
    printf("OK encoder:  %s\n", name);
}

static void require_demuxer(const char *name)
{
    if (!av_find_input_format(name)) {
        fprintf(stderr, "FAIL: demuxer not found: %s\n", name);
        exit(1);
    }
    printf("OK demuxer:  %s\n", name);
}

static void require_muxer(const char *name)
{
    if (!av_guess_format(name, NULL, NULL)) {
        fprintf(stderr, "FAIL: muxer not found: %s\n", name);
        exit(1);
    }
    printf("OK muxer:    %s\n", name);
}

static void require_protocol(const char *name)
{
    void *opaque = NULL;
    const char *proto;
    while ((proto = avio_enum_protocols(&opaque, 0)) != NULL) {
        if (strcmp(proto, name) == 0) {
            printf("OK protocol: %s\n", name);
            return;
        }
    }
    fprintf(stderr, "FAIL: protocol not found: %s\n", name);
    exit(1);
}

int main(void)
{
    printf("FFmpeg version: %s\n\n", av_version_info());

    /* decoders */
    require_decoder(AV_CODEC_ID_AAC,       "aac");
    require_decoder_by_name("libfdk_aac");
    require_decoder(AV_CODEC_ID_FLAC,      "flac");
    require_decoder(AV_CODEC_ID_MP2,       "mp2");
    require_decoder(AV_CODEC_ID_MP3,       "mp3");
    require_decoder(AV_CODEC_ID_VORBIS,    "vorbis");
    require_decoder(AV_CODEC_ID_OPUS,      "opus");
    require_decoder_by_name("libopus");
    require_decoder(AV_CODEC_ID_PCM_S16LE, "pcm_s16le");
    require_decoder(AV_CODEC_ID_PCM_S24LE, "pcm_s24le");
    require_decoder(AV_CODEC_ID_PCM_S32LE, "pcm_s32le");

    /* encoders */
    require_encoder(AV_CODEC_ID_AAC,       "aac");
    require_encoder_by_name("libfdk_aac");
    require_encoder_by_name("libmp3lame");
    require_encoder(AV_CODEC_ID_FLAC,      "flac");
    require_encoder(AV_CODEC_ID_MP2,       "mp2");
    require_encoder_by_name("libvorbis");
    require_encoder_by_name("libopus");
    require_encoder(AV_CODEC_ID_PCM_S16LE, "pcm_s16le");
    require_encoder(AV_CODEC_ID_PCM_S24LE, "pcm_s24le");
    require_encoder(AV_CODEC_ID_PCM_S32LE, "pcm_s32le");

    /* demuxers */
    require_demuxer("mpegts");
    require_demuxer("matroska");
    require_demuxer("ogg");
    require_demuxer("flac");
    require_demuxer("aac");

    /* muxers */
    require_muxer("mpegts");
    require_muxer("matroska");
    require_muxer("ogg");
    require_muxer("flac");
    require_muxer("adts");

    /* protocols */
    require_protocol("srt");
    require_protocol("file");

    printf("\nAll checks passed.\n");
    return 0;
}
