### the directory name
set(directory source/ANALYSIS/OPENSWATH)

### list all header files of the directory here
set(sources_list
MRMIonSeries.cpp
MRMAssay.cpp
MRMDecoy.cpp
MRMRTNormalizer.cpp
TransitionTSVReader.cpp
OpenSwathHelper.cpp
OpenSwathScoring.cpp
ChromatogramExtractor.cpp
ChromatogramExtractorAlgorithm.cpp
SpectrumAddition.cpp
MRMTransitionGroupPicker.cpp
DIAHelper.cpp
DIAScoring.cpp
DIAPrescoring.cpp
MRMFeatureFinderScoring.cpp
ConfidenceScoring.cpp
PeakPickerMRM.cpp
SwathWindowLoader.cpp
)

### add path to the filenames
set(sources)
foreach(i ${sources_list})
	list(APPEND sources ${directory}/${i})
endforeach(i)

### pass source file list to the upper instance
set(OpenMS_sources ${OpenMS_sources} ${sources})

### source group definition
source_group("Source Files\\ANALYSIS\\OPENSWATH" FILES ${sources})
set_source_files_properties(${directory}/sources.cmake PROPERTIES HEADER_FILE_ONLY TRUE)
