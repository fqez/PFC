// COMMENTS
/*

	Para medir el timepo de la simulacion, tomar una medida al empezar la simulación y otra al terminar y restarlas:
			
	    common::Time t = this->walking->getModel()->GetWorld()->GetSimTime();  //Tiempo en segundos
	    std::cout << "tiempo inic " << t.Float() << std::endl; 

	Para resetear el mundo al estado original:

		gazebo::physics::WorldPtr world = this->walking->getModel()->GetWorld();
		world->Reset();

*/




#include "walking.h"

#include <math.h>


//left side
#include "poseleftankle.h"
#include "poseleftelbow.h"
#include "poselefthip.h"
#include "poseleftknee.h"
#include "poseleftshoulder.h"

//neck
#include "poseneck.h"
#include "poseneckspeed.h"

//right side
#include "poserightankle.h"
#include "poserightelbow.h"
#include "poserighthip.h"
#include "poserightknee.h"
#include "poserightshoulder.h"

#define HIP_ROLL_A 0.2
#define HIP_ROLL_S 1
#define HIP_ROLL_P 1.047 /* PI/3*/
#define KNEE_PITCH_P 2.67	/*PI/2 + PI/4 + PI/10*/
#define ANKLE_PITCH_P 3.455	/*PI + PI/10*/
#define ANKLE_ROLL_A 0.23
#define ANKLE_ROLL_S -2.9
#define ANKLE_ROLL_P 2.0943	/*2pi/3*/
#define BASE_PHASE 0   //20	/*Right hip pitch phase, all the other phases depend on this one*/

namespace gazebo {
    GZ_REGISTER_MODEL_PLUGIN(Walking);

    //CONSTRCTOR
    Walking::Walking () {

	pthread_mutex_init(&this->mutex2, NULL);
	this->cfgfile_walking = std::string("--Ice.Config=walking.cfg");
        std::cout << "Constructor Walking" << std::endl;
    }

