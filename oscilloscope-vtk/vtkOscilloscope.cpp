#include "vtkOscilloscope.h"

vtkSmartPointer<vtkProgrammableDataObjectSource> VtkOscillosope::m_pDOS = vtkSmartPointer<vtkProgrammableDataObjectSource>::New();
vtkSmartPointer<vtkProgrammableDataObjectSource> VtkOscillosope::m_pDOSX = vtkSmartPointer<vtkProgrammableDataObjectSource>::New();
vtkSmartPointer<vtkProgrammableDataObjectSource> VtkOscillosope::m_pDOSY = vtkSmartPointer<vtkProgrammableDataObjectSource>::New();
vtkSmartPointer<vtkProgrammableDataObjectSource> VtkOscillosope::m_pDOSZ = vtkSmartPointer<vtkProgrammableDataObjectSource>::New();
vtkSmartPointer<vtkRenderer> VtkOscillosope::m_renderer = vtkSmartPointer<vtkRenderer>::New();
vtkSmartPointer<vtkPoints> VtkOscillosope::m_points = vtkSmartPointer<vtkPoints>::New();
vtkSmartPointer<vtkPolyLine> VtkOscillosope::m_polyLine = vtkSmartPointer<vtkPolyLine>::New();
vtkSmartPointer<vtkPoints> VtkOscillosope::m_pointsX = vtkSmartPointer<vtkPoints>::New();
vtkSmartPointer<vtkPolyLine> VtkOscillosope::m_polyLineX = vtkSmartPointer<vtkPolyLine>::New();
vtkSmartPointer<vtkPoints> VtkOscillosope::m_pointsY = vtkSmartPointer<vtkPoints>::New();
vtkSmartPointer<vtkPolyLine> VtkOscillosope::m_polyLineY = vtkSmartPointer<vtkPolyLine>::New();
vtkSmartPointer<vtkPoints> VtkOscillosope::m_pointsZ = vtkSmartPointer<vtkPoints>::New();
vtkSmartPointer<vtkPolyLine> VtkOscillosope::m_polyLineZ = vtkSmartPointer<vtkPolyLine>::New();
double VtkOscillosope::m_xRange[2] = { 0, 1 };
double VtkOscillosope::m_yRange[2] = { 0, 1 };
double VtkOscillosope::m_zRange[2] = { 0, 1 };
int VtkOscillosope::m_numOfPoints = 0;
double VtkOscillosope::m_t = 0;
int VtkOscillosope::counter2 = 0;
bool VtkOscillosope::rangeChange = false;
bool VtkOscillosope::firstFlag = true;

VtkOscillosope::VtkOscillosope()
{
}

void VtkOscillosope::run()
{
	this->initVtk();

	m_renderWindow->Render();

	this->addTimerEventCallback();

	// Start Displaying...
	m_rwi->Start();
}

