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
public:
	explicit VtkOscillosope();
	void run();
	void initVtk();
	void addTimerEventCallback();
	static void NextPoint();
	static void generateEllipse(void *caller);
	static void generateAxisX(void *caller);
	static void generateAxisY(void *caller);
	static void generateAxisZ(void *caller);
	static void TimerCallbackFunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* clientData, void* vtkNotUsed(callData));

	static vtkSmartPointer<vtkProgrammableDataObjectSource> m_pDOS;
	static vtkSmartPointer<vtkProgrammableDataObjectSource> VtkOscillosope::m_pDOSX;
	static vtkSmartPointer<vtkProgrammableDataObjectSource> VtkOscillosope::m_pDOSY;
	static vtkSmartPointer<vtkProgrammableDataObjectSource> VtkOscillosope::m_pDOSZ;

	static vtkSmartPointer<vtkRenderer> m_renderer;
	
	static vtkSmartPointer<vtkPoints> m_points;
	static vtkSmartPointer<vtkPolyLine> m_polyLine;

	static vtkSmartPointer<vtkPoints> m_pointsX;
	static vtkSmartPointer<vtkPolyLine> m_polyLineX;

	static vtkSmartPointer<vtkPoints> m_pointsY;
	static vtkSmartPointer<vtkPolyLine> m_polyLineY;

	static vtkSmartPointer<vtkPoints> m_pointsZ;
	static vtkSmartPointer<vtkPolyLine> m_polyLineZ;

	static int m_numOfPoints;
	static double m_t;
	static int counter2;
	static double m_xRange[2];
	static double m_yRange[2];
	static double m_zRange[2];
	static bool rangeChange;
	static bool firstFlag;
private:
	//vtkSmartPointer<vtkActor> m_actorData = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkRenderWindow> m_renderWindow = vtkSmartPointer<vtkRenderWindow >::New();
	vtkSmartPointer<vtkRenderWindowInteractor> m_rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkPolyDataMapper> m_mapperData = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> m_mapperAxisX = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> m_mapperAxisY = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> m_mapperAxisZ = vtkSmartPointer<vtkPolyDataMapper>::New();
};

