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


Gui::Gui(Shared* sm)
{


	string dir = string("/home/fran/.gazebo/models");
	vector<string> files = vector<string>();


    this->sm = sm;

    if (this->objectName().isEmpty())
    	this->setObjectName(QString::fromUtf8("this"));
	this->setWindowModality(Qt::NonModal);
	this->resize(310, 608);
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	this->setSizePolicy(sizePolicy);
	gridLayoutWidget = new QWidget(this);
	gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
	gridLayoutWidget->setGeometry(QRect(10, 530, 136, 62));
	gridGazebo = new QGridLayout(gridLayoutWidget);
	gridGazebo->setObjectName(QString::fromUtf8("gridGazebo"));
    gridGazebo->setContentsMargins(0, 0, 0, 0);
    abrirGazebo = new QPushButton(gridLayoutWidget);
    abrirGazebo->setObjectName(QString::fromUtf8("abrirGazebo"));
    QIcon icon;
    icon.addFile(QString::fromUtf8("icons/gazebo.png"), QSize(), QIcon::Normal, QIcon::Off);
    abrirGazebo->setIcon(icon);
	abrirGazebo->setEnabled(false);

    gridGazebo->addWidget(abrirGazebo, 2, 0, 1, 1);

    cerrarGazebo = new QPushButton(gridLayoutWidget);
    cerrarGazebo->setObjectName(QString::fromUtf8("cerrarGazebo"));
	cerrarGazebo->setEnabled(false);

    gridGazebo->addWidget(cerrarGazebo, 3, 0, 1, 1);

    modelos = new QGroupBox(this);
    modelos->setObjectName(QString::fromUtf8("modelos"));
    modelos->setEnabled(true);
    modelos->setGeometry(QRect(10, 10, 311, 321));
    gridLayoutWidget_2 = new QWidget(modelos);
    gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
    gridLayoutWidget_2->setGeometry(QRect(0, 30, 291, 281));
    gridModelos = new QGridLayout(gridLayoutWidget_2);
    gridModelos->setObjectName(QString::fromUtf8("gridModelos"));
    gridModelos->setContentsMargins(0, 0, 0, 0);
    cargarModelo = new QPushButton(gridLayoutWidget_2);
    cargarModelo->setObjectName(QString::fromUtf8("cargarModelo"));

    gridModelos->addWidget(cargarModelo, 2, 1, 1, 1);

    listaModelos = new QListWidget(gridLayoutWidget_2);
    listaModelos->setObjectName(QString::fromUtf8("listaModelos"));
	
	getdir(dir,files);
	std::sort(files.begin(), files.end());

	for (unsigned int i = 0;i < files.size();i++) {
		if (files[i].compare(".") == 0 || files[i].compare("..") == 0)
			continue;		
		listaModelos->addItem(QString::fromUtf8(files[i].c_str()));
	}

    gridModelos->addWidget(listaModelos, 0, 0, 1, 2);

    eliminarModelo = new QPushButton(gridLayoutWidget_2);
    eliminarModelo->setObjectName(QString::fromUtf8("eliminarModelo"));

    gridModelos->addWidget(eliminarModelo, 3, 1, 1, 1);

    simulacion = new QGroupBox(this);
    simulacion->setObjectName(QString::fromUtf8("simulacion"));
    simulacion->setEnabled(true);
    simulacion->setGeometry(QRect(10, 330, 301, 191));
    gridLayoutWidget_3 = new QWidget(simulacion);
    gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
    gridLayoutWidget_3->setGeometry(QRect(0, 20, 296, 161));
    gridSimulacion = new QGridLayout(gridLayoutWidget_3);
    gridSimulacion->setObjectName(QString::fromUtf8("gridSimulacion"));
    gridSimulacion->setContentsMargins(0, 0, 0, 0);
    paso = new QPushButton(gridLayoutWidget_3);
    paso->setObjectName(QString::fromUtf8("paso"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8("icons/step.png"), QSize(), QIcon::Normal, QIcon::Off);
    paso->setIcon(icon1);

    gridSimulacion->addWidget(paso, 2, 4, 1, 1);

    stop = new QPushButton(gridLayoutWidget_3);
    stop->setObjectName(QString::fromUtf8("stop"));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8("icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
    stop->setIcon(icon2);

    gridSimulacion->addWidget(stop, 2, 0, 1, 1);

    play = new QPushButton(gridLayoutWidget_3);
    play->setObjectName(QString::fromUtf8("play"));
    QIcon icon3;
    icon3.addFile(QString::fromUtf8("icons/play.png"), QSize(), QIcon::Normal, QIcon::Off);
    play->setIcon(icon3);

    gridSimulacion->addWidget(play, 2, 1, 1, 2);

    reiniciar = new QPushButton(gridLayoutWidget_3);
    reiniciar->setObjectName(QString::fromUtf8("reiniciar"));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8("icons/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
    reiniciar->setIcon(icon4);
    reiniciar->setIconSize(QSize(32, 32));

    gridSimulacion->addWidget(reiniciar, 2, 5, 1, 1);

    pausa = new QPushButton(gridLayoutWidget_3);
    pausa->setObjectName(QString::fromUtf8("pausa"));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8("icons/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
    pausa->setIcon(icon5);

    gridSimulacion->addWidget(pausa, 2, 3, 1, 1);

    cronometro = new LCDNumber(gridLayoutWidget_3);
    cronometro->setObjectName(QString::fromUtf8("cronometro"));
    cronometro->setSmallDecimalPoint(false);
    cronometro->setNumDigits(9);
    cronometro->setSegmentStyle(QLCDNumber::Flat);
    cronometro->setProperty("value", QVariant(0));
    cronometro->display("00:00.000");
	cronometro->timer->setInterval(1);

    gridSimulacion->addWidget(cronometro, 1, 0, 1, 6);

    estadisticas = new QGridLayout();
    estadisticas->setObjectName(QString::fromUtf8("estadisticas"));
    desviacion = new QLabel(gridLayoutWidget_3);
    desviacion->setObjectName(QString::fromUtf8("desviacion"));

    estadisticas->addWidget(desviacion, 2, 0, 1, 1);

    distancia = new QLabel(gridLayoutWidget_3);
    distancia->setObjectName(QString::fromUtf8("distancia"));

    estadisticas->addWidget(distancia, 0, 0, 1, 1);

    tiempo = new QLabel(gridLayoutWidget_3);
    tiempo->setObjectName(QString::fromUtf8("tiempo"));

    estadisticas->addWidget(tiempo, 1, 0, 1, 1);

    valDistancia = new QLabel(gridLayoutWidget_3);
    valDistancia->setObjectName(QString::fromUtf8("valDistancia"));

    estadisticas->addWidget(valDistancia, 0, 1, 1, 1);

    valTiempo = new QLabel(gridLayoutWidget_3);
    valTiempo->setObjectName(QString::fromUtf8("valTiempo"));

    estadisticas->addWidget(valTiempo, 1, 1, 1, 1);

    valDesviacion = new QLabel(gridLayoutWidget_3);
    valDesviacion->setObjectName(QString::fromUtf8("valDesviacion"));

    estadisticas->addWidget(valDesviacion, 2, 1, 1, 1);


    gridSimulacion->addLayout(estadisticas, 3, 0, 1, 6);

    gridLayoutWidget_3->raise();
    QWidget::setTabOrder(abrirGazebo, cerrarGazebo);
    QWidget::setTabOrder(cerrarGazebo, stop);
    QWidget::setTabOrder(stop, play);
    QWidget::setTabOrder(play, pausa);
    QWidget::setTabOrder(pausa, paso);
    QWidget::setTabOrder(paso, reiniciar);

	this->setWindowTitle(QApplication::translate("this", "Nao Control", 0, QApplication::UnicodeUTF8));
    abrirGazebo->setText(QApplication::translate("this", "Abrir Gazebo", 0, QApplication::UnicodeUTF8));
    cerrarGazebo->setText(QApplication::translate("this", "Cerrar Gazebo", 0, QApplication::UnicodeUTF8));
    modelos->setTitle(QApplication::translate("this", "Modelos", 0, QApplication::UnicodeUTF8));
    cargarModelo->setText(QApplication::translate("this", "Cargar modelo", 0, QApplication::UnicodeUTF8));
    eliminarModelo->setText(QApplication::translate("this", "Eliminar modelo", 0, QApplication::UnicodeUTF8));
    simulacion->setTitle(QApplication::translate("this", "Simulacion", 0, QApplication::UnicodeUTF8));
    paso->setText(QString());
    stop->setText(QString());
    play->setText(QString());
    reiniciar->setText(QApplication::translate("this", "Reiniciar", 0, QApplication::UnicodeUTF8));
    pausa->setText(QString());
    desviacion->setText(QApplication::translate("this", "Desviacion:", 0, QApplication::UnicodeUTF8));
    distancia->setText(QApplication::translate("this", "Distancia:", 0, QApplication::UnicodeUTF8));
    tiempo->setText(QApplication::translate("this", "Tiempo:", 0, QApplication::UnicodeUTF8));
    valDistancia->setText(QApplication::translate("this", "0cm", 0, QApplication::UnicodeUTF8));
    valTiempo->setText(QApplication::translate("this", "0s", 0, QApplication::UnicodeUTF8));
    valDesviacion->setText(QApplication::translate("this", "0cm", 0, QApplication::UnicodeUTF8));

	QObject::connect(abrirGazebo, SIGNAL(pressed()), this, SLOT(on_abrir_buttonpressed()));
	QObject::connect(cerrarGazebo, SIGNAL(pressed()), this, SLOT(on_cerrar_buttonpressed()));
	QObject::connect(cargarModelo, SIGNAL(pressed()), this, SLOT(on_cargar_buttonpressed()));
	QObject::connect(eliminarModelo, SIGNAL(pressed()), this, SLOT(on_eliminar_buttonpressed()));
	QObject::connect(play, SIGNAL(pressed()), this, SLOT(on_play_buttonpressed()));
	QObject::connect(pausa, SIGNAL(pressed()), this, SLOT(on_pausa_buttonpressed()));
	QObject::connect(paso, SIGNAL(pressed()), this, SLOT(on_paso_buttonpressed()));
	QObject::connect(stop, SIGNAL(pressed()), this, SLOT(on_stop_buttonpressed()));
	QObject::connect(reiniciar, SIGNAL(pressed()), this, SLOT(on_reiniciar_buttonpressed()));


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

	std::string command = "gazebo "+fileName;
    int a = system(command.c_str());	//manejo de excepciones

	std::cout << a << std::endl;
    
}

void Gui::on_abrir_buttonpressed() {

	cerrarGazebo->setEnabled(true);

	try {
		pthread_t t_gazebo;

		abrirGazebo->setEnabled(false);
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

	abrirGazebo->setEnabled(true);
	modelos->setEnabled(true);

	system("killall gzserver");

	std::cout << "presionado cerrar gazebo" << std::endl;
}

void Gui::on_cargar_buttonpressed(){

    std::string s = this->listaModelos->currentItem()->text().toUtf8().constData();
	modelos->setEnabled(false);
	abrirGazebo->setEnabled(true);
	crearFichero(s);

	std::cout << "presionado cargar "<< s << std::endl;
}

void Gui::on_eliminar_buttonpressed(){
	
	delete this->listaModelos->currentItem();

	std::cout << "presionado eliminar" << std::endl;
}
void Gui::on_paso_buttonpressed(){
	std::cout << "presionado paso" << std::endl;
}
void Gui::on_pausa_buttonpressed(){

	cronometro->pause();
	std::cout << "presionado pausa" << std::endl;
}
void Gui::on_play_buttonpressed(){
	
	cronometro->start();
   
	std::cout << "presionado play " << std::endl;
}
void Gui::on_stop_buttonpressed(){
	cronometro->stop();

	std::string tiempo = cronometro->getTime();
	
	valTiempo->setText(QString::fromStdString(tiempo));	
	std::cout << "presionado stop " << tiempo << std::endl;
}

void Gui::on_reiniciar_buttonpressed(){

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
