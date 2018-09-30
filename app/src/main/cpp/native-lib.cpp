                                                                                              // Computing of first transform from 0th row to a0th row of a pixels
#include <jni.h>
#include <string>
#include <cmath>
#include <android/log.h>
#include <arm_neon.h>
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
    jint *pixels = env->GetIntArrayElements(inputPixels_, NULL);
    jint *outputPixels = env->GetIntArrayElements(outputPixels_, NULL);

    int kernelRadFar = (int) ceil((2 * sigma_far));                                                            // Gaussian vector radius for far pixels
    int kernelRadNear = (int) ceil(2 * sigma_near);                                                            // Gaussian vector radius for near pixels
    __android_log_print(ANDROID_LOG_ERROR, "height :", "%d", height);
    __android_log_print(ANDROID_LOG_ERROR, "width :", "%d", width);
    __android_log_print(ANDROID_LOG_ERROR, "sigma_far :", "%d", sigma_far);
    __android_log_print(ANDROID_LOG_ERROR, "kernelRadFar :", "%d", kernelRadFar);
    int temp1 = 2*kernelRadFar+1;
    int temp2 = 2*kernelRadNear+1;
    float kernelMatNear[temp2];
    float kernelMatFar[temp1];
    cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env, instance,kernelRadFar, sigma_far,kernelMatFar );                                                           // Gaussian Vector far
    cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env, instance,kernelRadNear, sigma_near,kernelMatNear);                                                        // Gaussian Vector near

    __android_log_print(ANDROID_LOG_ERROR, "Matrix of CPP kernel!!!!!:", "%d", kernelRadFar);
    for(int i = 0; i < temp1; i++){
    __android_log_print(ANDROID_LOG_ERROR," ", "%lf", kernelMatFar[i]);
    }

//    int pixelArrayDimension = (height) * (width);                                             // pixel array dimension
//    int[] pixels = new int[pixelArrayDimension];                                                                    // pixel array creation and  initialization
    int radius_far=kernelRadFar;
    int radius_near=kernelRadNear;
//    int height=input.getHeight();                                                                                   //  image height
//    int width=input.getWidth();                                                                                     //  image width
    int a1a0=a1-a0;
    int a2a3=a3-a2;
//    System.out.println("Array size"+pixels.length+"\tHeight"+height+"\tWidth"+width);

//    input.getPixels(pixels, 0, input.getWidth(), 0, 0, input.getWidth(), input.getHeight());            // Storing pixel values into an integer array

//      First Transform Starts(row-wise)
//      q(y, x) = G(-r)*p(y, x-r), + … + G(0)*p(y, x),+ … + G(r)*p(y, x+r)
    for(int i=0;i<a0;i++){                                                                                          // Computing of first transform from 0th row to a0th row of a pixels
        for(int j = 0; j < width; j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,kernelMatFar,i,j,width,radius_far);                       // Method call to calculate first transform for each pixel
        }
    }

    for(int i=a0;i<(int)a1;i++){                                                                                    // Computing of first transform from a0th row to a1th row of the image bitmap
        float newsigmaFar=sigma_far*(a1-i)/a1a0;                                                                   // Computing new sigma value based on the current row of the pixel
        if(newsigmaFar<0.6){

            newsigmaFar=0.6;
        }

        int newGaussVectSize=(int) ceil((2 * sigma_far));
        float newGaussVect[2*newGaussVectSize+1];
    // New radius based on the updated sigma value
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaFar,newGaussVect);                                           // New gaussian vector using new radius and sigma value

        int newRadius=newGaussVectSize;
        for(int j = newRadius; j < width-newRadius; j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,newGaussVect,i,j,width,newRadius);                        // Method call to calculate first transform for each pixel
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
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaNear,newGaussVect);                                          // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j = newRadius; j < width-newRadius; j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,newGaussVect,i,j,width,newRadius);                        // Method call to calculate first transform for each pixel
        }

    }
//
    for(int i=a3;i<height;i++){                                                                                     // Method call to calculate first transform for each pixel from row a3 to height
        for(int j=0;j<width;j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,kernelMatNear,i,j,width,radius_near);                     //Method call to calculate first transform for each pixel

        }
    }
//
////        Second Transform Starts
////        P(y, x) = G(-r)*q(y+r, x), + … + G(0)*q(y, x),+ … + G(r)*q(y+r, x)
//
    for(int i=0;i<(a0);i++) {                                                                                       // Computing of Second transform from 0th row to a0th row of a pixels
    for (int j = 0; j < width ; j++) {
    pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,kernelMatFar,i,j,width,height,radius_far);                      // Method call to calculate Second transform for each pixel
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
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaFar,newGaussVect);                                           // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j=0;j<width;j++){
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius);                       // Method call to calculate Second transform for each pixel
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
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius);                       // Method call to calculate Second transform for each pixel
        }
    }
