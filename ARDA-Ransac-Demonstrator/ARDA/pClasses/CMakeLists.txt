cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(pClasses)

file(GLOB PC_FILES "src/p*.cpp")

ROOT_GENERATE_DICTIONARY(G__dict__CFD pCFD.h LINKDEF pCFDLinkDef.h)
ROOT_GENERATE_DICTIONARY(G__dict__DigitalSignal pDigitalSignal.h LINKDEF pDigitalSignalLinkDef.h)
ROOT_GENERATE_DICTIONARY(G__dict__DSP pDSP.h LINKDEF pDSPLinkDef.h)
ROOT_GENERATE_DICTIONARY(G__dict__FT pFT.h LINKDEF pFTLinkDef.h)
ROOT_GENERATE_DICTIONARY(G__dict__Signal pSignal.h LINKDEF pSignalLinkDef.h)
ROOT_GENERATE_DICTIONARY(G__dict__SplineArcSignal pSplineArcSignal.h LINKDEF pSplineArcSignalLinkDef.h)
ROOT_GENERATE_DICTIONARY(G__dict__Utility pUtility.h LINKDEF pUtilityLinkDef.h)
add_library(pClasses SHARED G__dict__CFD.cxx G__dict__DigitalSignal.cxx G__dict__DSP.cxx G__dict__FT.cxx G__dict__Signal.cxx G__dict__SplineArcSignal.cxx G__dict__Utility.cxx ${PC_FILES})
target_link_libraries(pClasses ${ROOT_LIBRARIES} -lm -lstdc++)
install(TARGETS pClasses DESTINATION bin/)
#install(FILES libdict__CFD_rdict.pcm libdict__DigitalSignal_rdict.pcm libdict__DSP_rdict.pcm libdict__FT_rdict.pcm libdict__Signal_rdict.pcm libdict__SplineArcSignal_rdict.pcm libdict__Utility_rdict.pcm libdict_rdict.pcm DESTINATION bin/)
install(DIRECTORY ${CMAKE_BINARY_DIR}/pClasses/ DESTINATION bin/ FILES_MATCHING PATTERN "lib*.pcm")
