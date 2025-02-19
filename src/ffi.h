#ifndef FFI_H
#define FFI_H

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#if defined(__clang__) || defined(__GNUC__)
__attribute__((__format__ (__printf__, 1, 2)))
void log_error(const char* fmt, ...);
__attribute__((__format__ (__printf__, 1, 2)))
void log_debug(const char* fmt, ...);
__attribute__((__format__ (__printf__, 1, 2)))
void log_info(const char* fmt, ...);
__attribute__((__format__ (__printf__, 1, 2)))
void log_trace(const char* fmt, ...);
__attribute__((__format__ (__printf__, 1, 2)))
void log_warn(const char* fmt, ...);
#else
void log_error(const char* fmt, ...);
void log_debug(const char* fmt, ...);
void log_info(const char* fmt, ...);
void log_trace(const char* fmt, ...);
void log_warn(const char* fmt, ...);
#endif

#define AV_NUM_DATA_POINTERS 8

#define AV_LOG_QUIET -8
#define AV_LOG_PANIC 0
#define AV_LOG_FATAL 8
#define AV_LOG_ERROR 16
#define AV_LOG_WARNING 24
#define AV_LOG_INFO 32
#define AV_LOG_VERBOSE 40
#define AV_LOG_DEBUG 48
#define AV_LOG_TRACE 56

enum AVPixelFormat {
  AV_PIX_FMT_YUV420P = 0,
  AV_PIX_FMT_NV12 = 23,
  AV_PIX_FMT_VAAPI = 44,
  AV_PIX_FMT_VDPAU = 98,
  AV_PIX_FMT_QSV = 114,
  AV_PIX_FMT_D3D11VA_VLD = 116,
  AV_PIX_FMT_CUDA = 117,
  AV_PIX_FMT_VIDEOTOOLBOX = 158,
  AV_PIX_FMT_MEDIACODEC = 165,
  AV_PIX_FMT_D3D11 = 172,
  AV_PIX_FMT_OPENCL = 180,
  AV_PIX_FMT_VULKAN = 191,
};

enum Quality { Quality_Default, Quality_High, Quality_Medium, Quality_Low };

enum RateControl {
  RC_DEFAULT,
  RC_CBR,
  RC_VBR,
};

typedef void (*DecodeCallback)(const void *obj, int width, int height,
                               int pixfmt, int linesize[AV_NUM_DATA_POINTERS],
                               uint8_t *data[AV_NUM_DATA_POINTERS], int key);
typedef void (*EncodeCallback)(const uint8_t *data, int len, int64_t pts,int key,
                               const void *obj);

void *new_encoder(const char *name, int width, int height, int pixfmt,
                  int align, int bit_rate, int time_base_num, int time_base_den,
                  int gop, int quality, int rc, int *linesize, int *offset,
                  int *length, EncodeCallback callback);
void *new_decoder(const char *name, int device_type, int output_surface, DecodeCallback callback);
void *new_muxer(const char *filename, int width, int height, int is265,
                int framerate);
int encode(void *encoder, const uint8_t *data, int length, const void *obj,
           int64_t ms);
int decode(void *decoder, const uint8_t *data, int length, const void *obj);
int write_video_frame(void *muxer, const uint8_t *data, int len,
                      int64_t pts_ms, int key);
int write_tail(void *muxer);
void free_encoder(void *encoder);
void free_decoder(void *decoder);
void free_muxer(void *muxer);
int get_linesize_offset_length(int pix_fmt, int width, int height, int align,
                               int *linesize, int *offset, int *length);
int set_bitrate(void *encoder, int bitrate);
int av_log_get_level(void);
void av_log_set_level(int level);
void get_bin_file(int is265, uint8_t **p, int *len);

void init_ffmpeg_logger_();

#endif  // FFI_H
