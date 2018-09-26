//
// Created by nanda on 9/23/2018.
//
#include <jni.h>
#include <android/log.h>
#include <arm_neon.h>
#include <malloc.h>
#include "tiltshiftBlur_neon_lib.h"

//#pragma GCC push_options
//#pragma GCC target ("+nothing+simd")

//jfloat Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(jint pixel, jfloat gauss,
//                                                                            jint mul) {                                                 // Method to calculate transform of each color of an individual pixel
//    int G = (pixel >> (mul * 8)) & 0xff;
//    float ret = (G * gauss);
//    return ret;
//}

//__extension__ static __inline float32_t __attribute__ ((__always_inline__))
//vaddvq_f32 (float32x4_t __a)
//{
//    return __builtin_aarch64_reduc_plus_scal_v4sf (__a);
//}

jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(jint *pixels,
                                                                          jfloat *kernelMat,
                                                                          jint i, jint j,
                                                                          jint width,
                                                                          jint radius){

    int temp1 = (2*radius+1)+(4-(radius%4));
    __android_log_print(ANDROID_LOG_ERROR, "Matrix of Neon kernel!!!!!:", "%d", radius);
    for(int x = 0; x < temp1; x++){
        __android_log_print(ANDROID_LOG_ERROR," ", "%d     %lf",x, kernelMat[x]);
    }

    jfloat R = 0.0, G = 0.0, B = 0.0, A = 0.0;

    float32x4_t a_Vec = vdupq_n_f32(0);
    float32x4_t r_Vec = vdupq_n_f32(0);
    float32x4_t g_Vec = vdupq_n_f32(0);
    float32x4_t b_Vec = vdupq_n_f32(0);

//    if (j >= radius && j < width -
//                           radius) {
        for (int g_index = -radius; g_index <= radius; g_index = g_index + 4) {
            uint32x4_t aVec = vdupq_n_u8(0);
            uint32x4_t rVec = vdupq_n_u8(0);
            uint32x4_t gVec = vdupq_n_u8(0);
            uint32x4_t bVec = vdupq_n_u8(0);

            float32x4_t matVec = vld1q_f32(kernelMat);

            uint32x4_t pixelVec = vld1q_u32(&pixels[i * width + j + g_index]);

            aVec = (pixelVec >> (3 * 8)) & 0xff;
            rVec = (pixelVec >> (2 * 8)) & 0xff;
            gVec = (pixelVec >> (1 * 8)) & 0xff;
            bVec = (pixelVec >> (0 * 8)) & 0xff;

            a_Vec = vmlaq_f32(a_Vec,aVec,matVec);
            r_Vec = vmlaq_f32(r_Vec,rVec,matVec);
            g_Vec = vmlaq_f32(g_Vec,gVec,matVec);
            b_Vec = vmlaq_f32(b_Vec,bVec,matVec);

            kernelMat = kernelMat + 4;
        }

//        A = vaddvq_f32(a_Vec);
//        R = vaddvq_f32(r_Vec);
//        G = vaddvq_f32(g_Vec);
//        B = vaddvq_f32(b_Vec);
        float tempArray[4];
        vst1q_f32(tempArray,a_Vec);
        for(int i=0; i<4;i++){
            A += tempArray[i];
        }
        vst1q_f32(tempArray,r_Vec);
        for(int i=0; i<4;i++){
            R += tempArray[i];
        }
        vst1q_f32(tempArray,g_Vec);
        for(int i=0; i<4;i++){
            G += tempArray[i];
        }
        vst1q_f32(tempArray,b_Vec);
        for(int i=0; i<4;i++){
            B += tempArray[i];
        }
//    }

    return ((jint) A & 0xff) << 24 | ((jint) R & 0xff) << 16 | ((jint) G & 0xff) << 8 |
           ((jint) B & 0xff);
}


jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(jint *pixels,
                                                                           jfloat *kernelMat,
                                                                           jint i, jint j,
                                                                           jint width,
                                                                           jint height,
                                                                           jint radius) {             // Method to calculate Second (column wise transform) of bitmap image with gaussian vector

    int temp1 = (2*radius+1)+(4-(radius%4));
    __android_log_print(ANDROID_LOG_ERROR, "Matrix of Neon kernel 2nd trans!!!!!:", "%d", radius);
    for(int x = 0; x < temp1; x++){
        __android_log_print(ANDROID_LOG_ERROR," ", "%lf", kernelMat[x]);
    }

    float32_t R = 0.0, G = 0.0, B = 0.0, A = 0.0;

    float32x4_t a_Vec = vdupq_n_f32(0);
    float32x4_t r_Vec = vdupq_n_f32(0);
    float32x4_t g_Vec = vdupq_n_f32(0);
    float32x4_t b_Vec = vdupq_n_f32(0);

//    if (i >= radius && i < height -
//                           radius) {                                                                        // Loop to calculate row transform for the each pixel on the first index [r,(width - r))
        for (int g_index = -radius; g_index <= radius; g_index = g_index+4) {

            uint32x4_t aVec = vdupq_n_u8(0);
            uint32x4_t rVec = vdupq_n_u8(0);
            uint32x4_t gVec = vdupq_n_u8(0);
            uint32x4_t bVec = vdupq_n_u8(0);

            float32x4_t matVec = vld1q_f32(kernelMat);

            uint32x4_t pixelVec = vld1q_u32(&pixels[j + (i + g_index) * width]);

            aVec = (pixelVec >> (3 * 8)) & 0xff;
            rVec = (pixelVec >> (2 * 8)) & 0xff;
            gVec = (pixelVec >> (1 * 8)) & 0xff;
            bVec = (pixelVec >> (0 * 8)) & 0xff;

            a_Vec = vmlaq_f32(a_Vec,aVec,matVec);
            r_Vec = vmlaq_f32(r_Vec,rVec,matVec);
            g_Vec = vmlaq_f32(g_Vec,gVec,matVec);
            b_Vec = vmlaq_f32(b_Vec,bVec,matVec);

            kernelMat = kernelMat + 4;

        }

        float tempArray[4];
        vst1q_f32(tempArray,a_Vec);
        for(int i=0; i<4;i++){
            A += tempArray[i];
        }
        vst1q_f32(tempArray,r_Vec);
        for(int i=0; i<4;i++){
            R += tempArray[i];
        }
        vst1q_f32(tempArray,g_Vec);
        for(int i=0; i<4;i++){
             G += tempArray[i];
        }
        vst1q_f32(tempArray,b_Vec);
        for(int i=0; i<4;i++){
            B += tempArray[i];
        }
//        A = (float32_t) __builtin_neon_vaddvq_f32((int8x16_t)a_Vec);
//        R = (float32_t) __builtin_neon_vaddvq_f32((int8x16_t)r_Vec);
//        G = (float32_t) __builtin_neon_vaddvq_f32((int8x16_t)g_Vec);
//        B = (float32_t) __builtin_neon_vaddvq_f32((int8x16_t)b_Vec);


//    }

    return ((jint) A & 0xff) << 24 | ((jint) R & 0xff) << 16 | ((jint) G & 0xff) << 8 |
           ((jint) B & 0xff);

}