    //LOAD
    void Walking::Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf ) {
        
        pthread_t thr_ice;
        pthread_create(&thr_ice, NULL, &thread_WalkingICE, (void*) this);
		
		model = _model;

        // Load OnUpdate method
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&Walking::OnUpdate, this));
    }

	physics::ModelPtr Walking::getModel()
    {
    	return model;
    }

    void Walking::Init () { }

    void Walking::OnUpdate () { }

    class WalkerI : virtual public jderobot::Walker {
    public:

		WalkerI(gazebo::Walking* walking) : walkerData(new jderobot::WalkerData()) {
			this->walking = walking;	
		}

        virtual ~WalkerI() {};

		void sleep(unsigned long milisec) {
		
		//unsigned long milisec = 3000L;
		struct timespec req={0};
	   	time_t sec=(int)(milisec/1000);
	    	milisec=milisec-(sec*1000);
	    	req.tv_sec=sec;
	    	req.tv_nsec=milisec*1000000L;
	    	while(nanosleep(&req,&req)==-1)
		    continue;
	    }

		void resetWorld() {
		
			  std::cout << "en reload" << std::endl;
			  gazebo::physics::WorldPtr world = this->walking->getModel()->GetWorld();
			  world->Reset();
		}

		float hip_pitch_val(float frequency, float time, float amplitude, float shift, float phase) {

			float x;
			float freq;

			freq = (float)2*M_PI/(frequency);
			x = (freq * time) + phase;

			//pasamos x a su valor correspondiente positivo entre 0 y 2PI
			while (x < 0){
				x = (2* M_PI) + x;
			}
			while (x > 2*M_PI){
				x = x -(2*M_PI);
			}	
	
			if ((x>(3*M_PI/4)) &&( x<M_PI)){
				return amplitude+shift;
			}
			else{
				return ((6.0/11.0)*amplitude*sin(x) + shift);
			}

		}

		float knee_pitch_val(float frequency, float time, float amplitude, float shift, float phase) {
		
			float x;
			float freq;

			freq = (float)4*M_PI/(frequency);
			x = (freq * time) + phase; // valor de x para mi funcion en grados

			//pasamos x a su valor correspondiente positivo entre 0 y 4PI
			while (x < 0)
				x = (4* M_PI) + x;
			while (x > 4*M_PI)
				x = x -(4*M_PI);
	
			if ((x > (7*M_PI/2 - M_PI/4))&&(x <= (7*M_PI/2))){
				return ((x - (7* M_PI/2 - M_PI/4))/((7*M_PI/2)-(7*M_PI/2 - M_PI/4)))*((amplitude+shift)-(0.22*amplitude*sin(7*M_PI/2 - M_PI/4)+shift)) + 0.22*amplitude*sin(7*M_PI/2 - M_PI/4)+shift;
			}
			else if ((x>(7*M_PI/2)) && (x <= (7*M_PI/2 + M_PI/4))){
				return ((x - (7*M_PI/2))/((7*M_PI/2+ M_PI/4) - (7*M_PI/2))) * ((amplitude*0.22*(sin(7*M_PI/2 + M_PI/4))+shift) - (amplitude+shift)) + (amplitude+shift);
			}
			else{
				return 0.22* amplitude * sin(x) + shift;
			}

			return 0;

		}

		float ankle_pitch_val (float frequency, float time, float amplitude, float shift, float phase) {


			float x;
			float freq;

			freq = (float)2*M_PI/(frequency);
			x = (freq * time) + phase;

			while (x < 0){
				x = (2* M_PI) + x;
			}
			while (x > 2*M_PI){
				x = x -(2*M_PI);
			}

			if (( x>=0) && (x<=2*M_PI/5)){
				return ((x/(2*M_PI/5))*(2*amplitude)) + (shift - amplitude);
			}
			else
				return ((x - (2*M_PI/5))/(8*M_PI/5))*(-2*amplitude) + (shift + amplitude);

		}

		void hip_pitch_value(float frequency, float amplitude, float shift, int pos) {

			float time;
			float moment;
			float phase;

			float result;
			
			time = 0;
			moment = frequency/pos;
			phase = BASE_PHASE;

			std::cout << "hip pitch resutls" << std::endl;
			for (int i = 0; i < 50; i++) {
				time = time + moment;
				result = hip_pitch_val(frequency, time, amplitude, shift, phase);
				positions_right_hip_pitch.push_back(result);
				std::cout << result << std::endl;
			}


		}

		void hip_roll_value(float frequency, float balance, int pos) {
	
			float amplitude, phase, shift;
			float time, moment, freq;
			
			float result;

			amplitude = HIP_ROLL_A * balance;
			shift = HIP_ROLL_S;
			phase = BASE_PHASE + HIP_ROLL_P;
			
			time = 0;
			moment = frequency/pos;
			freq = (float)2*M_PI/(frequency);

			std::cout << "right hip roll" << std::endl;
			for (int i = 0; i < 50; i++) {
				time = time + moment;
				result = amplitude*sin((freq*(time))+phase)+shift;
				positions_right_hip_roll.push_back(result);
				std::cout << result << std::endl;
			}


/*			amplitude=HIP_ROLL_A*r_amplitude;
			shift= HIP_ROLL_S;
			phase= r_hip_pitch_phase + HIP_ROLL_P;
			printf("valor de phase: %f\n",phase);
			pos_time= frequency/n_pos;
			my_frequency= (float)2*PI/(frequency);
			time=0;
			for (recorrido=m->inicio;recorrido!=NULL;recorrido=recorrido->siguiente){
				time=time+pos_time;
				recorrido->pos.right_hip.roll=amplitude*sin((my_frequency*(time))+phase)+shift;
			}
			time=0;
			shift=shift-1;
			for (recorrido=m->inicio;recorrido!=NULL;recorrido=recorrido->siguiente){
				time=time+pos_time;
				recorrido->pos.left_hip.roll=(amplitude*sin((my_frequency*(time))+phase)+shift);
			}
*/
		}

		void knee_pitch_value(float frequency, float amplitude, float phase, float shift, int pos) {

			float time;
			float moment;

			float result;
			

			time = 0;
			moment = frequency/pos;
			phase = BASE_PHASE + KNEE_PITCH_P + phase;

			std::cout << "knee pitch results" << std::endl;
			for (int i = 0; i < 50; i++) {
				time = time + moment;
				result = knee_pitch_val(frequency, time, amplitude, shift, phase);
				positions_right_knee_pitch.push_back(result);
				std::cout << result << std::endl;
			}
						
			//left knee positions
			

		}

		void ankle_pitch_value(float frequency, float amplitude, float phase, float shift, int pos) {
		
			float time, moment;

			float result;

			time = 0;
			moment = frequency/pos;
			phase = BASE_PHASE + ANKLE_PITCH_P + phase;

			std::cout << "ankle pitch values" << std::endl;
			for(int i = 0; i < 50; i++) {
				time = time + moment;
				result = ankle_pitch_val(frequency, time, amplitude, shift, phase);
				positions_right_ankle_pitch.push_back(result);
				std::cout << result << std::endl;
			}

		}

		void ankle_roll_value(float frequency, float balance, int pos) {
	
			float amplitude, phase, shift;
			float time, moment, freq;

			float result;

			amplitude = ANKLE_ROLL_A*balance;
			shift = ANKLE_ROLL_S;
			phase = BASE_PHASE - ANKLE_ROLL_P;

			time = 0;
			moment = frequency/pos;
			freq = (float)2*M_PI/(frequency);
			
			std::cout << "ankle roll results" << std::endl;
			for (int i = 0; i < 50; i++) {
				time = time + moment;
				result = amplitude*sin((freq*(time))+phase)+shift;
				positions_right_ankle_roll.push_back(result);
				std::cout << result << std::endl;
			}

/*
			amplitude=ANKLE_ROLL_A*r_amplitude;
			shift= ANKLE_ROLL_S;
			phase= r_hip_pitch_phase - ANKLE_ROLL_P;
			pos_time= frequency/n_pos;
			my_frequency= (float)2*PI/(frequency);
			time=0;
			for (recorrido=m->inicio;recorrido!=NULL;recorrido=recorrido->siguiente){
				time=time+pos_time;
				recorrido->pos.right_ankle.roll=amplitude*sin((my_frequency*(time))+phase)+shift;
			}
			time=0;
			shift=shift+5;
			for (recorrido=m->inicio;recorrido!=NULL;recorrido=recorrido->siguiente){
				time=time+pos_time;
				recorrido->pos.left_ankle.roll=(amplitude*sin((my_frequency*(time))+phase)+shift);
			}
*/
		}
	
		virtual Ice::Int stopWalk (const Ice::Current&) {

		}

		virtual Ice::Int setParams(const jderobot::WalkerDataPtr&  walkerData,
		    						     const Ice::Current&) {

			std::cout << "en setParams" << std::endl;

		         // I should check the value of h, in case this is 0 and therefore
		         //    the point is in the inifnity

			pthread_mutex_lock(&walking->mutex2);

			walking->my_params.w = walkerData->param1;	
			walking->my_params.hip_amplitude = walkerData->param2;	
			walking->my_params.hip_shift = walkerData->param3;	
			walking->my_params.knee_amplitude = walkerData->param4;	
			walking->my_params.knee_phase = walkerData->param5;	
			walking->my_params.knee_shift = walkerData->param6;	
			walking->my_params.ankle_amplitude = walkerData->param7;	
			walking->my_params.ankle_phase = walkerData->param8;	
			walking->my_params.ankle_shift = walkerData->param9;	
			walking->my_params.balance = 20.0000; //walkerData->param10;

			//yaw de la cadera fijo a 0. El balance se utiliza para calcular los roll.
			// 18 (pos) es el numero de valores que va a tener el vector. divide la onda completa en 18 valores. El for para obtener los valores vale con 18
			// iteraciones en este caso. A mayor numero, en mas valores divides la onda.
			hip_pitch_value(walking->my_params.w, walking->my_params.hip_amplitude, walking->my_params.hip_shift, 18);
			hip_roll_value(walking->my_params.w, walking->my_params.balance, 18);
			knee_pitch_value(walking->my_params.w, walking->my_params.knee_amplitude, walking->my_params.knee_phase, walking->my_params.knee_shift, 18);
			ankle_pitch_value(walking->my_params.w, walking->my_params.ankle_amplitude, walking->my_params.ankle_phase, walking->my_params.ankle_shift, 18);
			ankle_roll_value(walking->my_params.w, walking->my_params.balance, 18);	

/*std::cout << "param1: " << walkerData->param1 << std::endl;
std::cout << "param2: " << walkerData->param2 << std::endl;
std::cout << "param3: " << walkerData->param3 << std::endl;
std::cout << "param4: " << walkerData->param4 << std::endl;
std::cout << "param5: " << walkerData->param5 << std::endl;
std::cout << "param6: " << walkerData->param6 << std::endl;
std::cout << "param7: " << walkerData->param7 << std::endl;
std::cout << "param8: " << walkerData->param8 << std::endl;
std::cout << "param9: " << walkerData->param9 << std::endl;
std::cout << "param10: " << walkerData->param10 << std::endl;*/

			pthread_mutex_unlock(&walking->mutex2);

		    
		    return 0;
		}

		virtual Ice::Int startWalk (const Ice::Current&) {
			
			std::cout << "en startWalk" << std::endl;



			try {
	
			//AQUI VA EL GENERADOR DE ONDA DE LA CAMINATA PARAMETRIZADA!!!!!
			//GENERAR LOS VALORES PARA TODAS LAS ARTICULACIONES Y COMANDAR EL ROBOT MEDIANTE LOS PUNTEROS A DICHAS ARTICS.
		
			while (true) {

				
				for (int i = 0; i < 50; i++) { 
					
					poseRK->rightknee.motorsdata.tilt = positions_right_knee_pitch[i]*M_PI/180;
					sleep(3000);
				}



			}
			

			} catch (const Ice::Exception& e) {
		            std::cerr << e << std::endl;
	        } catch (const char* msg) {
	            std::cerr << msg << std::endl;
	        }
		};

	private:
		jderobot::WalkerDataPtr  walkerData;

		//right leg positions	
		std::vector<double> positions_right_hip_pitch;
		std::vector<double> positions_right_hip_roll;
		std::vector<double> positions_right_knee_pitch;
		std::vector<double> positions_right_ankle_pitch;
		std::vector<double> positions_right_ankle_roll;
		
		//left leg positions
		std::vector<double> positions_left_hip_pitch;
		std::vector<double> positions_left_hip_roll;
		std::vector<double> positions_left_knee_pitch;
		std::vector<double> positions_left_ankle_pitch;
		std::vector<double> positions_left_ankle_roll;
		
	public:
		gazebo::Walking* walking;
    };



    /* this thread offers external communications */
    void* thread_WalkingICE ( void* v ) {

        gazebo::Walking* walking = (gazebo::Walking*)v;
        char* name = (char*) walking->cfgfile_walking.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {
            ic = Ice::initialize(argc, argv);

            prop = ic->getProperties();

	    
	    	std::string EndpointsWalk = prop->getProperty("Walk.Endpoints");
	    	std::cout << "Walk Endpoints > " << EndpointsWalk << std::endl;

	    	Ice::ObjectAdapterPtr AdapterWalk = 
		    ic->createObjectAdapterWithEndpoints("AdapterWalking", EndpointsWalk);

	    	Ice::ObjectPtr walk = new WalkerI(walking);
      
	    	AdapterWalk->add(walk, ic->stringToIdentity("Walking"));

	    	AdapterWalk->activate();

        	ic->waitForShutdown();
        } catch (const Ice::Exception& e) {
            std::cerr << e << std::endl;
        } catch (const char* msg) {
            std::cerr << msg << std::endl;
        }
        if (ic) {
            try {
                ic->destroy();
            } catch (const Ice::Exception& e) {
                std::cerr << e << std::endl;
            }
        }
    }
}
