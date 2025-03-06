#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <g2d.h>

#define SRC_WIDTH  640   // Original width
#define SRC_HEIGHT 480   // Original height
#define DST_WIDTH  1920  // Scaled width
#define DST_HEIGHT 1080  // Scaled height

int main() {
    void *g2dHandle;
    struct g2d_surface src, dst;
    struct g2d_buf *srcBuf, *dstBuf;

    // Open G2D device
    if (g2d_open(&g2dHandle) < 0) {
        printf("Failed to open G2D device\n");
        return -1;
    }

    // Allocate memory for source and destination buffers
    srcBuf = g2d_alloc(SRC_WIDTH * SRC_HEIGHT * 4, 0);
    dstBuf = g2d_alloc(DST_WIDTH * DST_HEIGHT * 4, 0);

    if (!srcBuf || !dstBuf) {
        printf("Failed to allocate buffers\n");
        return -1;
    }

    // Fill source buffer with dummy data (optional)
    memset(srcBuf->buf_vaddr, 0xFF, SRC_WIDTH * SRC_HEIGHT * 4);  // White color

    // Configure source surface
    src.format = G2D_RGBA8888;
    src.planes[0] = srcBuf->buf_paddr;
    src.left = 0;
    src.top = 0;
    src.right = SRC_WIDTH;
    src.bottom = SRC_HEIGHT;
    src.stride = SRC_WIDTH;
    src.width = SRC_WIDTH;
    src.height = SRC_HEIGHT;
    src.rot = G2D_ROTATION_0;

    // Configure destination surface
    dst.format = G2D_RGBA8888;
    dst.planes[0] = dstBuf->buf_paddr;
    dst.left = 0;
    dst.top = 0;
    dst.right = DST_WIDTH;
    dst.bottom = DST_HEIGHT;
    dst.stride = DST_WIDTH;
    dst.width = DST_WIDTH;
    dst.height = DST_HEIGHT;
    dst.rot = G2D_ROTATION_0;

    // Perform scaling
    g2d_blit(g2dHandle, &src, &dst);
    g2d_finish(g2dHandle);  // Ensure operation is completed

    printf("Scaling completed successfully!\n");

    // Free buffers and close G2D device
    g2d_free(srcBuf);
    g2d_free(dstBuf);
    g2d_close(g2dHandle);

    return 0;
}