//
    for(int i=a3;i<height;i++) {                                                                                    // Method call to calculate Second transform for each pixel from row a3 to height
        for (int j = 0; j < width ; j++) {
            pixels[i * width + j] = cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,kernelMatNear,i,j,width,height,radius_near);                    //Method call to calculate first transform for each pixel

        }
    }


//    for (int j=0;j<height;j++){
//        for (int i=0;i<width;i++) {
//            int B = pixels[j*width+i]%0xff;
//            int G = (pixels[j*width+i]>>8)%0xff;
//            int R = (pixels[j*width+i]>>16)%0xff;
//            int A = 0xff;
//            R=0;
//            int color = (A & 0xff) << 24 | (R & 0xff) << 16 | (G & 0xff) << 8 | (B & 0xff);
//
//            outputPixels[j*width+i]=color;
//        }
//    }

    env->ReleaseIntArrayElements(inputPixels_, pixels, 0);
    env->ReleaseIntArrayElements(outputPixels_, outputPixels, 0);
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

    jint *pixels = env->GetIntArrayElements(inputPixels_, NULL);
//        __android_log_print(ANDROID_LOG_ERROR, "Matrix of Neon kernel!!!!!:");
        for (int g_index = 0; g_index <= 7; g_index++){
            __android_log_print(ANDROID_LOG_ERROR," ", "%d ", pixels[g_index]);
        }
    jint *outputPixels = env->GetIntArrayElements(outputPixels_, NULL);
    if(sigma_far<0.6){
        sigma_far=0.6;
    }
    if(sigma_near<0.6){
        sigma_near=0.6;
    }
    int kernelRadFar = (int) ceil((2 * sigma_far));                                                            // Gaussian vector radius for far pixels
    int kernelRadNear = (int) ceil(2 * sigma_near);                                                            // Gaussian vector radius for near pixels
    __android_log_print(ANDROID_LOG_ERROR, "a0 :", "%d", a0);
    __android_log_print(ANDROID_LOG_ERROR, "a3 :", "%d", a1);
    __android_log_print(ANDROID_LOG_ERROR, "a2 :", "%f", sigma_far);
    __android_log_print(ANDROID_LOG_ERROR, "a3 :", "%f",sigma_near);
    int temp1 = (2*kernelRadFar+1)+(4-((2*kernelRadFar+1)%4));
    int temp2 = (2*kernelRadNear+1)+(4-((2*kernelRadNear+1)%4));
    float *kernelMatNear=(float*)calloc (temp2,sizeof(float));
    float *kernelMatFar=(float*)calloc (temp1,sizeof(float));

    cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env, instance,kernelRadFar, sigma_far,kernelMatFar );                                                           // Gaussian Vector far
    cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env, instance,kernelRadNear, sigma_near,kernelMatNear);

    __android_log_print(ANDROID_LOG_ERROR, "Matrix of Neon kernel!!!!!:", "%d", kernelRadFar);
    for(int i = 0; i < temp1; i++){
        __android_log_print(ANDROID_LOG_ERROR," ", "%lf", kernelMatFar[i]);
    }
//    __android_log_print(ANDROID_LOG_ERROR, "Pixel before:", "%d", pixels[30+width*30]);
    for(int i=0;i<a0;i++){
        for(int j = 0; j < width; j++){
            outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,kernelMatFar,i,j,width,kernelRadFar);                       // Method call to calculate first transform for each pixel
//            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,kernelMatFar,i,j,width,kernelRadFar);
        }
    }

    for(int i=a0;i<a1;i++){                                                                                    // Computing of first transform from a0th row to a1th row of the image bitmap
        float newsigmaFar=sigma_far*(a1-i)/(a1-a0);                                                                   // Computing new sigma value based on the current row of the pixel
        if(newsigmaFar<0.6){

            newsigmaFar=0.6;
        }

        int newGaussVectSize=(int) ceil((2 * sigma_far));
        int temp1 = (2*newGaussVectSize+1)+(4-((2*newGaussVectSize+1)%4));
        float *newGaussVect=(float*)calloc (temp1,sizeof(float));
        //float newGaussVect[2*newGaussVectSize+1];
        // New radius based on the updated sigma value
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize,newsigmaFar,newGaussVect);                                           // New gaussian vector using new radius and sigma value

        int newRadius=newGaussVectSize;
        for(int j = 0; j < width; j++){
            outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,newGaussVect,i,j,width,newRadius);
