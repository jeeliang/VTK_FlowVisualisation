#include "vtkRenderer.h"
#include "vtkPointSet.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolume16Reader.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkOutlineFilter.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"
#include "vtkStructuredPointsReader.h"
#include "vtkMarchingCubes.h"
#include "vtkRecursiveDividingCubes.h"
#include "vtkScalarBarWidget.h"
#include "vtkScalarBarActor.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
//#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
//#include "vtkVolumeRayCastMIPFunction.h"
#include "vtkStructuredPointsReader.h"
#include "vtkThresholdPoints.h"
#include "vtkMaskPoints.h"
#include "vtkConeSource.h"
#include "vtkGlyph3D.h"
#include "vtkLookupTable.h"
#include "vtkCommand.h"
#include "vtkHedgeHog.h"
#include "vtkStreamTracer.h"
#include "vtkHyperStreamline.h"
#include "vtkPoints.h"


int main (int argc, char **argv)
{

  // Create the renderer, the render window, and the interactor. The renderer
  // draws into the render window, the interactor enables mouse- and 
  // keyboard-based interaction with the data within the render window.
	vtkRenderer *aRenderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();

	renWin->AddRenderer(aRenderer);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

  // Read
	vtkStructuredPointsReader *reader= vtkStructuredPointsReader::New();
	reader->SetFileName("../data/testData1.vtk");
	reader->Update();

  // Lookup Table
	vtkLookupTable *lut = vtkLookupTable::New();
	lut->SetHueRange( 0.667, 0.0 );
	lut->Build();

	// Glyph-based visualization using cones
	vtkConeSource* coneSource = vtkConeSource::New();
	// Set any properties for the cone source if desired (e.g., height, radius, resolution).

	vtkGlyph3D* glyphFilter = vtkGlyph3D::New();
	glyphFilter->SetInputConnection(reader->GetOutputPort());
	glyphFilter->SetSourceConnection(coneSource->GetOutputPort());
	glyphFilter->SetScaleFactor(1);
	glyphFilter->SetScaleModeToScaleByVector();

	vtkPolyDataMapper* glyphMapper = vtkPolyDataMapper::New();
	glyphMapper->SetInputConnection(glyphFilter->GetOutputPort());
	glyphMapper->SetScalarRange(0.0, 1.0);
	glyphMapper->SetLookupTable(lut);

	vtkActor* glyphActor = vtkActor::New();
	glyphActor->SetMapper(glyphMapper);

	aRenderer->AddActor(glyphActor);

  // Set a background color for the renderer and set the size of the
  // render window (expressed in pixels).
	aRenderer->SetBackground(0, 0, 0);
	renWin->SetSize(800, 600);

  // Initialize the event loop and then start it.
	iren->Initialize();
	renWin->SetWindowName( "Simple Volume Renderer" );
	renWin->Render();
	iren->Start(); 

	return 0;
}
