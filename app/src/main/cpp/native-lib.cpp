                                                                                              // Computing of first transform from 0th row to a0th row of a pixels
#include <jni.h>
#include <string>
#include <cmath>
#include <android/log.h>
#include <arm_neon.h>
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <typeinfo>
#include "tiltshiftBlur_lib.h"
#include "tiltshiftBlur_neon_lib.h"
//#include "tiltshiftBlur_lib.cpp"

    using namespace std;


    extern "C"
    JNIEXPORT jint JNICALL
    Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_tiltshiftcppnative(JNIEnv *env, jobject instance,
    jintArray inputPixels_,
    jintArray outputPixels_,
    jint width, jint height,
    jfloat sigma_far,
    jfloat sigma_near, jint a0,
    jint a1, jint a2,
    jint a3) {
//    timeval curTime,endtime;
//    gettimeofday(&curTime, NULL);
//    long int milli = curTime.tv_sec*1000+curTime.tv_usec/1000;
    jint *pixels = env->GetIntArrayElements(inputPixels_, NULL);                                                // Storing input pixel bitmap into memory
    jint *outputPixels = env->GetIntArrayElements(outputPixels_, NULL);                                         // Storing input bitmap into output pointer

    int kernelRadFar = (int) ceil((2 * sigma_far));                                                            // Gaussian vector radius for far pixels
    int kernelRadNear = (int) ceil(2 * sigma_near);                                                            // Gaussian vector radius for near pixels
    int temp1 = 2*kernelRadFar+1;
    int temp2 = 2*kernelRadNear+1;
    float kernelMatNear[temp2];                                                                                // Far region gaussian vector memory allocation
    float kernelMatFar[temp1];                                                                                 // Near region gaussian vector memmory allocation
//    timer_t start,end;
    cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env, instance,kernelRadFar, sigma_far,kernelMatFar );    // far region gaussian vector calculation call                                                            // Gaussian Vector far
    cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env, instance,kernelRadNear, sigma_near,kernelMatNear);  // near region gaussian vector calculation call                                                     // Gaussian Vector near

    int radius_far=kernelRadFar;
    int radius_near=kernelRadNear;

    int a1a0=a1-a0;
    int a2a3=a3-a2;


//      First Transform Starts(row-wise)
//      q(y, x) = G(-r)*p(y, x-r), + … + G(0)*p(y, x),+ … + G(r)*p(y, x+r)
    for(int i=0;i<a0;i++){                                                                                          // Computing of first transform from 0th row to a0th row of a pixels
        for(int j = 0; j < width; j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,kernelMatFar,i,j,width,radius_far);                       // Function call to calculate first transform for each pixel
        }
    }

    for(int i=a0;i<a1;i++){                                                                                    // Computing of first transform from a0th row to a1th row of the image bitmap
        float newsigmaFar=sigma_far*(a1-i)/a1a0;                                                                   // Computing new sigma value based on the current row of the pixel
        if(newsigmaFar<0.6){

            newsigmaFar=0.6;
        }

        int newGaussVectSize=(int) ceil((2 * sigma_far));                                                       // new gaussian vector radius calculation
        float newGaussVect[2*newGaussVectSize+1];                                                               // new gaussian vector memory allocation
    // New radius based on the updated sigma value
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, newsigmaFar,newGaussVect);           // New gaussian vector using new radius and sigma value

        int newRadius=newGaussVectSize;
        for(int j = 0; j < width; j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,newGaussVect,i,j,width,newRadius);   // Function call to calculate first transform for each pixel
        }

    }
//
    for(int i=a2;i<a3;i++){                                                                                         // Computing of first transform from a2th row to a3th row of the image bitmap
        float newsigmaNear=sigma_near*(i-a2)/a2a3;                                                                 // Computing new sigma value based on the current row of the pixel
        if(newsigmaNear<0.6){
            newsigmaNear=0.6;
        }
        int newGaussVectSize=(int) ceil((2 * sigma_near));                                                     // New radius based on the updated sigma value
        float newGaussVect[2*newGaussVectSize+1];
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize,newsigmaNear,newGaussVect);                                          // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j = 0; j < width; j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,newGaussVect,i,j,width,newRadius);                        // Function call to calculate first transform for each pixel
        }

    }
