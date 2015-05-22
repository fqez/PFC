//LCDNumber.h File

#ifndef LCDNUMBER_H
#define LCDNUMBER_H

#include <QtGui>
class LCDNumber: public QLCDNumber
{
  Q_OBJECT  
	
  public:
  QTimer* timer;
  QTime*  timeValue;
  std::string time;
  
  public:    
	LCDNumber()
	{
	    timer	= 	new QTimer();
	    timeValue	=	new QTime(0,0,0,0);
	    
	    this->display(timeValue->toString(QString("mm:ss.zzz")));
	    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(setDisplay()));
	};
	~ LCDNumber(){};
	int getMinute() {return this->timeValue->minute();};
	int getMili() {return this->timeValue->msec();};
	int getSecond() {return this->timeValue->second();};
	std::string getTime() {return this->time;};
	
   public slots:    
	void setDisplay()
	{
	  this->timeValue->setHMS(0,this->timeValue->addMSecs(1).minute(),
				    this->timeValue->addMSecs(1).second(),
				    this->timeValue->addMSecs(1).msec());
	  this->display(this->timeValue->toString(QString("mm:ss.zzz")));
	};
	
	void start(){
	  this->timer->start();};
	
	void stop(){
	  this->timer->stop();
	  time = this->timeValue->toString("mm:ss.zzz").toUtf8().constData();
	  timeValue->~QTime();
	  timeValue = new QTime(0,0,0,0);
	};

	void pause(){
	  this->timer->stop();
	};
	
};
#endif
