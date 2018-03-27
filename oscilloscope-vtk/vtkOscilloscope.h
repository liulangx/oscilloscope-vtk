#pragma once
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
//VTK_MODULE_INIT(vtkRenderingContextOpenGL2);
//VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
//VTK_MODULE_INIT(vtkRenderingFreeType);

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkProgrammableFilter.h>
#include <vtkCallbackCommand.h>
#include <vtkPolyDataStreamer.h>
#include <vtkProgrammableDataObjectSource.h>

class VtkOscillosope
{
private:
	
public:
	explicit VtkOscillosope();
	void run();
	static void NextPoint();
	static void generateEllipse(void *caller);
	static void TimerCallbackFunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* clientData, void* vtkNotUsed(callData));

	static vtkSmartPointer<vtkProgrammableDataObjectSource> m_pDOS;
	static vtkSmartPointer<vtkRenderer> m_renderer;
	
	static vtkSmartPointer<vtkPoints> m_points;
	static vtkSmartPointer<vtkPolyLine> m_polyLine;
	static int m_numOfPoints;
	static double m_t;
	static int counter2;
};