void VtkOscillosope::initVtk()
{
	m_renderWindow = vtkSmartPointer<vtkRenderWindow >::New();

	m_rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_rwi->SetRenderWindow(m_renderWindow);

	m_pDOS->SetExecuteMethod(&VtkOscillosope::generateEllipse, m_pDOS);

	m_pDOSX->SetExecuteMethod(&VtkOscillosope::generateAxisX, m_pDOSX);

	m_pDOSY->SetExecuteMethod(&VtkOscillosope::generateAxisY, m_pDOSY);

	m_pDOSZ->SetExecuteMethod(&VtkOscillosope::generateAxisZ, m_pDOSZ);

	m_mapperData = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_mapperData->SetInputConnection(m_pDOS->GetOutputPort());

	m_mapperAxisX = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_mapperAxisX->SetInputConnection(m_pDOSX->GetOutputPort());

	m_mapperAxisY = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_mapperAxisY->SetInputConnection(m_pDOSY->GetOutputPort());

	m_mapperAxisZ = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_mapperAxisZ->SetInputConnection(m_pDOSZ->GetOutputPort());

	vtkSmartPointer<vtkActor> m_actorData = vtkSmartPointer<vtkActor>::New();
	m_actorData->SetMapper(m_mapperData);
	m_actorData->GetProperty()->SetDiffuseColor(255, 255, 0);

	vtkSmartPointer<vtkActor> m_axisX = vtkSmartPointer<vtkActor>::New();
	m_axisX->SetMapper(m_mapperAxisX);
	m_axisX->GetProperty()->SetDiffuseColor(255, 0, 0);

	vtkSmartPointer<vtkActor> m_axisY = vtkSmartPointer<vtkActor>::New();
	m_axisY->SetMapper(m_mapperAxisY);
	m_axisY->GetProperty()->SetDiffuseColor(0, 255, 0);

	vtkSmartPointer<vtkActor> m_axisZ = vtkSmartPointer<vtkActor>::New();
	m_axisZ->SetMapper(m_mapperAxisZ);
	m_axisZ->GetProperty()->SetDiffuseColor(0, 0, 255);

	m_renderWindow->AddRenderer(m_renderer);
	m_renderer->AddActor(m_actorData);
	m_renderer->AddActor(m_axisX);
	m_renderer->AddActor(m_axisY);
	m_renderer->AddActor(m_axisZ);

	m_renderer->ResetCamera();
	m_renderer->SetBackground(1.0f, 1.0f, 1.0f);
}

void VtkOscillosope::addTimerEventCallback()
{
	// Add Timer Event...
	vtkSmartPointer<vtkCallbackCommand> timerCallback = vtkSmartPointer<vtkCallbackCommand>::New();
	timerCallback->SetCallback(VtkOscillosope::TimerCallbackFunction);

	m_rwi->Initialize();
	m_rwi->CreateRepeatingTimer(20);
	m_rwi->AddObserver(vtkCommand::TimerEvent, timerCallback);
}

