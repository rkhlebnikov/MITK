set(CPP_FILES
  Algorithms/vtkPointSetSlicer.cxx
  Algorithms/mitkAngleCorrectByPointFilter.cpp
  Algorithms/mitkCylindricToCartesianFilter.cpp
  Algorithms/mitkGeometryDataSource.cpp
  Algorithms/mitkImageToLookupTableFilter.cpp
  Algorithms/mitkInterpolateLinesFilter.cpp
#  Algorithms/mitkMeshSource.cpp
  Algorithms/mitkPlaneCutFilter.cpp
  Algorithms/mitkPlaneFit.cpp
  Algorithms/mitkPlanesPerpendicularToLinesFilter.cpp
  Algorithms/mitkPointSetToCurvedGeometryFilter.cpp
  Algorithms/mitkPointSetToGeometryDataFilter.cpp
  Algorithms/mitkPointSetIndexToWorldTransformFilter.cpp
  Algorithms/mitkSurfaceIndexToWorldTransformFilter.cpp
  Algorithms/mitkPolygonToRingFilter.cpp
  Algorithms/mitkProbeFilter.cpp
#  Algorithms/mitkUnstructuredGridHistogram.cpp
#  Algorithms/mitkVolumeVisualizationImagePreprocessor.cpp

  DataManagement/mitkAffineTransformationOperation.cpp
  DataManagement/mitkColorSequenceHalfTones.cpp
  DataManagement/mitkDelegateManager.cpp
  DataManagement/mitkDrawOperation.cpp
  DataManagement/mitkExternAbstractTransformGeometry.cpp
  DataManagement/mitkFrameOfReferenceUIDManager.cpp
  DataManagement/mitkItkBaseDataAdapter.cpp
  DataManagement/mitkObjectSet.cpp
  DataManagement/mitkPropertyManager.cpp
  DataManagement/mitkSeedsImage.cpp
  DataManagement/mitkSeedsImageLookupTableSource.cpp
  DataManagement/mitkSphereLandmarkProjector.cpp
# DataManagement/mitkUSLookupTableSource.cpp
  DataManagement/vtkObjectSet.cpp
#  IO/mitkObjFileIOFactory.cpp
#  IO/mitkObjFileReader.cpp
#  IO/mitkPACSPlugin.cpp
#  IO/mitkParRecFileIOFactory.cpp
#  IO/mitkParRecFileReader.cpp
#  IO/mitkPropertyListExportToXmlFile.cpp
#  IO/mitkPropertyListImportFromXmlFile.cpp
#  IO/mitkStlVolumeTimeSeriesIOFactory.cpp
#  IO/mitkStlVolumeTimeSeriesReader.cpp
#  IO/mitkUnstructuredGridVtkWriter.cpp
#  IO/mitkUnstructuredGridVtkWriterFactory.cpp
#  IO/mitkVtkUnstructuredGridIOFactory.cpp
#  IO/mitkVtkUnstructuredGridReader.cpp
#  IO/mitkVtkVolumeTimeSeriesIOFactory.cpp
#  IO/mitkVtkVolumeTimeSeriesReader.cpp
  Interactions/mitkConferenceEventMapper.cpp
  Interactions/mitkConnectPointsInteractor.cpp
  #Interactions/mitkCoordinateSupplier.cpp
  #Interactions/mitkDisplayCoordinateOperation.cpp
  #Interactions/mitkDisplayInteractor.cpp
  Interactions/mitkAffineInteractor3D.cpp
  Interactions/mitkDisplayPointSetInteractor.cpp
  #Interactions/mitkDisplayVectorInteractor.cpp
  Interactions/mitkInteractionDebug.cpp
  Interactions/mitkInteractionDebugger.cpp
  Interactions/mitkPointInteractor.cpp
  Interactions/mitkPointSelectorInteractor.cpp
  #Interactions/mitkPositionTracker.cpp
  Interactions/mitkSeedsInteractor.cpp
  Interactions/mitkSocketClient.cpp
  Interactions/mitkSurfaceDeformationInteractor3D.cpp
  Interactions/mitkSurfaceInteractor.cpp
#  Interactions/mitkCreateSurfaceTool.cpp
#  Rendering/mitkImageBackground2D.cpp
  Rendering/mitkLineMapper2D.cpp
# Rendering/mitkLineVtkMapper3D.cpp
#  Rendering/mitkNativeRenderWindowInteractor.cpp
)


set(RESOURCE_FILES
Interactions/AffineInteraction3D.xml
Interactions/AffineTranslationConfig.xml
Interactions/AffineRotationConfig.xml
Interactions/AffineDeformationConfig.xml
)

