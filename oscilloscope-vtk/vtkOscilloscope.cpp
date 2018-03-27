#include "vtkOscilloscope.h"

vtkSmartPointer<vtkProgrammableDataObjectSource> VtkOscillosope::m_pDOS = vtkSmartPointer<vtkProgrammableDataObjectSource>::New();
vtkSmartPointer<vtkRenderer> VtkOscillosope::m_renderer = vtkSmartPointer<vtkRenderer>::New();
vtkSmartPointer<vtkPoints> VtkOscillosope::m_points = vtkSmartPointer<vtkPoints>::New();
vtkSmartPointer<vtkPolyLine> VtkOscillosope::m_polyLine = vtkSmartPointer<vtkPolyLine>::New();
int VtkOscillosope::m_numOfPoints = 0;
double VtkOscillosope::m_t = 0;
int VtkOscillosope::counter2 = 0;

VtkOscillosope::VtkOscillosope()
{
}

void VtkOscillosope::run()
{
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow >::New();

	vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	rwi->SetRenderWindow(renderWindow);


	m_pDOS->SetExecuteMethod(&VtkOscillosope::generateEllipse, m_pDOS);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(m_pDOS->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetDiffuseColor(255, 255, 0);

	renderWindow->AddRenderer(m_renderer);
	m_renderer->AddActor(actor);
	m_renderer->ResetCamera();

	renderWindow->Render();

	// Add Timer Event...
	vtkSmartPointer<vtkCallbackCommand> timerCallback = vtkSmartPointer<vtkCallbackCommand>::New();
	timerCallback->SetCallback(VtkOscillosope::TimerCallbackFunction);

	rwi->Initialize();
	rwi->CreateRepeatingTimer(20);
	rwi->AddObserver(vtkCommand::TimerEvent, timerCallback);


	// Start Displaying...
	rwi->Start();
}

void VtkOscillosope::NextPoint()
{
	double y = m_t * cos(m_t);
	double z = m_t * sin(m_t);
	m_points->InsertNextPoint(m_t, y, z);
	m_polyLine->GetPointIds()->InsertNextId(m_numOfPoints);
	m_numOfPoints++;
	m_t += 0.1;
}

void VtkOscillosope::generateEllipse(void * caller)
{
	vtkProgrammableDataObjectSource *pDOS = vtkProgrammableDataObjectSource::SafeDownCast((vtkObjectBase*)caller);
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(m_polyLine);

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(m_points);
	polyData->SetLines(cells);

	m_pDOS->SetOutput(polyData);
}

void VtkOscillosope::TimerCallbackFunction(vtkObject * caller, long unsigned int vtkNotUsed, void * clientData, void * vtkNotUsed(callData))
{
	std::cout << "timer call back: " << counter2 << std::endl;
	vtkRenderWindowInteractor *rwi = static_cast<vtkRenderWindowInteractor*>(caller);

	NextPoint();

	m_pDOS->Modified();
	rwi->Render();
	m_renderer->ResetCamera(); // Optional: Reposition Camera, so it displays the whole object

	counter2++;
}