void VtkOscillosope::NextPoint()
{
	double y = m_t * cos(m_t);
	double z = m_t * sin(m_t);
	double x = m_t;
	m_points->InsertNextPoint(x, y, z);
	m_polyLine->GetPointIds()->InsertNextId(m_numOfPoints);
	if (firstFlag)
	{
		rangeChange = true;
		firstFlag = false;
	}
	else
		rangeChange = false;

	if (x > m_xRange[1])
	{
		m_xRange[1] = x + 0.5 * fabs(x);
		rangeChange = true;
	}
	else if(x < m_xRange[0])
	{
		m_xRange[0] = x - 0.5 * fabs(x);
		rangeChange = true;
	}

	if (y > m_yRange[1])
	{
		m_yRange[1] = y + 0.5 * fabs(y);
		rangeChange = true;
	}
	else if (y < m_yRange[0])
	{
		m_yRange[0] = y - 0.5 * fabs(y);
		rangeChange = true;
	}

	if (z > m_zRange[1])
	{
		m_zRange[1] = z + 0.5 * fabs(z);
		rangeChange = true;
	}
	else if (z < m_zRange[0])
	{
		m_zRange[0] = z - 0.5 * fabs(z);
		rangeChange = true;
	}

	double arrowbase[2] = { 0.01f, 0.05f };
	if (rangeChange)
	{	
		m_pointsX->Initialize();
		m_pointsY->Initialize();
		m_pointsZ->Initialize();
		m_polyLineX->Initialize();
		m_polyLineY->Initialize();
		m_polyLineZ->Initialize();

		vtkIdType pid = m_pointsX->InsertNextPoint(m_xRange[0], m_yRange[0], m_zRange[0]);
		m_polyLineX->GetPointIds()->InsertNextId(pid);
		pid = m_pointsX->InsertNextPoint(m_xRange[1], m_yRange[0], m_zRange[0]);
		m_polyLineX->GetPointIds()->InsertNextId(pid);

		pid = m_pointsY->InsertNextPoint(m_xRange[0], m_yRange[0], m_zRange[0]);
		m_polyLineY->GetPointIds()->InsertNextId(pid);
		pid = m_pointsY->InsertNextPoint(m_xRange[0], m_yRange[1], m_zRange[0]);
		m_polyLineY->GetPointIds()->InsertNextId(pid);

		pid = m_pointsZ->InsertNextPoint(m_xRange[0], m_yRange[0], m_zRange[0]);
		m_polyLineZ->GetPointIds()->InsertNextId(pid);
		pid = m_pointsZ->InsertNextPoint(m_xRange[0], m_yRange[0], m_zRange[1]);
		m_polyLineZ->GetPointIds()->InsertNextId(pid);
		/*int anglenum = 20;
		float pi = 3.1415926;
		for (int i = 0; i < anglenum; ++i)
		{
			if (i < (anglenum - 1))
			{
				m_polyLineX->GetPointIds()->InsertNextId(0);
				m_polyLineX->GetPointIds()->InsertNextId(i + 1);
				m_polyLineX->GetPointIds()->InsertNextId(i + 2);
			}
			else
			{
				m_polyLineX->GetPointIds()->InsertNextId(0);
				m_polyLineX->GetPointIds()->InsertNextId(i + 1);
				m_polyLineX->GetPointIds()->InsertNextId(1);
			}
		}
		m_pointsX->InsertNextPoint(m_xRange[1], m_yRange[0], m_zRange[0]);
		for (int i = 0; i < anglenum; ++i)
		{
			m_pointsX->InsertNextPoint(m_xRange[1] - arrowbase[1], m_yRange[0] + arrowbase[0] * cos(i * 2 * pi / anglenum), m_zRange[0] + arrowbase[0] * sin(i * 2 * pi / anglenum));
		}*/
		

	}

	//m_pointsX->InsertNextPoint(m_t, y+1, z);
	//m_polyLineX->GetPointIds()->InsertNextId(m_numOfPoints);

	//m_pointsY->InsertNextPoint(m_t, y+2, z);
	//m_polyLineY->GetPointIds()->InsertNextId(m_numOfPoints);

	//m_pointsZ->InsertNextPoint(m_t, y+3, z);
	//m_polyLineZ->GetPointIds()->InsertNextId(m_numOfPoints);
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

	pDOS->SetOutput(polyData);
}

void VtkOscillosope::generateAxisX(void * caller)
{
	vtkProgrammableDataObjectSource *pDOS = vtkProgrammableDataObjectSource::SafeDownCast((vtkObjectBase*)caller);
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(m_polyLineX);

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(m_pointsX);
	polyData->SetLines(cells);

	pDOS->SetOutput(polyData);
}

void VtkOscillosope::generateAxisY(void * caller)
{
	vtkProgrammableDataObjectSource *pDOS = vtkProgrammableDataObjectSource::SafeDownCast((vtkObjectBase*)caller);
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(m_polyLineY);

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(m_pointsY);
	polyData->SetLines(cells);

	pDOS->SetOutput(polyData);
}

void VtkOscillosope::generateAxisZ(void * caller)
{
	vtkProgrammableDataObjectSource *pDOS = vtkProgrammableDataObjectSource::SafeDownCast((vtkObjectBase*)caller);
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(m_polyLineZ);

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(m_pointsZ);
	polyData->SetLines(cells);

	pDOS->SetOutput(polyData);
}

void VtkOscillosope::TimerCallbackFunction(vtkObject * caller, long unsigned int vtkNotUsed, void * clientData, void * vtkNotUsed(callData))
{
	std::cout << "timer call back: " << counter2 << std::endl;
	vtkRenderWindowInteractor *rwi = static_cast<vtkRenderWindowInteractor*>(caller);

	NextPoint();

	m_pDOS->Modified();
	m_pDOSX->Modified();
	m_pDOSY->Modified();
	m_pDOSZ->Modified();
	rwi->Render();
	m_renderer->ResetCamera(); // Optional: Reposition Camera, so it displays the whole object

	counter2++;
}
