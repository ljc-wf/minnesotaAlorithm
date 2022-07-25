cmake_minimum_required(VERSION 3.18.1)

add_library(dll1 SHARED
            dll1.c abs.c conv.c diff.c dll1_emxutil.c dll1_initialize.c dll1_terminate.c eml_setop.c filter.c filtfilt.c findpeaks.c flipud.c
            ixamax.c mean.c mrdivide.c nonSingletonDim.c norm.c pan_tompkin.c polyfit.c power.c qrsolve.c rt_nonfinite.c rtGetInf.c rtGetNaN.c
            sign.c sort1.c sortIdx.c std.c vander.c xgemv.c xgeqp3.c xgerc.c xnrm2.c xswap.c xzlarf.c xzlarfg.c polyval.c Minnesota.c minnesotaproject.cpp)

# Include libraries needed for hello-jni lib
target_link_libraries(dll1
                      android
                      log)
