### the directory name
set(directory include/OpenMS/ANALYSIS/OPENSWATH)

### list all header files of the directory here
set(sources_list_h
  PeakPickerMRM.h
  ChromatogramExtractor.h
  ConfidenceScoring.h
  DIAHelper.h
  DIAPrescoring.h
  DIAScoring.h
  MRMIonSeries.h
  MRMAssay.h
  MRMDecoy.h
  MRMFeatureFinderScoring.h
  MRMRTNormalizer.h
  MRMTransitionGroupPicker.h
  OpenSwathHelper.h
  OpenSwathScoring.h
  SpectrumAddition.h
  TransitionTSVReader.h
)

### add path to the filenames
set(sources_h)
foreach(i ${sources_list_h})
	list(APPEND sources_h ${directory}/${i})
endforeach(i)

### source group definition
source_group("Header Files\\OpenMS\\ANALYSIS\\OPENSWATH" FILES ${sources_h})
set_source_files_properties(${directory}/sources.cmake PROPERTIES HEADER_FILE_ONLY TRUE)

set(OpenMS_sources_h ${OpenMS_sources_h} ${sources_h})

