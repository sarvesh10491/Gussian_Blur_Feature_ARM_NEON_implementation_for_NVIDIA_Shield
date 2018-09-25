//
// Created by nanda on 9/23/2018.
//

#ifndef TILT_SHIFT_BLUR_TILTSHIFTBLUR_NEON_LIB_H
#define TILT_SHIFT_BLUR_TILTSHIFTBLUR_NEON_LIB_H
#ifdef __cplusplus
extern "C" {
#endif

jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_firstTransform(jint *pixels,
                                                                               jfloat *kernelMat,
                                                                               jint i, jint j,
                                                                               jint width,
                                                                               jint radius);

jint Java_com_example_achal_tilt_1shift_1blur_GaussianBlur_neon_secondTransform(jint *pixels,
                                                                           jfloat *kernelMat,
                                                                           jint i, jint j,
                                                                           jint width,
                                                                           jint height,
                                                                           jint radius);




#ifdef __cplusplus
}
#endif
#endif //TILT_SHIFT_BLUR_TILTSHIFTBLUR_NEON_LIB_H