//
    for(int i=a3;i<height;i++){                                                                                     // Function call to calculate first transform for each pixel from row a3 to height
        for(int j=0;j<width;j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,kernelMatNear,i,j,width,radius_near);                     //Function call to calculate first transform for each pixel

        }
    }
//
////        Second Transform Starts
////        P(y, x) = G(-r)*q(y+r, x), + … + G(0)*q(y, x),+ … + G(r)*q(y+r, x)
//
    for(int i=0;i<(a0);i++) {                                                                                       // Computing of Second transform from 0th row to a0th row of a pixels
    for (int j = 0; j < width ; j++) {
    pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,kernelMatFar,i,j,width,height,radius_far);                      // Function call to calculate Second transform for each pixel
    }
    }
//
//
    for(int i=a0;i<a1;i++){                                                                                             // Computing of Second transform from a0th row to a1th row of the image bitmap
        float newsigmaFar=sigma_far*(a1-i)/a1a0;                                                                   // Computing new sigma value based on the current row pf the pixel
        if(newsigmaFar<0.6){
            newsigmaFar=0.6;
        }
        int newGaussVectSize=(int) ceil((2 * sigma_far));                                                      // New radius based on the updated sigma value
        float newGaussVect[2*newGaussVectSize+1];
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaFar,newGaussVect); // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j=0;j<width;j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius);  // Function call to calculate Second transform for each pixel
        }

    }
//
    for(int i=a2;i<a3;i++){                                                                                         // Computing of Second transform from a2th row to a3th of the image bitmap
        float newsigmaNear=sigma_near*(i-a2)/a2a3;                                                                 // Computing new sigma value based on the current row pf the pixel
        if(newsigmaNear<0.6){
            newsigmaNear=0.6;
         }
        int newGaussVectSize=(int) ceil((2 * sigma_near));                                                     // New radius based on the updated sigma value
        float newGaussVect[2*newGaussVectSize+1];
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaNear,newGaussVect);                                          // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j=0;j<width;j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius); // Function call to calculate Second transform for each pixel
        }
    }
//
    for(int i=a3;i<height;i++) {                                                                                    // Function call to calculate Second transform for each pixel from row a3 to height
        for (int j = 0; j < width ; j++) {
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,kernelMatNear,i,j,width,height,radius_near);                    //Function call to calculate first transform for each pixel

        }
    }
//    gettimeofday(&endtime
//            , NULL);
//    long int fin = endtime.tv_sec*1000+endtime.tv_usec/1000-milli;
//    __android_log_print(ANDROID_LOG_ERROR, "Timestamp :","%ld",fin);
    env->ReleaseIntArrayElements(inputPixels_, pixels, 0);                                                          // sending transformed pixel to java code
    env->ReleaseIntArrayElements(outputPixels_, outputPixels, 0);

