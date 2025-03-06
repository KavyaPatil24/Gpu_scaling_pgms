#include <stdio.h>
#include <stdlib.h>
#include <g2d.h>

#define MAX_ITERATIONS 100
#define SRC_WIDTH 640
#define SRC_HEIGHT 480
#define DST_WIDTH 1024
#define DST_HEIGHT 768
#define PIXEL_SIZE 4  // Assuming 4 bytes per pixel (RGBA)

typedef struct {
    int width;
    int height;
    unsigned char* data;
} Resolution;

// Initialize source resolution with test data
Resolution initResolution() {
    Resolution res;
    res.width = SRC_WIDTH;
    res.height = SRC_HEIGHT;
    size_t dataSize = res.width * res.height * PIXEL_SIZE;
    res.data = (unsigned char*)malloc(dataSize);
    if (res.data == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    for (size_t i = 0; i < dataSize; i++) {
        res.data[i] = i % 256;
    }
    return res;
}

// Perform scaling using G2D API
void scaleResolutionG2D(Resolution* src, Resolution* dst, struct g2d_buf* srcBuf, struct g2d_buf* dstBuf, struct g2d_context* g2dHandle) {
    struct g2d_surface srcSurface, dstSurface;
    
    srcSurface.format = G2D_RGBA8888;
    srcSurface.planes[0] = (int)srcBuf->buf_paddr;
    srcSurface.left = 0;
    srcSurface.top = 0;
    srcSurface.right = src->width;
    srcSurface.bottom = src->height;
    srcSurface.stride = src->width * PIXEL_SIZE;
    
    dstSurface.format = G2D_RGBA8888;
    dstSurface.planes[0] = (int)dstBuf->buf_paddr;
    dstSurface.left = 0;
    dstSurface.top = 0;
    dstSurface.right = dst->width;
    dstSurface.bottom = dst->height;
    dstSurface.stride = dst->width * PIXEL_SIZE;
    
    g2d_blit(g2dHandle, &srcSurface, &dstSurface);
    g2d_finish(g2dHandle);
}

int main() {
    struct g2d_context *g2dHandle;
    g2d_open(&g2dHandle);

    Resolution srcRes = initResolution();
    Resolution dstRes = { DST_WIDTH, DST_HEIGHT, (unsigned char*)malloc(DST_WIDTH * DST_HEIGHT * PIXEL_SIZE) };
    
    struct g2d_buf *srcBuf = g2d_alloc(SRC_WIDTH * SRC_HEIGHT * PIXEL_SIZE, 0);
    struct g2d_buf *dstBuf = g2d_alloc(DST_WIDTH * DST_HEIGHT * PIXEL_SIZE, 0);
    memcpy(srcBuf->buf_vaddr, srcRes.data, SRC_WIDTH * SRC_HEIGHT * PIXEL_SIZE);
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        printf("Iteration %d\n", i + 1);
        scaleResolutionG2D(&srcRes, &dstRes, srcBuf, dstBuf, g2dHandle);
    }
    
    free(srcRes.data);
    free(dstRes.data);
    g2d_free(srcBuf);
    g2d_free(dstBuf);
    g2d_close(g2dHandle);
    return 0;
}

