#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.hh"
#include "common/common.hh"
#include "transport/transport.hh"


// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>


// JDErobot general ice component includes
#include <jderobot/pose3d.h>

#ifndef POSE3D_H
#define	POSE3D_H

namespace gazebo {
    
    
    class Pose3d : public ModelPlugin {
    public:
        
        Pose3d();
        
        virtual void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/);
        physics::ModelPtr getModel();
        
        pthread_mutex_t mutex;
        pthread_mutex_t mutex2;
        int count;
        struct pose3d_t {
            float x;
            float y;
            float z;
            float q0;
            float q1;
            float q2;
            float q3;
        };
        // float h; Do I need it here??
        math::Quaternion initial_q;
        pose3d_t mypose;
        std::string cfgfile_pose3d;
        
    private:
        
        void OnUpdate();
        physics::ModelPtr model;
        math::Pose position;
        event::ConnectionPtr updateConnection;
        
        
    };
    
}

#endif	/* POSE3D_H */
