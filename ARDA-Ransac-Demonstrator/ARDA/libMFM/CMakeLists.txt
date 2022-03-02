cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(libMFM)

add_definitions(-DTIXML_USE_STL)

add_library(MFM SHARED MFMCommonFrame.cc  MFMExogamFrame.cc MFMCoboFrame.cc   MFMBasicFrame.cc
			  MFMEbyedatFrame.cc MFMBlobFrame.cc   MFMOscilloFrame.cc MFMMergeFrame.cc
			  MFMScalerDataFrame.cc
			  MFMMutantFrame.cc MFMRibfFrame.cc DataParameters.cc MError.cc)
target_link_libraries(MFM tinyxml -lm -lstdc++)

install(TARGETS MFM DESTINATION bin/)
#
# add_library(MFM SHARED MFMCommonFrame.cc  MFMExogamFrame.cc MFMCoboFrame.cc   MFMBasicFrame.cc
# 			  MFMEbyedatFrame.cc MFMBlobFrame.cc   MFMOscilloFrame.cc MFMMergeFrame.cc
# 			  MFMXmlDataDescriptionFrame.cc MFMScalerDataFrame.cc MFMXmlFileHeaderFrame.cc
# 			  MFMMutantFrame.cc MFMRibfFrame.cc DataParameters.cc MError.cc)
# target_link_libraries(MFM tinyxml)
