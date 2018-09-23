#include <jni.h>
#include <string>
#include <cmath>
#include <android/log.h>

using namespace std;

//extern "C"
//JNIEXPORT jstring
//
//JNICALL
//Java_com_example_achal_tilt_1shift_1blur_MainActivity_stringFromJNI(
//        JNIEnv *env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}

jdouble Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(jint pixel,jdouble gauss, jint mul){                                                 // Method to calculate transform of each color of an individual pixel
    int G = (pixel >> (mul*8)) & 0xff;
    double ret =  (G * gauss);
    return ret;
}

jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(jint *pixels,jdouble *kernelMat, jint i, jint j,jint width, jint radius){               // Method to calculate First (row wise transform) of bitmap image with gaussian vector

    jdouble R=0.0,G=0.0,B=0.0,A=0.0;
    if(j>=radius && j<width-radius) {                                                                                // Loop to calculate row transform for the each pixel on the first index [r,(width - r))
        for (int g_index = -radius; g_index <= radius; g_index++) {
            A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 3);               //  Color wise transform for Most significant 8 bits
            R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 2);               //  Color wise transform for red bits
            G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 1);               //  Color wise transform for green bits
            B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 0);               //  Color wise transform for blue bits
        }
    }
    else if(j<radius) {                                                                                                      // Loop to calculate row transform for the each pixel on the index [0,r)
        for (int g_index = -j,k=0; g_index <=radius; g_index++,k++) {
            A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 3);                           //  Color wise transform for Most significant 8 bits
            R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 2);                           //  Color wise wise transform for red bits
            G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 1);                           //  Color wise wise transform for green bits
            B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 0);                           //  Color wise wise transform for blue bits
        }
    }
    else                                                                                                             // Loop to calculate row transform for the each pixel on the index [width-r,width)
    {
        for (int g_index = -radius; g_index < width-j; g_index++) {
            A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 3);               //  Color wise transform for Most significant 8 bits
            R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 2);               //  Color wise transform for red bits
            G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 1);               //  Color wise transform for green bits
            B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 0);               //  Color wise transform for blue bits
        }
    }



    return ((jint) A & 0xff) << 24 | ((jint) R & 0xff) << 16 | ((jint) G & 0xff) << 8 | ((jint) B & 0xff);
}

jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(jint *pixels,jdouble *kernelMat, jint i, jint j,jint width,jint height, jint radius) {             // Method to calculate Second (column wise transform) of bitmap image with gaussian vector
    jdouble R = 0.0, G = 0.0, B = 0.0, A = 0.0;
//    int pixelslength = sizeof (pixels) / sizeof (pixels[0]);
//    __android_log_print(ANDROID_LOG_ERROR, "pixelslength :", "%d", pixelslength);
//    jint height = pixelslength / width;
    if (i >= radius && i < height - radius) {                                                                        // Loop to calculate row transform for the each pixel on the first index [r,(width - r))
        for (int g_index = -radius; g_index <= radius; g_index++) {
            A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 3);             //  Color wise transform for Most significant 8 bits
            R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 2);             //  Color wise transform for red bits
            G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 1);             //  Color wise transform for green bits
            B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 0);             //  Color wise transform for blue bits

        }
    }
    else if(i<radius){                                                                                               // Loop to calculate row transform for the each pixel on the index [0,r)
        for (int g_index = -i; g_index <= radius; g_index++) {
            A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 3);             //  Color wise transform for Most significant 8 bits
            R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 2);             //  Color wise transform for red bits
            G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 1);             //  Color wise transform for green bits
            B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 0);             //  Color wise transform for blue bits
        }

    }
    else{                                                                                                            // Loop to calculate row transform for the each pixel on the index [width-r,width)
        for (int g_index = -radius; g_index < height-i; g_index++) {
            A += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 3);             //  Color wise transform for Most significant 8 bits
            R += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 2);             //  Color wise transform for red bits
            G += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 1);             //  Color wise transform for green bits
            B += Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 0);             //  Color wise transform for blue bits

        }
    }

    return ((int) A & 0xff) << 24 | ((int) R & 0xff) << 16 | ((int) G & 0xff) << 8 | ((int) B & 0xff);                // combining all A,R,G,B transformed values into an integer and returning to the caller
}

jdouble Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_getGaussian(JNIEnv *env, jobject instance,jdouble k, jdouble var) {                                                              // Method calculating gaussian transform for a given value of radius and standard deviation
    return ((1 / (2 * M_PI * pow(var, 2))) * (exp(-1*((pow(k, 2)) / (2 * pow(var, 2))))));
}

void Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(JNIEnv *env, jobject instance, jint r, jfloat var, jdouble* matrix) {                                                               // Method to calculate entire gaussian vector

    int matrixsize = (2*r + 1);
    __android_log_print(ANDROID_LOG_ERROR, "matrixsize in kernelmatrix:", "%d", matrixsize);
//    double matrix[(2 * r + 1)];                                                                        // Creating gaussian vector of size 2*r+1
    double total = 0, normTotal = 0;
//    int matrixsize = sizeof (matrix) / sizeof(matrix[0]);
    for (int i = 0; i < matrixsize; i++) {
        matrix[i] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_getGaussian(env,instance,i - r, var);
        total += matrix[i];
    }
    for (int i = 0; i < matrixsize; i++) {
        matrix[i] = matrix[i] / total;
        normTotal += matrix[i];
    }
//    return matrix;
}

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
    int temp1 = 2*kernelRadFar+1;
    int temp2 = 2*kernelRadNear+1;
    double kernelMatNear[temp2];
    double kernelMatFar[temp1];
    Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env, instance,kernelRadFar, sigma_far,kernelMatFar );                                                           // Gaussian Vector far
    Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env, instance,kernelRadNear, sigma_near,kernelMatNear);                                                        // Gaussian Vector near

