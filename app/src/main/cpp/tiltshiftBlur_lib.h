//
// Created by nanda on 9/22/2018.
//

using namespace std;

#ifndef TILT_SHIFT_BLUR_TILTSHIFTBLUR_LIB_H
#define TILT_SHIFT_BLUR_TILTSHIFTBLUR_LIB_H
namespace cppLib {
    void Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_kernelMatrix(JNIEnv *env,
                                                                            jobject instance,
                                                                            jint r, jfloat var,
                                                                            jfloat *matrix);

    jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_firstTransform(jint *pixels,
                                                                              jfloat *kernelMat,
                                                                              jint i, jint j,
                                                                              jint width,
                                                                              jint radius);

    jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_secondTransform(jint *pixels,
                                                                               jfloat *kernelMat,
                                                                               jint i, jint j,
                                                                               jint width,
                                                                               jint height,
                                                                               jint radius);

    jfloat
    Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_colorGaussBlur(jint pixel, jfloat gauss,
                                                                         jint mul);

    jfloat
    Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_getGaussian(JNIEnv *env, jobject instance,
                                                                      jfloat k, jfloat var);
}
extern "C" {
void Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_kernelMatrix(JNIEnv *env,
                                                                             jobject instance,
                                                                             jint r, jfloat var,
                                                                             jfloat *matrix);
jfloat Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_getGaussian(JNIEnv *env,
                                                                              jobject instance,
                                                                              jfloat k,
                                                                              jfloat var);

}
#endif //TILT_SHIFT_BLUR_TILTSHIFTBLUR_LIB_H