//    std::cout << typeid(finish).name() << std::endl;

    return 0;
    }

    extern "C"
    JNIEXPORT jint JNICALL
    Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_tiltshiftneonnative(JNIEnv *env, jobject instance,
                                                                            jintArray inputPixels_,
                                                                            jintArray outputPixels_,
                                                                            jint width, jint height,
                                                                            jfloat sigma_far,
                                                                            jfloat sigma_near, jint a0,
                                                                            jint a1, jint a2,
                                                                            jint a3){

//    timeval curTime,endtime;
//    gettimeofday(&curTime, NULL);
//    long int milli = curTime.tv_sec*1000+curTime.tv_usec/1000;
    jint *pixels = env->GetIntArrayElements(inputPixels_, NULL);                                                // Storing image bitmap into memory and assigning pointer
    jint *outputPixels = env->GetIntArrayElements(outputPixels_, NULL);                                         // Storing image bitmap into memory and assigning different output pointer
    if(sigma_far<0.6){
        sigma_far=0.6;                                                                                          // Capping sigma value
    }
    if(sigma_near<0.6){                                                                                         // capping sigma value
        sigma_near=0.6;
    }
    int kernelRadFar = (int) ceil((2 * sigma_far));                                                            // Gaussian vector radius for far pixels
    int kernelRadNear = (int) ceil(2 * sigma_near);                                                            // Gaussian vector radius for near pixels
    int temp1 = (2*kernelRadFar+1)+(4-((2*kernelRadFar+1)%4));                                                 // converting vector size into next multiple of 4
    int temp2 = (2*kernelRadNear+1)+(4-((2*kernelRadNear+1)%4));                                               // converting vector size into next multiple of 4
    float *kernelMatNear=(float*)calloc (temp2,sizeof(float));                                                 // Far region Gaussian vector with size next multiple of 4 after temp1 extra padded values are 0s
    float *kernelMatFar=(float*)calloc (temp1,sizeof(float));                                                  // Near region Gaussian vector with size next multiple of 4 after temp1 extra padded values are 0s

    cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env, instance,kernelRadFar, sigma_far,kernelMatFar );    // Faar region Gaussian vector + padded zeroes calculation                                                           // Gaussian Vector far
    cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env, instance,kernelRadNear, sigma_near,kernelMatNear);  // Near region Gaussian vector + padded zeroes calculation

    for(int i=0;i<a0;i++){
        for(int j = 0; j < width; j++){
        outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,kernelMatFar,i,j,width,kernelRadFar);  // Function call to calculate first transform in vectorized manner call
        }
    }

    for(int i=a0;i<a1;i++){                                                                                    // Computing of first transform from a0th row to a1th row of the image bitmap
        float newsigmaFar=sigma_far*(a1-i)/(a1-a0);                                                                   // Computing new sigma value based on the current row of the pixel
        if(newsigmaFar<0.6){

            newsigmaFar=0.6;
        }

        int newGaussVectSize=(int) ceil((2 * sigma_far));
        int temp1 = (2*newGaussVectSize+1)+(4-((2*newGaussVectSize+1)%4));                                      // converting vector size into next multiple of 4
        float *newGaussVect=(float*)calloc (temp1,sizeof(float));                                               // Row-wise Gaussian vector with size next multiple of 4 after temp1 extra padded values are 0s
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize,newsigmaFar,newGaussVect);     // New gaussian vector using new radius and sigma value

        int newRadius=newGaussVectSize;
        for(int j = 0; j < width; j++){
            outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,newGaussVect,i,j,width,newRadius); // Function call to calculate first transform in vectorized manner call
         }
        free(newGaussVect);                                                                                     // free calloc memory
    }

    for(int i=a2;i<a3;i++){                                                                                         // Computing of first transform from a2th row to a3th row of the image bitmap
        float newsigmaNear=sigma_near*(i-a2)/(a3-a2);                                                                 // Computing new sigma value based on the current row of the pixel
        if(newsigmaNear<0.6){
            newsigmaNear=0.6;
        }
        int newGaussVectSize=(int) ceil((2 * sigma_near));                                                     // New radius based on the updated sigma value
        int temp1 = (2*newGaussVectSize+1)+(4-((2*newGaussVectSize+1)%4));                                     // converting vector size into next multiple of 4
        float *newGaussVect=(float*)calloc (temp1,sizeof(float));                                              // Row-wise Gaussian vector with size next multiple of 4 after temp1 extra padded values are 0s
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize,newsigmaNear,newGaussVect);   // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j = 0; j < width; j++){
            outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,newGaussVect,i,j,width,newRadius);   // Function call to calculate first transform in vectorized manner call
        }
        free(newGaussVect);
    }

    for(int i=a3;i<height;i++){                                                                                     // Function call to calculate first transform for each pixel from row a3 to height
        for(int j=0;j<width;j++){
            outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,kernelMatNear,i,j,width,kernelRadNear); // Function call to calculate first transform in vectorized manner call
        }
    }


    for(int i=0;i<a0;i++) {                                                                                    // Function call to calculate Second transform for each pixel from row a3 to height
        for (int j = 0; j < width ; ) {
                if(j<width-width%4) {
                    vst1q_s32(&outputPixels[i * width + j],
                              Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(
                                      outputPixels, kernelMatFar, i, j, width, height,
                                  kernelRadFar));                                                              // Each call to neon second transform returns a 32*4 vector with transformed pixel values overwritten on its respective location
                    j+=4;
                }
                else{
                    outputPixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(outputPixels,kernelMatFar,i,j,width,height,kernelRadFar); // If image width is not multiple of 4 then using C++ library is called per pixel
                    j++;
                }

        }
    }

    for(int i=a0;i<a1;i++){                                                                                             // Computing of Second transform from a0th row to a1th row of the image bitmap
            float newsigmaFar=sigma_far*(a1-i)/(a1-a0);                                                                   // Computing new sigma value based on the current row pf the pixel
            if(newsigmaFar<0.6){
                newsigmaFar=0.6;
            }
            int newGaussVectSize=(int) ceil((2 * sigma_far));
            int temp1 = (2*newGaussVectSize+1);
            float *newGaussVect=(float*)calloc (temp1,sizeof(float));
            cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaFar,newGaussVect); // New gaussian vector using new radius and sigma value
            int newRadius=newGaussVectSize;
            for(int j=0;j<width;){
               // Method call to calculate Second transform for each pixel
                if(j<width-width%4) {
                    vst1q_s32(&outputPixels[i * width + j],
                              Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(
                                      outputPixels, newGaussVect, i, j, width, height,
                                      newRadius));                                                                      // Each call to neon second transform returns a 32*4 vector with transformed pixel values overwritten on its respective location
                    j+=4;
                }
                else{
                    outputPixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(outputPixels,newGaussVect,i,j,width,height,newRadius);  // If image width is not multiple of 4 then using C++ library is called per pixel
                    j++;
                }

            }
        free(newGaussVect);                                                                                            // free calloc memory
        }

    for(int i=a2;i<a3;i++){                                                                                         // Computing of Second transform from a2th row to a3th of the image bitmap
        float newsigmaNear=sigma_near*(i-a2)/(a3-a2);                                                                 // Computing new sigma value based on the current row pf the pixel
        if(newsigmaNear<0.6){
            newsigmaNear=0.6;
        }
        int newGaussVectSize=(int) ceil((2 * sigma_near));                                                     // New radius based on the updated sigma value
        int temp1 = (2*newGaussVectSize+1);
        float *newGaussVect=(float*)calloc (temp1,sizeof(float));
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, newsigmaNear,newGaussVect);                                          // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j=0;j<width;){
           if(j<width-width%4) {
                vst1q_s32(&outputPixels[i * width + j],
                          Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(
                                  outputPixels, newGaussVect, i, j, width, height,
                                  newRadius));                                                                           // Each call to neon second transform returns a 32*4 vector with transformed pixel values overwritten on its respective location
                j+=4;
            }
            else{
                outputPixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(outputPixels,newGaussVect,i,j,width,height,newRadius);// If image width is not multiple of 4 then using C++ library is called per pixel
                j++;
            }
        }
        free(newGaussVect);                                                                             // free calloc memory
    }


        for(int i=a3;i<height;i++) {                                                                                    // Function call to calculate Second transform for each pixel from row a3 to height
        for (int j = 0; j < width ; ) {
           if(j<width-width%4) {
                vst1q_s32(&outputPixels[i * width + j],
                          Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(
                                  outputPixels, kernelMatNear, i, j, width, height,
                                  kernelRadNear));                                                                  // Each call to neon second transform returns a 32*4 vector with transformed pixel values overwritten on its respective location
                j+=4;
            }
            else{
                outputPixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(outputPixels,kernelMatNear,i,j,width,height,kernelRadNear);// If image width is not multiple of 4 then using C++ library is called per pixel
                j++;
            }
        }
    }
    free(kernelMatFar);
    free(kernelMatNear);
//    gettimeofday(&endtime, NULL);
//    long int fin = endtime.tv_sec*1000+endtime.tv_usec/1000-milli;
//    __android_log_print(ANDROID_LOG_ERROR, "Timestamp :","%ld",fin);
    env->ReleaseIntArrayElements(inputPixels_, pixels, 0);
    env->ReleaseIntArrayElements(outputPixels_, outputPixels, 0);                                                   // sending transformed pixel to java code
    return 0;


}

