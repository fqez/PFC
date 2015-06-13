/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Author:     Borja Menéndez Moreno <b.menendez.moreno@gmail.com>
 *  Co-author:  José María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.hh"
#include "common/common.hh"
#include "transport/transport.hh"

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

// JDErobot general ice component includes
#include <jderobot/pose3dencoders.h>
#include <jderobot/pose3dmotors.h>
#include <jderobot/walker.h>

#include "common.h"

#ifndef WALKING_H
#define WALKING_H

namespace gazebo {

    void* thread_WalkingICE ( void* v );

    class Walking : public ModelPlugin {
    public:

        Walking ();
        virtual void Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf );
        virtual void Init ();
		
		physics::ModelPtr getModel();

		pthread_mutex_t mutex;
       	pthread_mutex_t mutex2;
		struct params_t {
            		float w;
			float hip_amplitude;
			float hip_shift;
	  		float knee_amplitude;
			float knee_phase;
			float knee_shift;
			float ankle_amplitude;
			float ankle_phase;
			float ankle_shift;
			float balance;
        };
        params_t my_params;
        std::string cfgfile_walking;      

    private:
        void OnUpdate ();
        
        event::ConnectionPtr updateConnection;
        physics::ModelPtr model;

    };
}

#endif // POSERIGHTSHOULDER_H
