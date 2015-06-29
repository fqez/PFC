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


#define SHOULDER_PITCH_A 0.37
#define SHOULDER_PITCH_S 90.5
#define SHOULDER_PITCH_P 3.14
#define SHOULDER_ROLL_A 0.05
#define SHOULDER_ROLL_S -12.5
#define SHOULDER_ROLL_P 0
#define ELBOW_YAW_A 0.2
#define ELBOW_YAW_S 75
#define ELBOW_YAW_P 3.14
#define ELBOW_ROLL_A 0.22
#define ELBOW_ROLL_S 31
#define ELBOW_ROLL_P 0

#define HIP_ROLL_A 0.2
#define HIP_ROLL_S 1
#define HIP_ROLL_P 1.047 /* PI/3*/
#define KNEE_PITCH_P 2.67	/*PI/2 + PI/4 + PI/10*/
#define ANKLE_PITCH_P 3.455	/*PI + PI/10*/
#define ANKLE_ROLL_A 0.23
#define ANKLE_ROLL_S -2.9
#define ANKLE_ROLL_P 2.0943	/*2pi/3*/
#define BASE_PHASE -1.5  /*Right hip pitch phase, all the other phases depend on this one*/

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

		WalkerI(gazebo::Walking* walking) : walkerData(new jderobot::WalkerData()), stadistics(new jderobot::StadisticsData()) {
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

			//right leg orders
			poseRH->righthip.motorsdata.tilt = 0.0000000;
			poseRH->righthip.motorsdata.roll = 0.0000000;
			poseRH->righthip.motorsdata.pan = 0.0000000;
			poseRK->rightknee.motorsdata.tilt = 0.0000000;
			poseRA->rightankle.motorsdata.tilt = 0.0000000;
			poseRA->rightankle.motorsdata.roll = 0.0000000;

			//left leg orders
			poseLH->lefthip.motorsdata.tilt = 0.0000000;
			poseLH->lefthip.motorsdata.roll = 0.0000000;
			poseLH->lefthip.motorsdata.pan = 0.0000000;
			poseLK->leftknee.motorsdata.tilt = 0.0000000;
			poseLA->leftankle.motorsdata.tilt = 0.0000000;
			poseLA->leftankle.motorsdata.roll = 0.0000000;

			//rigth arm orders
			poseRS->rightshoulder.motorsdata.tilt = 0.0000000;
			poseRS->rightshoulder.motorsdata.roll = 0.0000000;
			poseRE->rightelbow.motorsdata.roll = 0.0000000;
			poseRE->rightelbow.motorsdata.pan = 0.0000000;
	
			//left arm orders
			poseLS->leftshoulder.motorsdata.tilt = 0.0000000;
			poseLS->leftshoulder.motorsdata.roll = 0.0000000;
			poseLE->leftelbow.motorsdata.roll = 0.0000000;
			poseLE->leftelbow.motorsdata.pan = 0.0000000;

			//Aqui reseteamos el mundo.
		  	gazebo::physics::WorldPtr world = this->walking->getModel()->GetWorld();
		  	world->Reset();
		}

		math::Pose nao_position() {

			gazebo::physics::ModelPtr model = this->walking->getModel();
			return model->GetWorldPose();
		}

		bool hasFallen() {
			return (nao_position().pos.z < 0.15) ? true : false;
		}

		float simTime() {
			common::Time t = this->walking->getModel()->GetWorld()->GetRealTime();  //Tiempo en segundos
	    	return t.Float();
		}

		void emptyVectors() {

			std::cout << "en empty!!" << std::endl;

			//right leg positions	
			positions_right_hip_pitch.clear();
			positions_right_hip_roll.clear();
			positions_right_knee_pitch.clear();
			positions_right_ankle_pitch.clear();
			positions_right_ankle_roll.clear();
		
			//left leg positions
			positions_left_hip_pitch.clear();
			positions_left_hip_roll.clear();
			positions_left_knee_pitch.clear();
			positions_left_ankle_pitch.clear();
			positions_left_ankle_roll.clear();

			//right arm positions
			positions_right_shoulder_pitch.clear();
			positions_right_shoulder_roll.clear();
			positions_right_elbow_roll.clear();
			positions_right_elbow_yaw.clear();
		
			//left arm positions
			positions_left_shoulder_pitch.clear();
			positions_left_shoulder_roll.clear();
			positions_left_elbow_roll.clear();
			positions_left_elbow_yaw.clear();

			/*//right leg positions	
			positions_right_hip_pitch.resize(0);
			positions_right_hip_roll.resize(0);
			positions_right_knee_pitch.resize(0);
			positions_right_ankle_pitch.resize(0);
			positions_right_ankle_roll.resize(0);
		
			//left leg positions
			positions_left_hip_pitch.resize(0);
			positions_left_hip_roll.resize(0);
			positions_left_knee_pitch.resize(0);
			positions_left_ankle_pitch.resize(0);
			positions_left_ankle_roll.resize(0);

			//right arm positions
			positions_right_shoulder_pitch.resize(0);
			positions_right_shoulder_roll.resize(0);
			positions_right_elbow_roll.resize(0);
			positions_right_elbow_yaw.resize(0);
		
			//left arm positions
			positions_left_shoulder_pitch.resize(0);
			positions_left_shoulder_roll.resize(0);
			positions_left_elbow_roll.resize(0);
			positions_left_elbow_yaw.resize(0);

			{
				//right leg positions	
				std::vector<double> temp_positions_right_hip_pitch;
				std::vector<double> temp_positions_right_hip_roll;
				std::vector<double> temp_positions_right_knee_pitch;
				std::vector<double> temp_positions_right_ankle_pitch;
				std::vector<double> temp_positions_right_ankle_roll;
		
				//left leg positions
				std::vector<double> temp_positions_left_hip_pitch;
				std::vector<double> temp_positions_left_hip_roll;
				std::vector<double> temp_positions_left_knee_pitch;
				std::vector<double> temp_positions_left_ankle_pitch;
				std::vector<double> temp_positions_left_ankle_roll;

				//right arm positions
				std::vector<double> temp_positions_right_shoulder_pitch;
				std::vector<double> temp_positions_right_shoulder_roll;
				std::vector<double> temp_positions_right_elbow_roll;
				std::vector<double> temp_positions_right_elbow_yaw;
		
				//left arm positions
				std::vector<double> temp_positions_left_shoulder_pitch;
				std::vector<double> temp_positions_left_shoulder_roll;
				std::vector<double> temp_positions_left_elbow_roll;
				std::vector<double> temp_positions_left_elbow_yaw;

				//right leg positions	
				positions_right_hip_pitch.swap(temp_positions_right_hip_pitch);
				positions_right_hip_roll.swap(temp_positions_right_hip_roll);
				positions_right_knee_pitch.swap(temp_positions_right_knee_pitch);
				positions_right_ankle_pitch.swap(temp_positions_right_ankle_pitch);
				positions_right_ankle_roll.swap(temp_positions_right_ankle_roll);
		
				//left leg positions
				positions_left_hip_pitch.swap(temp_positions_left_hip_pitch);
				positions_left_hip_roll.swap(temp_positions_left_hip_roll);
				positions_left_knee_pitch.swap(temp_positions_left_knee_pitch);
				positions_left_ankle_pitch.swap(temp_positions_left_ankle_pitch);
				positions_left_ankle_roll.swap(temp_positions_left_ankle_roll);

				//right arm positions
				positions_right_shoulder_pitch.swap(temp_positions_right_shoulder_pitch);
				positions_right_shoulder_roll.swap(temp_positions_right_shoulder_roll);
				positions_right_elbow_roll.swap(temp_positions_right_elbow_roll);
				positions_right_elbow_yaw.swap(temp_positions_right_elbow_yaw);
		
				//left arm positions
				positions_left_shoulder_pitch.swap(temp_positions_left_shoulder_pitch);
				positions_left_shoulder_roll.swap(temp_positions_left_shoulder_roll);
				positions_left_elbow_roll.swap(temp_positions_left_elbow_roll);
				positions_left_elbow_yaw.swap(temp_positions_left_elbow_yaw);
			}*/
		}

		bool condition() {
//std::cout << "fallen: " << hasFallen() << " simTime: " << (simTime() > 6.0) << " result: " << ((hasFallen()) || (simTime() > 6.0)) << std::endl;
			return ((hasFallen()) || (simTime() > 7.0));
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

		float ankle_pitch_val(float frequency, float time, float amplitude, float shift, float phase) {


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

			float r_phase, l_phase;
			
			time = 0;
			moment = frequency/pos;
			r_phase = BASE_PHASE;

			std::cout << "hip pitch resutls" << std::endl;
			for (int i = 0; i < pos; i++) {
				time = time + moment;
				result = hip_pitch_val(frequency, time, amplitude, shift, r_phase);
				positions_right_hip_pitch.push_back(result);
				std::cout << result << std::endl;
			}
			time = 0;
			l_phase = M_PI + r_phase;
			std::cout << "hip pitch resutls left" << std::endl;
			for (int i = 0; i < pos; i++) {
				time = time + moment;
				result = hip_pitch_val(frequency, time, amplitude, shift, l_phase);
				positions_left_hip_pitch.push_back(result);
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
			for (int i = 0; i < pos; i++) {
				time = time + moment;
				result = amplitude*sin((freq*(time))+phase)+shift;
				positions_right_hip_roll.push_back(result);
				std::cout << result << std::endl;
			}
			time = 0;
			shift = shift - 1;
			std::cout << "right hip roll left" << std::endl;
			for (int i = 0; i < pos; i++) {
				time = time + moment;
				result = amplitude*sin((freq*(time))+phase)+shift;
				positions_left_hip_roll.push_back(result);
				std::cout << result << std::endl;
			}

		}

		void knee_pitch_value(float frequency, float amplitude, float phase, float shift, int pos) {

			float time;
			float moment;

			float result;
			float r_phase, l_phase;
			

			time = 0;
			moment = frequency/pos;
			r_phase = BASE_PHASE + KNEE_PITCH_P + phase;

			std::cout << "knee pitch results" << std::endl;
			//right knee positions
			for (int i = 0; i < pos; i++) {
				time = time + moment;
				result = knee_pitch_val(frequency, time, amplitude, shift, r_phase);
				positions_right_knee_pitch.push_back(result);
				std::cout << result << std::endl;
			}
			std::cout << "knee pitch results left" << std::endl;
			//left knee positions
			time = 0;
			l_phase = 2*M_PI + r_phase;
			for (int i = 0; i < pos; i++) {
				time = time + moment;
				result = knee_pitch_val(frequency, time, amplitude, shift, l_phase);
				positions_left_knee_pitch.push_back(result);
				std::cout << result << std::endl;
			}
		}

		void ankle_pitch_value(float frequency, float amplitude, float phase, float shift, int pos) {
		
			float time, moment;

			float result;
			float r_phase, l_phase;

			time = 0;
			moment = frequency/pos;
			r_phase = BASE_PHASE + ANKLE_PITCH_P + phase;

			std::cout << "ankle pitch values" << std::endl;
			for(int i = 0; i < pos; i++) {
				time = time + moment;
				result = ankle_pitch_val(frequency, time, amplitude, shift, r_phase);
				positions_right_ankle_pitch.push_back(result);
				std::cout << result << std::endl;
			}
			time = 0;
			l_phase = M_PI + r_phase;
			std::cout << "ankle pitch values left" << std::endl;
			for(int i = 0; i < pos; i++) {
				time = time + moment;
				result = ankle_pitch_val(frequency, time, amplitude, shift, l_phase);
				positions_left_ankle_pitch.push_back(result);
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
			for (int i = 0; i < pos; i++) {
				time = time + moment;
				result = amplitude*sin((freq*(time))+phase)+shift;
				positions_right_ankle_roll.push_back(result);
				std::cout << result << std::endl;
			}
			time = 0;
			shift = shift + 5;
			std::cout << "ankle roll results left" << std::endl;
			for (int i = 0; i < pos; i++) {
				time = time + moment;
				result = amplitude*sin((freq*(time))+phase)+shift;
				positions_left_ankle_roll.push_back(result);
				std::cout << result << std::endl;
			}
		}

		void shoulder_pitch_value(float frequency, float amplitude_arms, int pos) {
			float moment;
			float time;
			float freq;
			float phase, shift,amplitude;
			
			float result;

			phase=BASE_PHASE + SHOULDER_PITCH_P;
			shift=SHOULDER_PITCH_S;
			amplitude=SHOULDER_PITCH_A*amplitude_arms;
			moment = frequency/pos;
			freq = (float)2*M_PI/(frequency);
			time=0;
			//std::cout << "right shoulder pitch" << std::endl;
			for (int i = 0; i < pos; i++){
				time=time+moment;
				result =amplitude*sin((freq*(time))+phase)+shift;
				positions_right_shoulder_pitch.push_back(result);
				//std::cout << result << std::endl;
			}
			phase=phase+(M_PI);
			for (int i = 0; i < pos; i++){
				time=time+moment;
				result=amplitude*sin((freq*(time))+phase)+shift;
				positions_left_shoulder_pitch.push_back(result);
			}
		}

		void shoulder_roll_value(float frequency, float amplitude_arms, int pos) {
			float moment;
			float time;
			float freq;
			float phase, shift,amplitude;
			
			float result;

			phase=BASE_PHASE + SHOULDER_ROLL_P;
			shift=SHOULDER_ROLL_S;
			amplitude=SHOULDER_ROLL_A*amplitude_arms;
			moment = frequency/pos;
			freq = (float)2*M_PI/(frequency);
			time=0;
			for (int i = 0; i < pos; i++){
				time=time+moment;
				result =amplitude*sin((freq*(time))+phase)+shift;
				positions_right_shoulder_roll.push_back(result);
				//std::cout << result << std::endl;
			}
			time = 0;
			for (int i = 0; i < pos; i++){
				time=time+moment;
				result=-(amplitude*sin((freq*(time))+phase)+shift);
				positions_left_shoulder_roll.push_back(result);
			}
	
		}

		void elbow_roll_value(float frequency, float amplitude_arms, int pos){
			float moment;
			float time;
			float freq;
			float phase, shift,amplitude;
			
			float result;

			phase=BASE_PHASE + ELBOW_ROLL_P;
			shift=ELBOW_ROLL_S;
			amplitude=ELBOW_ROLL_A*amplitude_arms;
			moment = frequency/pos;
			freq = (float)2*M_PI/(frequency);
			time=0;
			for (int i = 0; i < pos; i++){
				time=time+moment;
				result =amplitude*sin((freq*(time))+phase)+shift;
				positions_right_elbow_roll.push_back(result);
				//std::cout << result << std::endl;
			}
			time = 0;
			for (int i = 0; i < pos; i++){
				time=time+moment;
				result=-(amplitude*sin((freq*(time))+phase)+shift);
				positions_left_elbow_roll.push_back(result);
			}
		}

		void elbow_yaw_value(float frequency, float amplitude_arms, int pos){
			float moment;
			float time;
			float freq;
			float phase, shift,amplitude;
			
			float result;

			phase=BASE_PHASE + ELBOW_YAW_P;
			shift=ELBOW_YAW_S;
			amplitude=ELBOW_YAW_A*amplitude_arms;
			moment = frequency/pos;
			freq = (float)2*M_PI/(frequency);
			time=0;
			for (int i = 0; i < pos; i++){
				time=time+moment;
				result =amplitude*sin((freq*(time))+phase)+shift;
				positions_right_elbow_yaw.push_back(result);
				//std::cout << result << std::endl;
			}
			time = 0;
			for (int i = 0; i < pos; i++){
				time=time+moment;
				result=-(amplitude*sin((freq*(time))+phase)+shift);
				positions_left_elbow_yaw.push_back(result);
			}
		}
	
		virtual Ice::Int stopWalk (const Ice::Current&) {

		}

		virtual Ice::Int setParams(const jderobot::WalkerDataPtr&  walkerData,
		    						     const Ice::Current&) {

			//std::cout << "en setParams" << std::endl;

		         // I should check the value of h, in case this is 0 and therefore
		         //    the point is in the inifnity

			pthread_mutex_lock(&walking->mutex2);

			walking->my_params.w = walkerData->param1;	
walking->my_params.hip_amplitude = walkerData->param2;	
walking->my_params.hip_shift = walkerData->param3;	
walking->my_params.knee_amplitude = walkerData->param4;;	
walking->my_params.knee_phase = walkerData->param5;	
walking->my_params.knee_shift = 42.1;	
walking->my_params.ankle_amplitude = 7.5;	
walking->my_params.ankle_phase = walkerData->param8;	
walking->my_params.ankle_shift = -21.4;	
walking->my_params.balance = 38.29999;

			int n_pos = 18;
			float arms_amplitude = 50.0;

			sleep_time = (walking->my_params.w / n_pos) * 1000;

std::cout << "@@@@@@@@@@@@@@ " << sleep_time << std::endl;

			//yaw de la cadera fijo a 0. El balance se utiliza para calcular los roll.
			// 18 (pos) es el numero de valores que va a tener el vector. divide la onda completa en 18 valores. El for para obtener los valores vale con 18
			// iteraciones en este caso. A mayor numero, en mas valores divides la onda.
	
			//Legs
			hip_pitch_value(walking->my_params.w, walking->my_params.hip_amplitude, walking->my_params.hip_shift, n_pos);
			hip_roll_value(walking->my_params.w, walking->my_params.balance, n_pos);
			knee_pitch_value(walking->my_params.w, walking->my_params.knee_amplitude, walking->my_params.knee_phase, walking->my_params.knee_shift, n_pos);
			ankle_pitch_value(walking->my_params.w, walking->my_params.ankle_amplitude, walking->my_params.ankle_phase, walking->my_params.ankle_shift, n_pos);
			ankle_roll_value(walking->my_params.w, walking->my_params.balance, n_pos);


			//Arms
			shoulder_pitch_value(walking->my_params.w, arms_amplitude, n_pos);
			shoulder_roll_value(walking->my_params.w, arms_amplitude, n_pos);
			elbow_roll_value(walking->my_params.w, arms_amplitude, n_pos);
			elbow_yaw_value(walking->my_params.w, arms_amplitude, n_pos);	



			pthread_mutex_unlock(&walking->mutex2);

		    
		    return 0;
		}

		virtual jderobot::StadisticsDataPtr getStadistics (const Ice::Current&) {

			return stadistics;
		}

		virtual Ice::Int startWalk (const Ice::Current&) {
			
			std::cout << "en startWalkv2" << std::endl;
/*std::cout << "elmentos en Rhip pitch vector " << positions_right_hip_pitch.size() << " capacity: " << positions_right_hip_pitch.capacity() << std::endl;
std::cout << "elmentos en Rhip roll vector " << positions_right_hip_roll.size() << " capacity: " << positions_right_hip_roll.capacity() << std::endl;
std::cout << "elmentos en Rknee pitch vector " << positions_right_knee_pitch.size() << " capacity: " << positions_right_knee_pitch.capacity() << std::endl;
std::cout << "elmentos en Rankle pitch vector " << positions_right_ankle_pitch.size() << " capacity: " << positions_right_ankle_pitch.capacity() << std::endl;
std::cout << "elmentos en Rankle roll vector " << positions_right_ankle_roll.size() << " capacity: " << positions_right_ankle_roll.capacity() << std::endl;

std::cout << "elmentos en Lhip pitch vector " << positions_left_hip_pitch.size() << " capacity: " << positions_left_hip_pitch.capacity() << std::endl;
std::cout << "elmentos en Lhip roll vector " << positions_left_hip_roll.size() << " capacity: " << positions_left_hip_roll.capacity() << std::endl;
std::cout << "elmentos en Lknee pitch vector " << positions_left_knee_pitch.size() << " capacity: " << positions_left_knee_pitch.capacity() << std::endl;
std::cout << "elmentos en Lankle pitch vector " << positions_left_ankle_pitch.size() << " capacity: " << positions_left_ankle_pitch.capacity() << std::endl;
std::cout << "elmentos en Lankle roll vector " << positions_left_ankle_roll.size() << " capacity: " << positions_left_ankle_roll.capacity() << std::endl;

std::cout << "elmentos en Rshoulder pitch vector " << positions_right_shoulder_pitch.size() << " capacity: " << positions_right_shoulder_pitch.capacity() << std::endl;
std::cout << "elmentos en Rshoulder roll vector " << positions_right_shoulder_roll.size() << " capacity: " << positions_right_shoulder_roll.capacity() << std::endl;
std::cout << "elmentos en Relbow roll vector " << positions_right_elbow_roll.size() << " capacity: " << positions_right_elbow_roll.capacity() << std::endl;
std::cout << "elmentos en Relbow yaw vector " << positions_right_elbow_yaw.size() << " capacity: " << positions_right_elbow_yaw.capacity() << std::endl;


std::cout << "elmentos en Lshoulder pitch vector " << positions_left_shoulder_pitch.size() << " capacity: " << positions_left_shoulder_pitch.capacity() << std::endl;
std::cout << "elmentos en Lshoulder roll vector " << positions_left_shoulder_roll.size() << " capacity: " << positions_left_shoulder_roll.capacity() << std::endl;
std::cout << "elmentos en Lelbow roll vector " << positions_left_elbow_roll.size() << " capacity: " << positions_left_elbow_roll.capacity() << std::endl;
std::cout << "elmentos en Lelbow yaw vector " << positions_left_elbow_yaw.size() << " capacity: " << positions_left_elbow_yaw.capacity() << std::endl;*/
			try {
//sleep(2000);
				resetWorld();
		
				stadistics->x0 = nao_position().pos.x * 100.0;
				stadistics->y0 = nao_position().pos.y * 100.0;
				stadistics->z0 = nao_position().pos.z * 100.0;

				float cont = 0;
				float error = 0.0;
				float pos_ant_x = 0.0;
				float pos_ant_y = 0.0;
				float pos_x = 0.0;
				float pos_y = 0.0;
				
				while (!condition()) {  //condiciones: que no hayan pasado 6 segundos de simulacion y que el robot no haya caido
					
					pos_x = nao_position().pos.x * 100.0;
					pos_y = nao_position().pos.y * 100.0;

					error = error + sqrt( pow((pos_x - pos_ant_x),2) + pow((pos_y - pos_ant_y),2) );
					cont = cont + 1.0;

					pos_ant_x = pos_x;
					pos_ant_y = pos_y;
					for (int i = 0; i < 18; i++) { 
					
						//right leg orders
						poseRH->righthip.motorsdata.tilt = positions_right_hip_pitch[i]*M_PI/180;
						poseRH->righthip.motorsdata.roll = positions_right_hip_roll[i]*M_PI/180;
						poseRH->righthip.motorsdata.pan = 0.0000;
						poseRK->rightknee.motorsdata.tilt = positions_right_knee_pitch[i]*M_PI/180;
						poseRA->rightankle.motorsdata.tilt = positions_right_ankle_pitch[i]*M_PI/180;
						poseRA->rightankle.motorsdata.roll = positions_right_ankle_roll[i]*M_PI/180;

						//left leg orders
						poseLH->lefthip.motorsdata.tilt = positions_left_hip_pitch[i]*M_PI/180;
						poseLH->lefthip.motorsdata.roll = positions_left_hip_roll[i]*M_PI/180;
						poseLH->lefthip.motorsdata.pan = 0.0000;
						poseLK->leftknee.motorsdata.tilt = positions_left_knee_pitch[i]*M_PI/180;
						poseLA->leftankle.motorsdata.tilt = positions_left_ankle_pitch[i]*M_PI/180;
						poseLA->leftankle.motorsdata.roll = positions_left_ankle_roll[i]*M_PI/180;

						//rigth arm orders
						poseRS->rightshoulder.motorsdata.tilt = positions_right_shoulder_pitch[i]*M_PI/180;
						poseRS->rightshoulder.motorsdata.roll = positions_right_shoulder_roll[i]*M_PI/180;
						poseRE->rightelbow.motorsdata.roll = positions_right_elbow_roll[i]*M_PI/180;
						poseRE->rightelbow.motorsdata.pan = positions_right_elbow_yaw[i]*M_PI/180;
				
						//left arm orders
						poseLS->leftshoulder.motorsdata.tilt = positions_left_shoulder_pitch[i]*M_PI/180;
						poseLS->leftshoulder.motorsdata.roll = positions_left_shoulder_roll[i]*M_PI/180;
						poseLE->leftelbow.motorsdata.roll = positions_left_elbow_roll[i]*M_PI/180;
						poseLE->leftelbow.motorsdata.pan = positions_left_elbow_yaw[i]*M_PI/180;

						sleep(sleep_time);
					}
				}
				
				emptyVectors();
				this->walking->getModel()->GetWorld()->SetPaused(true);
				stadistics->x1 = nao_position().pos.x * 100.0;
				stadistics->y1 = nao_position().pos.y * 100.0;
				stadistics->z1 = nao_position().pos.z * 100.0;
				stadistics->simTime = simTime();
				stadistics->distanceX = std::abs(stadistics->x1 - stadistics->x0) * 100.0;
				stadistics->distanceY = std::abs(stadistics->y1 - stadistics->y0) * 100.0;
				stadistics->desviation = error / (cont * 1.0);
				stadistics->fallen = hasFallen();

				this->walking->getModel()->GetWorld()->SetPaused(false);					

				resetWorld();
				return 0;

			} catch (const Ice::Exception& e) {
		            std::cerr << e << std::endl;
	        } catch (const char* msg) {
	            std::cerr << msg << std::endl;
	        }
		};

	private:
		jderobot::WalkerDataPtr  walkerData;

		int sleep_time;

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

		//right arm positions
		std::vector<double> positions_right_shoulder_pitch;
		std::vector<double> positions_right_shoulder_roll;
		std::vector<double> positions_right_elbow_roll;
		std::vector<double> positions_right_elbow_yaw;
		
		//left arm positions
		std::vector<double> positions_left_shoulder_pitch;
		std::vector<double> positions_left_shoulder_roll;
		std::vector<double> positions_left_elbow_roll;
		std::vector<double> positions_left_elbow_yaw;

		jderobot::StadisticsDataPtr stadistics;	


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
