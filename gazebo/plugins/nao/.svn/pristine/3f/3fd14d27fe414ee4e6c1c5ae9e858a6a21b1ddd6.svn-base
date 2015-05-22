#include "pose3d.h"

namespace gazebo {

    void *pose3dICE(void* v);

    GZ_REGISTER_MODEL_PLUGIN(Pose3d)

    Pose3d::Pose3d() {
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&mutex2, NULL);
        count = 0;
        std::cout << "constructor Pose3d" << std::endl;
    }

    void Pose3d::Load(physics::ModelPtr _parent, sdf::ElementPtr) {

        model = _parent;
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                boost::bind(&Pose3d::OnUpdate, this));
    }
    
    physics::ModelPtr Pose3d::getModel()
    {
    	return model;
    }

    void Pose3d::OnUpdate() {

        if (count == 0) {
            count++;
//            std::string name = this->model->GetName();
//            std::cout << "GetName() pose3d: " << name << std::endl;
            cfgfile_pose3d = std::string("--Ice.Config=pose3d.cfg");
            pthread_t thr_gui;
            pthread_create(&thr_gui, NULL, &pose3dICE, (void*) this);
        }

        position = model->GetWorldPose();
        
        pthread_mutex_lock(&mutex);
        mypose.x = position.pos.x;
        mypose.y = position.pos.y;
        mypose.z = position.pos.z;
        mypose.q0 = position.rot.w;
        mypose.q1 = position.rot.x;
        mypose.q2 = position.rot.y;
        mypose.q3 = position.rot.z;
        pthread_mutex_unlock(&mutex);

    }

    class Pose3DI : virtual public jderobot::Pose3D {
    public:

        Pose3DI(gazebo::Pose3d* pose) : pose3dData(new jderobot::Pose3DData()) {
            this->pose = pose;
        }

        virtual ~Pose3DI() {
        };

	virtual jderobot::Pose3DDataPtr getPose3DData(const Ice::Current&) {}
    
        virtual Ice::Int setPose3DData(const jderobot::Pose3DDataPtr&  pose3dData,
        						     const Ice::Current&) {
             math::Pose position = this->pose->getModel()->GetWorldPose();

             // I should check the value of h, in case this is 0 and therefore
             //    the point is in the inifnity

		pthread_mutex_lock(&pose->mutex2);

		position.pos.x = pose3dData->x;
		position.pos.y = pose3dData->y;
		position.pos.z = pose3dData->z;
		position.rot.w = pose3dData->q0;
		position.rot.x = pose3dData->q1;
		position.rot.y = pose3dData->q2;
		position.rot.z = pose3dData->q3;

		position.Set(math::Vector3(pose3dData->x, pose3dData->y, pose3dData->z),
                             math::Quaternion(pose3dData->q0, pose3dData->q1, pose3dData->q2, pose3dData->q3)
                             );

		pthread_mutex_unlock(&pose->mutex2);

        	this->pose->getModel()->SetWorldPose(position);
            return 0;
        };

    public:
        gazebo::Pose3d* pose;
    private:
        jderobot::Pose3DDataPtr pose3dData;
    };

    class Pose3DO : virtual public jderobot::Pose3D {
    public:

        Pose3DO(gazebo::Pose3d* pose) : pose3dData(new jderobot::Pose3DData()) {
            this->pose = pose;
        }

        virtual ~Pose3DO() {
        };
    
        virtual Ice::Int setPose3DData(const jderobot::Pose3DDataPtr&  pose3dData,
        						     const Ice::Current&) {}

        virtual jderobot::Pose3DDataPtr getPose3DData(const Ice::Current&) {
            pthread_mutex_lock(&pose->mutex2);

            pose3dData->x = pose->mypose.x;
            pose3dData->y = pose->mypose.y;
            pose3dData->z = pose->mypose.z;
            pose3dData->h = 1; // position in meters
            pose3dData->q0 = pose->mypose.q0;
            pose3dData->q1 = pose->mypose.q1;
            pose3dData->q2 = pose->mypose.q2;
            pose3dData->q3 = pose->mypose.q3;

            pthread_mutex_unlock(&pose->mutex2);

            return pose3dData;
        };

    public:
        gazebo::Pose3d* pose;
    private:
        jderobot::Pose3DDataPtr pose3dData;
    };

    void *pose3dICE(void* v) {
        gazebo::Pose3d* base = (gazebo::Pose3d*)v;
        char* name = (char*) base->cfgfile_pose3d.c_str();
        Ice::CommunicatorPtr ic;
        int argc = 1;
        Ice::PropertiesPtr prop;
        char* argv[] = {name};

        try {

            ic = Ice::initialize(argc, argv);


            prop = ic->getProperties();
            std::string Endpoints = prop->getProperty("Pose3d.Endpoints");
            std::cout << "Pose3d Endpoints > " << Endpoints << std::endl;

            Ice::ObjectAdapterPtr adapter =
                    ic->createObjectAdapterWithEndpoints("Pose3d", Endpoints);
            Ice::ObjectPtr objectI = new Pose3DI(base);
	    Ice::ObjectPtr objectO = new Pose3DO(base);

            adapter->add(objectI, ic->stringToIdentity("Pose3dI"));
	    adapter->add(objectO, ic->stringToIdentity("Pose3dO"));

            adapter->activate();
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
