//
// Created by nanda on 9/23/2018.
//
#include <jni.h>
#include <android/log.h>
#include <arm_neon.h>
#include <stdio.h>
#include "tiltshiftBlur_neon_lib.h"


jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(jint *pixels,
                                                                          jfloat *kernelMat,
                                                                          jint i, jint j,
                                                                          jint width,
                                                                          jint radius) {                        // This function calculates row wise transform for one pixel using neon intrisic libraries
                                                                                                                // Transform is calulated for each memory location and it is vectorized by accesing neighboring 4 memory location using vectors
    jfloat R = 0.0, G = 0.0, B = 0.0, A = 0.0;                                                                  // Individual color transform is done with 4*32*4 int vectors each 32*4 vector for a color

    float32x4_t a_Vec = vdupq_n_f32(0);                                                                        // 4*32*4 float vector to store intermediate transform values
    float32x4_t r_Vec = vdupq_n_f32(0);
    float32x4_t g_Vec = vdupq_n_f32(0);
    float32x4_t b_Vec = vdupq_n_f32(0);


    if (j >= radius && j < width -
                           radius) {
        for (int g_index = -radius; g_index <= radius; g_index = g_index + 4) {                                 // Loop to calculate row transform for the each pixel on the first index [r,(width - r))
            int32x4_t aVec ;                                                                                    // These 4*32*4 vector loads A,R,G,B values for each 4 neighbors accessed during each iteration
            int32x4_t rVec ;
            int32x4_t gVec ;
            int32x4_t bVec ;

            float32x4_t matVec = vld1q_f32(kernelMat);                                                          // 4 location of gaussian vector loaded

            int32x4_t pixelVec = vld1q_s32(&pixels[i * width + j + g_index]);                                   // 4 location of pixels accesed each iteration


            aVec = (pixelVec >> (3 * 8)) & 0xff;                                                                // Extracting A, R, G, B values for each pixels
            rVec = (pixelVec >> (2 * 8)) & 0xff;
            gVec = (pixelVec >> (1 * 8)) & 0xff;
            bVec = (pixelVec >> (0 * 8)) & 0xff;
//            print_uint32(rVec,"rVec");
            float32x4_t temp_int_Vec = vcvtq_f32_s32(aVec);                                                     // calculating individual color transform and accumulating in *color*_vec
            a_Vec = vmlaq_f32(a_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(rVec);
            r_Vec = vmlaq_f32(r_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(gVec);
            g_Vec = vmlaq_f32(g_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(bVec);
            b_Vec = vmlaq_f32(b_Vec, temp_int_Vec, matVec);
//            print_float32(r_Vec,"r_Vec");
            kernelMat = kernelMat + 4;                                                                          // incrementing kernel vector location by 4
        }
    } else if (j < radius) {                                                                                     // Loop to calculate row transform for the each pixel on the index [0,r)
        for (int g_index = -j; g_index <= radius; g_index+=4) {
            int32x4_t aVec ;                                                                                    // These 4*32*4 vector loads A,R,G,B values for each 4 neighbors accessed during each iteration
            int32x4_t rVec ;
            int32x4_t gVec ;
            int32x4_t bVec ;

            float32x4_t matVec = vld1q_f32(kernelMat+radius + g_index);                                         // 4 location of gaussian vector loaded

            int32x4_t pixelVec = vld1q_s32(&pixels[i * width + j + g_index]);                                   // 4 location of pixels accesed each iteration

            aVec = (pixelVec >> (3 * 8)) & 0xff;                                                                // Extracting A, R, G, B values for each pixels
            rVec = (pixelVec >> (2 * 8)) & 0xff;
            gVec = (pixelVec >> (1 * 8)) & 0xff;
            bVec = (pixelVec >> (0 * 8)) & 0xff;
//            print_uint32(rVec,"rVec");
            float32x4_t temp_int_Vec = vcvtq_f32_s32(aVec);                                                     // calculating individual color transform and accumulating in *color*_vec
            a_Vec = vmlaq_f32(a_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(rVec);
            r_Vec = vmlaq_f32(r_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(gVec);
            g_Vec = vmlaq_f32(g_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(bVec);
            b_Vec = vmlaq_f32(b_Vec, temp_int_Vec, matVec);

        }
    } else                                                                                                    // Loop to calculate row transform for the each pixel on the index [width-r,width)
    {
        for (int g_index = -radius; g_index < width - j; g_index+=4) {

            int32x4_t aVec ;                                                                                 // These 4*32*4 vector loads A,R,G,B values for each 4 neighbors accessed during each iteration
            int32x4_t rVec ;
            int32x4_t gVec ;
            int32x4_t bVec ;

            float32x4_t matVec = vld1q_f32(kernelMat+radius + g_index);                                     // 4 location of gaussian vector loaded

            int32x4_t pixelVec = vld1q_s32(&pixels[i * width + j + g_index]);                               // 4 location of pixels accesed each iteration

            aVec = (pixelVec >> (3 * 8)) & 0xff;                                                            // Extracting A, R, G, B values for each pixels
            rVec = (pixelVec >> (2 * 8)) & 0xff;
            gVec = (pixelVec >> (1 * 8)) & 0xff;
            bVec = (pixelVec >> (0 * 8)) & 0xff;
//            print_uint32(rVec,"rVec");
            float32x4_t temp_int_Vec = vcvtq_f32_s32(aVec);
            a_Vec = vmlaq_f32(a_Vec, temp_int_Vec, matVec);                                                 // calculating individual color transform and accumulating in *color*_vec
            temp_int_Vec = vcvtq_f32_s32(rVec);
            r_Vec = vmlaq_f32(r_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(gVec);
            g_Vec = vmlaq_f32(g_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(bVec);
            b_Vec = vmlaq_f32(b_Vec, temp_int_Vec, matVec);

        }
    }

        float tempArray[4];
        vst1q_f32(tempArray, a_Vec);                                                                        // Storing accumulated trasformed values in the *color*_vec
        for (int x = 0; x < 4; x++) {
            A += tempArray[x];
        }
        vst1q_f32(tempArray, r_Vec);
        for (int x = 0; x < 4; x++) {
            R += tempArray[x];
        }

        vst1q_f32(tempArray, g_Vec);
        for (int x = 0; x < 4; x++) {
            G += tempArray[x];
        }

        vst1q_f32(tempArray, b_Vec);
        for (int x = 0; x < 4; x++) {
            B += tempArray[x];
        }

        return ((jint) A & 0xff) << 24 | ((jint) R & 0xff) << 16 | ((jint) G & 0xff) << 8 |
               ((jint) B & 0xff);                                                                          // calculating final values from accumulated color vector values
    }

// Second/ Column transform
    int32x4_t Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(jint *pixels,
                                                                                    jfloat *kernelMat,
                                                                                    jint i, jint j,
                                                                                    jint width,
                                                                                    jint height,
                                                                                    jint radius) {             // Function calculates column transform, since column memory is not stored sequencially
                                                                                                               // We access 4 pixels at a time and calulate transform on them
                                                                                                               // No of iternations done on these 4 pixels are equal to the length of gaussian vector
                                                                                                               // Thus vectorization done in a little different manner
    float32_t R = 0.0, G = 0.0, B = 0.0, A = 0.0;
    float32x4_t a_Vec = vdupq_n_f32(0);                                                                        // These 4*32*4 vector loads A,R,G,B values for each 4 neighbors accessed during each iteration
    float32x4_t r_Vec = vdupq_n_f32(0);
    float32x4_t g_Vec = vdupq_n_f32(0);
    float32x4_t b_Vec = vdupq_n_f32(0);
if (i >= radius && i < height - radius) {                                                                      // Loop to calculate row transform for the each pixel on the first index [r,(width - r))
        for (int g_index = -radius; g_index <= radius; g_index++) {

            int32x4_t aVec;                                                                                   // These 4*32*4 vector loads A,R,G,B values for each 4 pixels accessed during each iteration
            int32x4_t rVec;
            int32x4_t gVec;
            int32x4_t bVec;

            float32x4_t matVec = vdupq_n_f32(kernelMat[radius+g_index]);                                      // Loading same kernel vector value and duplicating it across matVec

            int32x4_t pixelVec = vld1q_s32(&pixels[j + (i + g_index) * width]);                               // 4 location of pixels accessed each iteration, column wise memory load

            aVec = (pixelVec >> (3 * 8)) & 0xff;                                                              // Extracting A, R, G, B values for each pixels
            rVec = (pixelVec >> (2 * 8)) & 0xff;
            gVec = (pixelVec >> (1 * 8)) & 0xff;
            bVec = (pixelVec >> (0 * 8)) & 0xff;

            float32x4_t temp_int_Vec = vcvtq_f32_s32(aVec);                                                   // calculating and accumulating individual color transform and accumulating in *color*_vec
            a_Vec = vmlaq_f32(a_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(rVec);
            r_Vec = vmlaq_f32(r_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(gVec);
            g_Vec = vmlaq_f32(g_Vec, temp_int_Vec, matVec);
            temp_int_Vec = vcvtq_f32_s32(bVec);
            b_Vec = vmlaq_f32(b_Vec, temp_int_Vec, matVec);
        }
    } else if (i < radius) {                                                                                 // Loop to calculate row transform for the each pixel on the index [0,r)
            for (int g_index = -i; g_index <= radius; g_index++) {

                int32x4_t aVec;                                                                              // These 4*32*4 vector loads A,R,G,B values for each 4 pixels accessed during each iteration
                int32x4_t rVec;
                int32x4_t gVec;
                int32x4_t bVec;

                float32x4_t matVec = vdupq_n_f32(kernelMat[radius+g_index]);                                // Loading same kernel vector value and duplicating it across matVec

                int32x4_t pixelVec = vld1q_s32(&pixels[j + (i + g_index) * width]);                         // 4 location of pixels accessed each iteration, column wise memory load

                aVec = (pixelVec >> (3 * 8)) & 0xff;                                                        // Extracting A, R, G, B values for each pixels
                rVec = (pixelVec >> (2 * 8)) & 0xff;
                gVec = (pixelVec >> (1 * 8)) & 0xff;
                bVec = (pixelVec >> (0 * 8)) & 0xff;

                float32x4_t temp_int_Vec = vcvtq_f32_s32(aVec);                                             // calculating and accumulating individual color transform and accumulating in *color*_vec
                a_Vec = vmlaq_f32(a_Vec, temp_int_Vec, matVec);
                temp_int_Vec = vcvtq_f32_s32(rVec);
                r_Vec = vmlaq_f32(r_Vec, temp_int_Vec, matVec);
                temp_int_Vec = vcvtq_f32_s32(gVec);
                g_Vec = vmlaq_f32(g_Vec, temp_int_Vec, matVec);
                temp_int_Vec = vcvtq_f32_s32(bVec);
                b_Vec = vmlaq_f32(b_Vec, temp_int_Vec, matVec);

            }
        } else {


//             Loop to calculate row transform for the each pixel on the index [width-r,width)
            for (int g_index = -radius; g_index < height - i; g_index++) {

                int32x4_t aVec;                                                                             // These 4*32*4 vector loads A,R,G,B values for each 4 pixels accessed during each iteration
                int32x4_t rVec;
                int32x4_t gVec;
                int32x4_t bVec;

                float32x4_t matVec = vdupq_n_f32(kernelMat[radius+g_index]);                                // Loading same kernel vector value and duplicating it across matVec

                int32x4_t pixelVec = vld1q_s32(&pixels[j + (i + g_index) * width]);                         // 4 location of pixels accessed each iteration, column wise memory load

                aVec = (pixelVec >> (3 * 8)) & 0xff;                                                        // Extracting A, R, G, B values for each pixels
                rVec = (pixelVec >> (2 * 8)) & 0xff;
                gVec = (pixelVec >> (1 * 8)) & 0xff;
                bVec = (pixelVec >> (0 * 8)) & 0xff;

                float32x4_t temp_int_Vec = vcvtq_f32_s32(aVec);                                             // calculating and accumulating individual color transform and accumulating in *color*_vec
                a_Vec = vmlaq_f32(a_Vec, temp_int_Vec, matVec);
                temp_int_Vec = vcvtq_f32_s32(rVec);
                r_Vec = vmlaq_f32(r_Vec, temp_int_Vec, matVec);
                temp_int_Vec = vcvtq_f32_s32(gVec);
                g_Vec = vmlaq_f32(g_Vec, temp_int_Vec, matVec);
                temp_int_Vec = vcvtq_f32_s32(bVec);
                b_Vec = vmlaq_f32(b_Vec, temp_int_Vec, matVec);

             }

}

        return (((vcvtq_s32_f32 (a_Vec) & 0xff) << 24) | ((vcvtq_s32_f32 (r_Vec) & 0xff) << 16) | ((vcvtq_s32_f32 (g_Vec) & 0xff) << 8) | ((vcvtq_s32_f32 (b_Vec) & 0xff))); // Aggregating color values calculated for all the 4 pixels and storing in one vector


}