//            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,newGaussVect,i,j,width,newRadius);                        // Method call to calculate first transform for each pixel
        }

    }

    for(int i=a2;i<a3;i++){                                                                                         // Computing of first transform from a2th row to a3th row of the image bitmap
        float newsigmaNear=sigma_near*(i-a2)/(a3-a2);                                                                 // Computing new sigma value based on the current row of the pixel
        if(newsigmaNear<0.6){
            newsigmaNear=0.6;
        }
        int newGaussVectSize=(int) ceil((2 * sigma_near));                                                     // New radius based on the updated sigma value
        int temp1 = (2*newGaussVectSize+1)+(4-((2*newGaussVectSize+1)%4));
        float *newGaussVect=(float*)calloc (temp1,sizeof(float));
        //float newGaussVect[2*newGaussVectSize+1];
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize,newsigmaNear,newGaussVect);                                          // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j = 0; j < width; j++){
            outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,newGaussVect,i,j,width,newRadius);
//            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,newGaussVect,i,j,width,newRadius);                        // Method call to calculate first transform for each pixel
            outputPixels[i * width + j]=cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius);
        }

    }

    for(int i=a3;i<height;i++){                                                                                     // Method call to calculate first transform for each pixel from row a3 to height
        for(int j=0;j<width;j++){
            outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,kernelMatNear,i,j,width,kernelRadNear);
//            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(pixels,kernelMatNear,i,j,width,kernelRadNear);                     //Method call to calculate first transform for each pixel

        }
    }

//        __android_log_print(ANDROID_LOG_ERROR, "Pixel after 1:", "%d", pixels[30+width*30]);

    for(int i=0;i<a0;i++) {                                                                                    // Method call to calculate Second transform for each pixel from row a3 to height
        for (int j = 0; j < width ; j+=4) {
//            outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,kernelMatNear,i,j,width,height,kernelRadNear);                    //Method call to calculate first transform for each pixel
//            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,kernelMatFar,i,j,width,height,kernelRadFar);
//            outputPixels[i * width + j] =;

//                int32x4_t ret_transform = i * width + j;
                vst1q_s32(&outputPixels[i * width + j], Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,kernelMatFar,i,j,width,height,kernelRadFar));

        }
    }
//
    for(int i=a0;i<a1;i++){                                                                                             // Computing of Second transform from a0th row to a1th row of the image bitmap
            float newsigmaFar=sigma_far*(a1-i)/(a1-a0);                                                                   // Computing new sigma value based on the current row pf the pixel
            if(newsigmaFar<0.6){
                newsigmaFar=0.6;
            }
            int newGaussVectSize=(int) ceil((2 * sigma_far));
            int temp1 = (2*newGaussVectSize+1)+(4-((2*newGaussVectSize+1)%4));
            float *newGaussVect=(float*)calloc (temp1,sizeof(float));
            cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaFar,newGaussVect);                                           // New gaussian vector using new radius and sigma value
            int newRadius=newGaussVectSize;
            for(int j=0;j<width;j+=4){
//                outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius);
//                pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius);                       // Method call to calculate Second transform for each pixel
                vst1q_s32(&outputPixels[i * width + j],Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius));
            }

        }

    for(int i=a2;i<a3;i++){                                                                                         // Computing of Second transform from a2th row to a3th of the image bitmap
        float newsigmaNear=sigma_near*(i-a2)/(a3-a2);                                                                 // Computing new sigma value based on the current row pf the pixel
        if(newsigmaNear<0.6){
            newsigmaNear=0.6;
        }
        int newGaussVectSize=(int) ceil((2 * sigma_near));                                                     // New radius based on the updated sigma value
        int temp1 = (2*newGaussVectSize+1)+(4-((2*newGaussVectSize+1)%4));
        float *newGaussVect=(float*)calloc (temp1,sizeof(float));
        //float newGaussVect[2*newGaussVectSize+1];
        cppLib::Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, newsigmaNear,newGaussVect);                                          // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j=0;j<width;j++){
//            outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius);
//            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius);                       // Method call to calculate Second transform for each pixel
            vst1q_s32(&outputPixels[i * width + j],Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius));
        }
    }


        for(int i=a3;i<height;i++) {                                                                                    // Method call to calculate Second transform for each pixel from row a3 to height
        for (int j = 0; j < width ; j+=4) {
//            outputPixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,kernelMatNear,i,j,width,height,kernelRadNear);
//            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,kernelMatNear,i,j,width,height,kernelRadNear);                    //Method call to calculate first transform for each pixel
            vst1q_s32(&outputPixels[i * width + j], Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(pixels,kernelMatNear,i,j,width,height,kernelRadNear));
        }
    }
        __android_log_print(ANDROID_LOG_ERROR, "Pixel after 2:", "%d", pixels[30+width*30]);
    env->ReleaseIntArrayElements(inputPixels_, pixels, 0);
    env->ReleaseIntArrayElements(outputPixels_, outputPixels, 0);
    return 0;


}