//    __android_log_print(ANDROID_LOG_ERROR, "Matrix of kernel!!!!!:", "%d", kernelRadFar);
//    for(int i = 0; i < temp1; i++){
//        __android_log_print(ANDROID_LOG_ERROR," ", "%lf", kernelMatFar[i]);
//    }

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
            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,kernelMatFar,i,j,width,radius_far);                       // Method call to calculate first transform for each pixel
        }
    }

    for(int i=a0;i<(int)a1;i++){                                                                                    // Computing of first transform from a0th row to a1th row of the image bitmap
        double newsigmaFar=sigma_far*(a1-i)/a1a0;                                                                   // Computing new sigma value based on the current row of the pixel
        if(newsigmaFar<0.6){

            newsigmaFar=0.6;
        }

        int newGaussVectSize=(int) ceil((2 * sigma_far));
        double newGaussVect[2*newGaussVectSize+1];
        // New radius based on the updated sigma value
        Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaFar,newGaussVect);                                           // New gaussian vector using new radius and sigma value

        int newRadius=newGaussVectSize;
        for(int j = newRadius; j < width-newRadius; j++){
            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,newGaussVect,i,j,width,newRadius);                        // Method call to calculate first transform for each pixel
        }

    }
//
    for(int i=a2;i<a3;i++){                                                                                         // Computing of first transform from a2th row to a3th row of the image bitmap
        double newsigmaNear=sigma_near*(i-a2)/a2a3;                                                                 // Computing new sigma value based on the current row of the pixel
        if(newsigmaNear<0.6){
            newsigmaNear=0.6;
        }
        int newGaussVectSize=(int) ceil((2 * sigma_near));                                                     // New radius based on the updated sigma value
        double newGaussVect[2*newGaussVectSize+1];
        Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaNear,newGaussVect);                                          // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j = newRadius; j < width-newRadius; j++){
            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,newGaussVect,i,j,width,newRadius);                        // Method call to calculate first transform for each pixel
        }

    }
//
    for(int i=a3;i<height;i++){                                                                                     // Method call to calculate first transform for each pixel from row a3 to height
        for(int j=0;j<width;j++){

            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(pixels,kernelMatNear,i,j,width,radius_near);                     //Method call to calculate first transform for each pixel

        }

    }
//
////        Second Transform Starts
////        P(y, x) = G(-r)*q(y+r, x), + … + G(0)*q(y, x),+ … + G(r)*q(y+r, x)
//
    for(int i=0;i<(a0);i++) {                                                                                       // Computing of Second transform from 0th row to a0th row of a pixels
        for (int j = 0; j < width ; j++) {
            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,kernelMatFar,i,j,width,height,radius_far);                      // Method call to calculate Second transform for each pixel
        }
    }
//
//
    for(int i=a0;i<a1;i++){                                                                                             // Computing of Second transform from a0th row to a1th row of the image bitmap
        double newsigmaFar=sigma_far*(a1-i)/a1a0;                                                                   // Computing new sigma value based on the current row pf the pixel
        if(newsigmaFar<0.6){
            newsigmaFar=0.6;
        }
        int newGaussVectSize=(int) ceil((2 * sigma_far));                                                      // New radius based on the updated sigma value
        double newGaussVect[2*newGaussVectSize+1];
        Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaFar,newGaussVect);                                           // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j=0;j<width;j++){
            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius);                       // Method call to calculate Second transform for each pixel
        }

    }
//
    for(int i=a2;i<a3;i++){                                                                                         // Computing of Second transform from a2th row to a3th of the image bitmap
        double newsigmaNear=sigma_near*(i-a2)/a2a3;                                                                 // Computing new sigma value based on the current row pf the pixel
        if(newsigmaNear<0.6){
            newsigmaNear=0.6;
        }
        int newGaussVectSize=(int) ceil((2 * sigma_near));                                                     // New radius based on the updated sigma value
        double newGaussVect[2*newGaussVectSize+1];
        Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(env,instance,newGaussVectSize, (float) newsigmaNear,newGaussVect);                                          // New gaussian vector using new radius and sigma value
        int newRadius=newGaussVectSize;
        for(int j=0;j<width;j++){
            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,newGaussVect,i,j,width,height,newRadius);                       // Method call to calculate Second transform for each pixel
        }
    }
//
    for(int i=a3;i<height;i++) {                                                                                    // Method call to calculate Second transform for each pixel from row a3 to height
        for (int j = 0; j < width ; j++) {
            pixels[i * width + j] = Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(pixels,kernelMatNear,i,j,width,height,radius_near);                    //Method call to calculate first transform for each pixel

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

