/*
 *  Copyright (C) 1997-2012 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
 *            Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>	
 *  		 Javier Vazquez Pereda <javiervarper@yahoo.es>
 */

#ifndef WALKER_ICE
#define WALKER_ICE

#include <common.ice>

module jderobot{  

	class WalkerData
  	{
		float param1;
		float param2;
		float param3;
  		float param4;
		float param5;
		float param6;
		float param7;
		float param8;
		float param9;
		float param10;
  	};

	class StadisticsData
	{
		int id;
		float x0;
		float y0;
		float z0;
		float x1;
		float y1;
		float z1;
		float simTime;
		float distance;
		float distanceX;
		float distanceY;
		double desviation;
		int fallen;
		float fitness;
	};

	/** 
   	* Interface to the walker activator.
   	*/
	interface Walker
	{
		int startWalk();
		int stopWalk();
		int setParams(WalkerData data);
		StadisticsData getStadistics();
	};


}; //module

#endif //Pose3DMOTORS_ICE
