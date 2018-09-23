//
// Created by nanda on 9/22/2018.
//

//using namespace std;
namespace cppLib {
#ifndef TILT_SHIFT_BLUR_TILTSHIFTBLUR_LIB_H
#define TILT_SHIFT_BLUR_TILTSHIFTBLUR_LIB_H

    void Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(JNIEnv *env,
                                                                            jobject instance,
                                                                            jint r, jfloat var,
                                                                            jdouble *matrix);

    jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(jint *pixels,
                                                                              jdouble *kernelMat,
                                                                              jint i, jint j,
                                                                              jint width,
                                                                              jint radius);

    jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(jint *pixels,
                                                                               jdouble *kernelMat,
                                                                               jint i, jint j,
                                                                               jint width,
                                                                               jint height,
                                                                               jint radius);

    jdouble
    Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(jint pixel, jdouble gauss,
                                                                         jint mul);

    jdouble
    Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_getGaussian(JNIEnv *env, jobject instance,
                                                                      jdouble k, jdouble var);
}
#endif //TILT_SHIFT_BLUR_TILTSHIFTBLUR_LIB_H
