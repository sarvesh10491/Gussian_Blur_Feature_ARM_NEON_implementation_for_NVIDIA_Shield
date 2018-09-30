//
// Created by nanda on 9/22/2018.
//
#include <jni.h>
#include <string>
#include <cmath>
#include <android/log.h>
#include "tiltshiftBlur_lib.h"

//using namespace std;

namespace cppLib {
    jfloat
    Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(jint pixel, jfloat gauss,
                                                                         jint mul) {                                                 // Method to calculate transform of each color of an individual pixel
        int G = (pixel >> (mul * 8)) & 0xff;
        float ret = (G * gauss);
        return ret;
    }

    jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(jint *pixels,
                                                                              jfloat *kernelMat,
                                                                              jint i, jint j,
                                                                              jint width,
                                                                              jint radius) {               // Method to calculate First (row wise transform) of bitmap image with gaussian vector

        jfloat R = 0.0, G = 0.0, B = 0.0, A = 0.0;
        if (j >= radius && j < width -
                               radius) {                                                                                // Loop to calculate row transform for the each pixel on the first index [r,(width - r))
            for (int g_index = -radius; g_index <= radius; g_index++) {
                A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + j + g_index], kernelMat[radius + g_index],
                        3);               //  Color wise transform for Most significant 8 bits
                R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + j + g_index], kernelMat[radius + g_index],
                        2);               //  Color wise transform for red bits
                G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + j + g_index], kernelMat[radius + g_index],
                        1);               //  Color wise transform for green bits
                B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + j + g_index], kernelMat[radius + g_index],
                        0);               //  Color wise transform for blue bits
            }
        } else if (j <
                   radius) {                                                                                                      // Loop to calculate row transform for the each pixel on the index [0,r)
            for (int g_index = -j, k = 0; g_index <= radius; g_index++, k++) {
                A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + k], kernelMat[radius + g_index],
                        3);                           //  Color wise transform for Most significant 8 bits
                R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + k], kernelMat[radius + g_index],
                        2);                           //  Color wise wise transform for red bits
                G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + k], kernelMat[radius + g_index],
                        1);                           //  Color wise wise transform for green bits
                B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + k], kernelMat[radius + g_index],
                        0);                           //  Color wise wise transform for blue bits
            }
        } else                                                                                                             // Loop to calculate row transform for the each pixel on the index [width-r,width)
        {
            for (int g_index = -radius; g_index < width - j; g_index++) {
                A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + j + g_index], kernelMat[radius + g_index],
                        3);               //  Color wise transform for Most significant 8 bits
                R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + j + g_index], kernelMat[radius + g_index],
                        2);               //  Color wise transform for red bits
                G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + j + g_index], kernelMat[radius + g_index],
                        1);               //  Color wise transform for green bits
                B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[i * width + j + g_index], kernelMat[radius + g_index],
                        0);               //  Color wise transform for blue bits
            }
        }


        return ((jint) A & 0xff) << 24 | ((jint) R & 0xff) << 16 | ((jint) G & 0xff) << 8 |
               ((jint) B & 0xff);
    }

    jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(jint *pixels,
                                                                               jfloat *kernelMat,
                                                                               jint i, jint j,
                                                                               jint width,
                                                                               jint height,
                                                                               jint radius) {             // Method to calculate Second (column wise transform) of bitmap image with gaussian vector
        jfloat R = 0.0, G = 0.0, B = 0.0, A = 0.0;
//    int pixelslength = sizeof (pixels) / sizeof (pixels[0]);
//    __android_log_print(ANDROID_LOG_ERROR, "pixelslength :", "%d", pixelslength);
//    jint height = pixelslength / width;
        if (i >= radius && i < height -
                               radius) {                                                                        // Loop to calculate row transform for the each pixel on the first index [r,(width - r))
            for (int g_index = -radius; g_index <= radius; g_index++) {
                A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        3);             //  Color wise transform for Most significant 8 bits
                R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        2);             //  Color wise transform for red bits
                G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        1);             //  Color wise transform for green bits
                B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        0);             //  Color wise transform for blue bits

            }
        } else if (i <
                   radius) {                                                                                               // Loop to calculate row transform for the each pixel on the index [0,r)
            for (int g_index = -i; g_index <= radius; g_index++) {
                A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        3);             //  Color wise transform for Most significant 8 bits
                R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        2);             //  Color wise transform for red bits
                G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        1);             //  Color wise transform for green bits
                B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        0);             //  Color wise transform for blue bits
            }

        } else {                                                                                                            // Loop to calculate row transform for the each pixel on the index [width-r,width)
            for (int g_index = -radius; g_index < height - i; g_index++) {
                A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        3);             //  Color wise transform for Most significant 8 bits
                R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        2);             //  Color wise transform for red bits
                G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        1);             //  Color wise transform for green bits
                B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(
                        pixels[j + (i + g_index) * width], kernelMat[radius + g_index],
                        0);             //  Color wise transform for blue bits

            }
        }

        return ((int) A & 0xff) << 24 | ((int) R & 0xff) << 16 | ((int) G & 0xff) << 8 | ((int) B &
                                                                                          0xff);                // combining all A,R,G,B transformed values into an integer and returning to the caller
    }

    jfloat
    Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_getGaussian(JNIEnv *env, jobject instance,
                                                                      jfloat k,
                                                                      jfloat var) {                                                              // Method calculating gaussian transform for a given value of radius and standard deviation
        return ((1 / (2 * M_PI * pow(var, 2))) * (exp(-1 * ((pow(k, 2)) / (2 * pow(var, 2))))));
    }

    void Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(JNIEnv *env,
                                                                            jobject instance,
                                                                            jint r, jfloat var,
                                                                            jfloat *matrix) {                                                               // Method to calculate entire gaussian vector

        int matrixsize = (2 * r + 1);
//        __android_log_print(ANDROID_LOG_ERROR, "matrixsize in kernelmatrix:", "%d", matrixsize);
//    double matrix[(2 * r + 1)];                                                                        // Creating gaussian vector of size 2*r+1
        double total = 0, normTotal = 0;
//    int matrixsize = sizeof (matrix) / sizeof(matrix[0]);
        for (int i = 0; i < matrixsize; i++) {
            matrix[i] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_getGaussian(env,
                                                                                          instance,
                                                                                          i - r,
                                                                                          var);
            total += matrix[i];
        }
        for (int i = 0; i < matrixsize; i++) {
            matrix[i] = matrix[i] / total;
            normTotal += matrix[i];
        }
//    return matrix;
    }

}