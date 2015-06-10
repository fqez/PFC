#include "gui.h"
#include "glWidget.h"
#include <QLayout>
#include <cv.h>
#include <iostream>
#include <cv.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/wait.h>
#include <pthread.h>


Shared* sm;
bool reset_search;
using namespace std;

int getdir (string dir, vector<string> &files)
{

    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

Gui::Gui(Shared* shm)
{

	sm = shm;
	reset_search = true;

	string dir = string("/home/fran/.gazebo/models");
	vector<string> files = vector<string>();


 	if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("this"));
    this->resize(364, 690);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->setMouseTracking(false);
    actionOpen = new QAction(this);
    actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
    actionClose = new QAction(this);
    actionClose->setObjectName(QString::fromUtf8("actionClose"));
    actionFile = new QAction(this);
    actionFile->setObjectName(QString::fromUtf8("actionFile"));
    actionObject = new QAction(this);
    actionObject->setObjectName(QString::fromUtf8("actionObject"));
    actionSalir = new QAction(this);
    actionSalir->setObjectName(QString::fromUtf8("actionSalir"));
    actionCargar_movimiento = new QAction(this);
    actionCargar_movimiento->setObjectName(QString::fromUtf8("actionCargar_movimiento"));
    actionCargar_Par_metros = new QAction(this);
    actionCargar_Par_metros->setObjectName(QString::fromUtf8("actionCargar_Par_metros"));
    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    horizontalLayout = new QHBoxLayout(centralwidget);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    tabWidget = new QTabWidget(centralwidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    basicTab = new QWidget();
    basicTab->setObjectName(QString::fromUtf8("basicTab"));
    verticalLayout_2 = new QVBoxLayout(basicTab);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    models = new QGroupBox(basicTab);
    models->setObjectName(QString::fromUtf8("models"));
    verticalLayout = new QVBoxLayout(models);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    modelosList = new QListWidget(models);
    modelosList->setObjectName(QString::fromUtf8("modelosList"));
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(modelosList->sizePolicy().hasHeightForWidth());
    modelosList->setSizePolicy(sizePolicy1);

	getdir(dir,files);
	std::sort(files.begin(), files.end());

	for (unsigned int i = 0;i < files.size();i++) {
		if (files[i].compare(".") == 0 || files[i].compare("..") == 0)
			continue;
		modelosList->addItem(QString::fromUtf8(files[i].c_str()));
	}

    verticalLayout->addWidget(modelosList);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer);

    buttonCargar = new QPushButton(models);
    buttonCargar->setObjectName(QString::fromUtf8("buttonCargar"));

    horizontalLayout_2->addWidget(buttonCargar);

    buttonEliminar = new QPushButton(models);
    buttonEliminar->setObjectName(QString::fromUtf8("buttonEliminar"));

    horizontalLayout_2->addWidget(buttonEliminar);


    verticalLayout->addLayout(horizontalLayout_2);


    verticalLayout_2->addWidget(models);

    simulation = new QGroupBox(basicTab);
    simulation->setObjectName(QString::fromUtf8("simulation"));
    simulation->setEnabled(true);
    verticalLayout_6 = new QVBoxLayout(simulation);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    gridLayout_8 = new QGridLayout();
    gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
    playButton = new QPushButton(simulation);
    playButton->setObjectName(QString::fromUtf8("playButton"));
    playButton->setMouseTracking(false);
    QIcon icon;
    icon.addFile(QString::fromUtf8("icons/play.png"), QSize(), QIcon::Normal, QIcon::Off);
    playButton->setIcon(icon);
    playButton->setIconSize(QSize(32, 32));
    playButton->setFlat(false);

    gridLayout_8->addWidget(playButton, 1, 0, 1, 3);

    cronometro = new LCDNumber();
	cronometro->setObjectName(QString::fromUtf8("cronometro"));
	cronometro->setSmallDecimalPoint(false);
	cronometro->setNumDigits(9);
	cronometro->setSegmentStyle(QLCDNumber::Flat);
	cronometro->setProperty("value", QVariant(0));
	cronometro->display("00:00.000");
	cronometro->timer->setInterval(1);

	gridLayout_8->addWidget(cronometro, 0, 0, 1, 6);

    reiniciarButton = new QPushButton(simulation);
    reiniciarButton->setObjectName(QString::fromUtf8("reiniciarButton"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8("icons/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
    reiniciarButton->setIcon(icon1);
    reiniciarButton->setIconSize(QSize(16, 16));

    gridLayout_8->addWidget(reiniciarButton, 1, 5, 1, 1);

    pausaButton = new QPushButton(simulation);
    pausaButton->setObjectName(QString::fromUtf8("pausaButton"));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8("icons/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
    pausaButton->setIcon(icon2);

    gridLayout_8->addWidget(pausaButton, 1, 4, 1, 1);

    stopButton = new QPushButton(simulation);
    stopButton->setObjectName(QString::fromUtf8("stopButton"));
    QIcon icon3;
    icon3.addFile(QString::fromUtf8("icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
    stopButton->setIcon(icon3);

    gridLayout_8->addWidget(stopButton, 1, 3, 1, 1);


    verticalLayout_6->addLayout(gridLayout_8);

    gridLayout_10 = new QGridLayout();
    gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
    tiempoValLabel = new QLabel(simulation);
    tiempoValLabel->setObjectName(QString::fromUtf8("tiempoValLabel"));

    gridLayout_10->addWidget(tiempoValLabel, 1, 1, 1, 1);

    distanciaLabel = new QLabel(simulation);
    distanciaLabel->setObjectName(QString::fromUtf8("distanciaLabel"));

    gridLayout_10->addWidget(distanciaLabel, 0, 0, 1, 1);

    distanciaValLabel = new QLabel(simulation);
    distanciaValLabel->setObjectName(QString::fromUtf8("distanciaValLabel"));

    gridLayout_10->addWidget(distanciaValLabel, 0, 1, 1, 1);

    desviacionValLabel = new QLabel(simulation);
    desviacionValLabel->setObjectName(QString::fromUtf8("desviacionValLabel"));

    gridLayout_10->addWidget(desviacionValLabel, 2, 1, 1, 1);

    desviacionLabel = new QLabel(simulation);
    desviacionLabel->setObjectName(QString::fromUtf8("desviacionLabel"));

    gridLayout_10->addWidget(desviacionLabel, 2, 0, 1, 1);

    pushButton_5 = new QPushButton(simulation);
    pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

    gridLayout_10->addWidget(pushButton_5, 4, 1, 1, 1);

    label = new QLabel(simulation);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout_10->addWidget(label, 3, 0, 1, 1);

    label_2 = new QLabel(simulation);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setTextFormat(Qt::AutoText);

    gridLayout_10->addWidget(label_2, 3, 1, 1, 1);

    timepoLabel = new QLabel(simulation);
    timepoLabel->setObjectName(QString::fromUtf8("timepoLabel"));

    gridLayout_10->addWidget(timepoLabel, 1, 0, 1, 1);


    verticalLayout_6->addLayout(gridLayout_10);


    verticalLayout_2->addWidget(simulation);

    gazebo = new QGridLayout();
    gazebo->setObjectName(QString::fromUtf8("gazebo"));
    buttonAbrirGazebo = new QPushButton(basicTab);
    buttonAbrirGazebo->setObjectName(QString::fromUtf8("buttonAbrirGazebo"));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8("icons/gazebo.png"), QSize(), QIcon::Normal, QIcon::Off);
    buttonAbrirGazebo->setIcon(icon4);

    gazebo->addWidget(buttonAbrirGazebo, 0, 0, 1, 1);

    buttonCerrarGazebo = new QPushButton(basicTab);
    buttonCerrarGazebo->setObjectName(QString::fromUtf8("buttonCerrarGazebo"));

    gazebo->addWidget(buttonCerrarGazebo, 1, 0, 1, 1);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gazebo->addItem(horizontalSpacer_2, 0, 1, 1, 1);


    verticalLayout_2->addLayout(gazebo);

    tabWidget->addTab(basicTab, QString());
    advancedTab = new QWidget();
    advancedTab->setObjectName(QString::fromUtf8("advancedTab"));
    tabWidget->addTab(advancedTab, QString());

    horizontalLayout->addWidget(tabWidget);

    this->setCentralWidget(centralwidget);
    menubar = new QMenuBar(this);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 364, 25));
    menubar->setNativeMenuBar(false);
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuMovimientos = new QMenu(menubar);
    menuMovimientos->setObjectName(QString::fromUtf8("menuMovimientos"));
    menuSimulacion = new QMenu(menubar);
    menuSimulacion->setObjectName(QString::fromUtf8("menuSimulacion"));
    this->setMenuBar(menubar);
    statusbar = new QStatusBar(this);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    this->setStatusBar(statusbar);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(menuMovimientos->menuAction());
    menubar->addAction(menuSimulacion->menuAction());
    menuFile->addAction(actionSalir);
    menuMovimientos->addAction(actionCargar_movimiento);
    menuSimulacion->addAction(actionCargar_Par_metros);


    QObject::connect(actionSalir, SIGNAL(activated()), this, SLOT(close()));

    tabWidget->setCurrentIndex(0);



    this->setWindowTitle(QApplication::translate("this", "this", 0, QApplication::UnicodeUTF8));
    actionOpen->setText(QApplication::translate("this", "Open", 0, QApplication::UnicodeUTF8));
    actionClose->setText(QApplication::translate("this", "Close", 0, QApplication::UnicodeUTF8));
    actionFile->setText(QApplication::translate("this", "File", 0, QApplication::UnicodeUTF8));
    actionObject->setText(QApplication::translate("this", "Object", 0, QApplication::UnicodeUTF8));
    actionSalir->setText(QApplication::translate("this", "Salir", 0, QApplication::UnicodeUTF8));
    actionCargar_movimiento->setText(QApplication::translate("this", "Cargar movimiento", 0, QApplication::UnicodeUTF8));
    actionCargar_Par_metros->setText(QApplication::translate("this", "Cargar Par\303\241metros", 0, QApplication::UnicodeUTF8));
    models->setTitle(QApplication::translate("this", "Modelos", 0, QApplication::UnicodeUTF8));
    buttonCargar->setText(QApplication::translate("this", "Cargar", 0, QApplication::UnicodeUTF8));
    buttonEliminar->setText(QApplication::translate("this", "Eliminar", 0, QApplication::UnicodeUTF8));
    simulation->setTitle(QApplication::translate("this", "Simulacion", 0, QApplication::UnicodeUTF8));
    playButton->setText(QString());
    reiniciarButton->setText(QApplication::translate("this", "Reiniciar", 0, QApplication::UnicodeUTF8));
    pausaButton->setText(QString());
    stopButton->setText(QString());
    tiempoValLabel->setText(QApplication::translate("this", "--", 0, QApplication::UnicodeUTF8));
    distanciaLabel->setText(QApplication::translate("this", "Distancia:", 0, QApplication::UnicodeUTF8));
    distanciaValLabel->setText(QApplication::translate("this", "--", 0, QApplication::UnicodeUTF8));
    desviacionValLabel->setText(QApplication::translate("this", "--", 0, QApplication::UnicodeUTF8));
    desviacionLabel->setText(QApplication::translate("this", "Desviaci\303\263n:", 0, QApplication::UnicodeUTF8));
    pushButton_5->setText(QApplication::translate("this", "Detalles", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("this", "Test completados:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("this", "--", 0, QApplication::UnicodeUTF8));
    timepoLabel->setText(QApplication::translate("this", "Tiempo:", 0, QApplication::UnicodeUTF8));
    buttonAbrirGazebo->setText(QApplication::translate("this", "Abrir Gazebo", 0, QApplication::UnicodeUTF8));
    buttonCerrarGazebo->setText(QApplication::translate("this", "Cerrar Gazebo", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(basicTab), QApplication::translate("this", "B\303\241sico", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(advancedTab), QApplication::translate("this", "Avanzado", 0, QApplication::UnicodeUTF8));
    menuFile->setTitle(QApplication::translate("this", "Archivo", 0, QApplication::UnicodeUTF8));
    menuMovimientos->setTitle(QApplication::translate("this", "Movimientos", 0, QApplication::UnicodeUTF8));
    menuSimulacion->setTitle(QApplication::translate("this", "Simulacion", 0, QApplication::UnicodeUTF8));

	QObject::connect(buttonAbrirGazebo, SIGNAL(pressed()), this, SLOT(on_abrir_buttonpressed()));
	QObject::connect(buttonCerrarGazebo, SIGNAL(pressed()), this, SLOT(on_cerrar_buttonpressed()));
	QObject::connect(buttonCargar, SIGNAL(pressed()), this, SLOT(on_cargar_buttonpressed()));
	QObject::connect(buttonEliminar, SIGNAL(pressed()), this, SLOT(on_eliminar_buttonpressed()));
	QObject::connect(playButton, SIGNAL(pressed()), this, SLOT(on_play_buttonpressed()));
	QObject::connect(pausaButton, SIGNAL(pressed()), this, SLOT(on_pausa_buttonpressed()));
	QObject::connect(stopButton, SIGNAL(pressed()), this, SLOT(on_stop_buttonpressed()));
	QObject::connect(reiniciarButton, SIGNAL(pressed()), this, SLOT(on_reiniciar_buttonpressed()));

    show();

}

void clean_myfile() {
    std::remove("tmp/nao.world");
}

int Gui::crearFichero(std::string a) {

    std::ofstream fs("tmp/nao.world"); 
    std::atexit(clean_myfile);

    if(!fs)
    {
        std::cerr<<"Cannot open the output file."<<std::endl;
        return 1;
    }
    fs<<"<?xml version=\"1.0\" ?>\n";
    fs<<"<sdf version=\"1.3\">\n";
    fs<<"  <world name=\"default\">\n";
    fs<<"    <!-- Ground -->\n";
    fs<<"    <include>\n";
    fs<<"      <uri>model://ground_plane</uri>\n";
    fs<<"    </include>\n";
    fs<<"    <!-- Robot -->\n";
    fs<<"    <include>\n";
    fs<<"      <uri>model://" << a << "</uri>\n";
    fs<<"    </include>\n";
    fs<<"    <!-- A global light source -->\n";
    fs<<"    <include>\n";
    fs<<"      <uri>model://sun</uri>\n";
    fs<<"    </include>\n";
	fs<<"    <light type=\"point\" name=\"point_light_front\">\n";
	fs<<"      <pose>1 0 1 0 0 0</pose>\n";
	fs<<"      <diffuse>1 1 1 1</diffuse>\n";
	fs<<"      <specular>.1 .1 .1 1</specular>\n";
	fs<<"      <attenuation>\n";
	fs<<"        <range>20</range>\n";
	fs<<"        <linear>0.2</linear>\n";
	fs<<"        <constant>0.8</constant>\n";
	fs<<"        <quadratic>0.01</quadratic>\n";
	fs<<"      </attenuation>\n";
	fs<<"      <cast_shadows>false</cast_shadows>\n";
	fs<<"    </light>\n";
	fs<<"    <light type=\"point\" name=\"point_light_back\">\n";
	fs<<"      <pose>-1 0 1 0 0 0</pose>\n";
	fs<<"      <diffuse>1 1 1 1</diffuse>\n";
	fs<<"      <specular>.1 .1 .1 1</specular>\n";
	fs<<"      <attenuation>\n";
	fs<<"        <range>20</range>\n";
	fs<<"        <linear>0.2</linear>\n";
	fs<<"        <constant>0.8</constant>\n";
	fs<<"        <quadratic>0.01</quadratic>\n";
	fs<<"      </attenuation>\n";
	fs<<"      <cast_shadows>false</cast_shadows>\n";
	fs<<"    </light>\n";
    fs<<"  </world>\n";
    fs<<"</sdf>";
    fs.close();
    return 0;

}


void Gui::updateThreadGui(){

}

void* gazeboThread(void*) {

	string fileName = "tmp/nao.world";
    std::string command;

//MEJORAR: EN LUGAR DE EJECUTAR GAZEBO, EJECUTAR GZSERVER Y ABRIR Y CERRAR EL GUI CON GZCLIENT CON EL BOTON DEL GUI
//SERÁ NECESARIO UN HILO PARA MOSTRAR LA INTERFAZ.

	//si la opcion de interfaz de gazebo esta activada
	if (true)
		 command = "gazebo "+fileName;
	else
		//si no queremos interfaz de gazebo
		command = "gzserver "+fileName;
    int a = system(command.c_str());	//manejo de excepciones

	std::cout << a << std::endl;
    
}

void Gui::on_abrir_buttonpressed() {

	buttonCerrarGazebo->setEnabled(true);
	models->setEnabled(false);

	try {
		pthread_t t_gazebo;

		buttonAbrirGazebo->setEnabled(false);
		pthread_create(&t_gazebo, NULL, &gazeboThread, NULL);

		//letting gazebo start
		sleep(6000);
		sm->connectGazebo(true);

	}catch(const char* msg) {
		std::cerr << msg << std::endl;
        exit(-1);
	}	
  
    std::cout << "presionado abrir gazebo" << std::endl;
}
void Gui::on_cerrar_buttonpressed(){

	buttonAbrirGazebo->setEnabled(true);
	models->setEnabled(true);

	system("killall gzserver > /dev/null");

	std::cout << "presionado cerrar gazebo" << std::endl;
}
void Gui::on_cargar_buttonpressed(){

    std::string s = this->modelosList->currentItem()->text().toUtf8().constData();
	buttonAbrirGazebo->setEnabled(true);
	crearFichero(s);

	std::cout << "presionado cargar "<< s << std::endl;
}
void Gui::on_eliminar_buttonpressed(){
	std::cout << "presionado eliminar" << std::endl;
}


void* search(void*) {
		
	RakedSearch* ss = new RakedSearch(sm);
	ss->initialize();
	ss->searchAll(reset_search);

	std::cout << "FIN" << std::endl;

	return 0;
}

void Gui::on_play_buttonpressed(){

	//Comenzar las simulaciones.
	sm->setSimState(0);	//START
	sm->setSearchState(0);	//STOP

	cronometro->start();

	pthread_t search_t;
	pthread_create(&search_t, NULL, &search, NULL);
   
	std::cout << "presionado play " << std::endl;
}
void Gui::on_pausa_buttonpressed(){

	//Pausar la simulacion actual
	sm->setSimState(1);	//PAUSE
	sm->setSearchState(1);	//STOP
	reset_search = false;
	cronometro->pause();


	std::cout << "presionado pausa" << std::endl;
}
void Gui::on_stop_buttonpressed(){

	//Detener simulación
	sm->setSimState(2);	//STOP
	sm->setSearchState(2);	//STOP
	reset_search = false;
	//contador simulaciones a cero
	cronometro->stop();

	std::string tiempo = cronometro->getTime();
	
	tiempoValLabel->setText(QString::fromStdString(tiempo));	
	std::cout << "presionado stop " << tiempo << std::endl;
}

void Gui::on_reiniciar_buttonpressed(){

	//Limpiar historial de tests
	std::cout << "presionado reiniciar" << std::endl;
}



void Gui::updateAll() {
	
    window.expose();

}

void Gui::sleep(unsigned long milisec) {
		
	//unsigned long milisec = 3000L;
	struct timespec req={0};
   	time_t sec=(int)(milisec/1000);
	milisec=milisec-(sec*1000);
	req.tv_sec=sec;
	req.tv_nsec=milisec*1000000L;
	while(nanosleep(&req,&req)==-1)
    	continue;
}

Gui::~Gui(){}
