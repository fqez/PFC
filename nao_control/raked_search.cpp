//
//  shared.cpp
//  
//
//  Created by Francisco Perez on 18/02/14.
//
//

#include "raked_search.h"


	RakedSearch::RakedSearch(Shared* sm) {
		this->sm = sm;
	}

	RakedSearch::~RakedSearch() {}

//	jderobot::WalkerDataPtr getLastSearch(){}
//	jderobot::WalkerDataPtr getSearch(int index){}


	void RakedSearch::writeIndex() {

		//std::cout << "writing index" << std::endl;
		//Creates an instance of ofstream, and opens example.txt
	  	std::ofstream a_file ( "index.txt" );
	  	// Outputs to index.txt through a_file
	  	a_file<<"index#"<<index<<"\n";
	  	a_file<<"for1:"<<for1<<"\n";
	  	a_file<<"for2:"<<for2<<"\n";
	  	a_file<<"for3:"<<for3<<"\n";
	  	a_file<<"for4:"<<for4<<"\n";
	  	a_file<<"for5:"<<for5<<"\n";
	  	a_file<<"for6:"<<for6<<"\n";
	  	a_file<<"for7:"<<for7<<"\n";
	  	a_file<<"for8:"<<for8<<"\n";
	  	a_file<<"for9:"<<for9<<"\n";
	  	a_file<<"for10:"<<for10<<"\n";
	  	// Close the file stream explicitly
	  	a_file.close();
	  
	}

	std::string splitStr(std::string str, std::string delimiter) {
		std::vector<std::string> strs;
		boost::split(strs, str, boost::is_any_of(delimiter));
		return strs[1];
	}
	
	void RakedSearch::readIndex() {
		
		std::string str;	
		std::cout << "reading index" << std::endl;

	  	std::ifstream b_file ( "index.txt" );
	  	b_file>> str;
		this->index = boost::lexical_cast<int>(splitStr(str, "#"));

	  	b_file>> str;
		this->for1 = boost::lexical_cast<int>(splitStr(str, ":"));

		b_file>> str;
		this->for2 = boost::lexical_cast<int>(splitStr(str, ":"));

		b_file>> str;
		this->for3 = boost::lexical_cast<int>(splitStr(str, ":"));

		b_file>> str;
		this->for4 = boost::lexical_cast<int>(splitStr(str, ":"));

		b_file>> str;
		this->for5 = boost::lexical_cast<int>(splitStr(str, ":"));

		b_file>> str;
		this->for6 = boost::lexical_cast<int>(splitStr(str, ":"));

		b_file>> str;
		this->for7 = boost::lexical_cast<int>(splitStr(str, ":"));

		b_file>> str;
		this->for8 = boost::lexical_cast<int>(splitStr(str, ":"));

		b_file>> str;
		this->for9 = boost::lexical_cast<int>(splitStr(str, ":"));

		b_file>> str;
		this->for10 = boost::lexical_cast<int>(splitStr(str, ":"));
	
	  	std::cout<< index <<"\n" << for1 <<"\n" << for2 <<"\n" << for3 <<"\n" << for4 <<"\n" << for5 <<"\n" << for6 <<"\n" << for7 <<"\n" << for8 <<"\n" << for9 <<"\n" << for10 << std::endl;
		
	}

	bool RakedSearch::checkPaused() {
		if (this->sm->isPaused()) {
			std::cout << "parado" << std::endl;
			//save indexes
			writeIndex();
			return true;	
		}else{
			return false;
		}
	}

	void RakedSearch::searchAll(bool reset) {

		std::cout << "searching all" << std::endl;


		std::ofstream search_file;
		if (reset) {
			initialize();
			std::cout << "writing searches" << std::endl;
        	search_file.open( "searches.txt", std::ofstream::trunc );
		}
		else {
			readIndex();
			std::cout << "reading searches" << std::endl;
			search_file.open( "searches.txt", std::ofstream::app );
		}

		int for1a = for1;
		int for2a = for2;
		int for3a = for3;
		int for4a = for4;
		int for5a = for5;
		int for6a = for6;
		int for7a = for7;
		int for8a = for8;
		int for9a = for9;
		int for10a = for10;

				
		for(for1a = my_params.w.min ; for1a < my_params.w.max; for1a = for1a + my_params.w.inc) {
			if(checkPaused()) {break;}
			if(!reset) {for1a = for1;}
			for(for2a = my_params.hip_amplitude.min; for2a < my_params.hip_amplitude.max; for2a = for2a + my_params.hip_amplitude.inc) {
				if(checkPaused()) {break;}
				if(!reset) {for2a = for2;}
				for(for3a = my_params.hip_shift.min; for3a < my_params.hip_shift.max; for3a = for3a + my_params.hip_shift.inc) {
					if(checkPaused()) {break;}
					if(!reset) {for3a = for3;}
					for(for4a =my_params.knee_amplitude.min ; for4a < my_params.knee_amplitude.max; for4a = for4a + my_params.knee_amplitude.inc) {
						if(checkPaused()) {break;}
						if(!reset) {for4a = for4;}
						for(for5a = my_params.knee_phase.min; for5a < my_params.knee_phase.max; for5a = for5a + my_params.knee_phase.inc) {
							if(checkPaused()) {break;}
							if(!reset) {for5a = for5;}
							for(for6a = my_params.knee_shift.min; for6a < my_params.knee_shift.max; for6a = for6a + my_params.knee_shift.inc) {
								if(checkPaused()) {break;}
								if(!reset) {for6a = for6;}
								for(for7a = my_params.ankle_amplitude.min; for7a < my_params.ankle_amplitude.max; for7a = for7a + my_params.ankle_amplitude.inc) {
									if(checkPaused()) {break;}
									if(!reset) {for7a = for7;}
									for(for8a =my_params.ankle_phase.min ; for8a < my_params.ankle_phase.max; for8a = for8a + my_params.ankle_phase.inc) {
										if(checkPaused()) {break;}
										if(!reset) {for8a = for8;}
										for(for9a =my_params.ankle_shift.min ; for9a < my_params.ankle_shift.max; for9a = for9a + my_params.ankle_shift.inc) {
											if(checkPaused()) {break;}
											if(!reset) {for9a = for9;}
											for(for10a =my_params.balance.min ; for10a < my_params.balance.max; for10a = for10a + my_params.balance.inc) {


												if(checkPaused()) {
													break;
												}else{
	
													if(!reset) {
														for10a = for10; 
														reset = true;
													}


//std::cout<< index <<"\n" << for1a <<"\n" << for2a <<"\n" << for3a <<"\n" << for4a <<"\n" << for5a <<"\n" << for6a <<"\n" << for7a <<"\n" << for8a <<"\n" << for9a <<"\n" << for10a << std::endl;
//std::cout << "-------------------------" << std::endl;

													jderobot::WalkerData* comb = new jderobot::WalkerData();
													this->index++;		

													float aux = for1a/10.0;  	
												
													//guardar valores en memoria compartida (vector)
													comb->param1 = aux;
													comb->param2 = for2a;
													comb->param3 = for3a;
													comb->param4 = for4a;
													comb->param5 = for5a;  //valores de PI
													comb->param6 = for6a;
													comb->param7 = for7a;
													comb->param8 = for8a;   //valores de PI
													comb->param9 = for9a;
													comb->param10 = for10a;
		
													this->sm->addSearch(comb);
												
													//save the lastest 100 searches. For security
													if (index%100 == 0) {
														//std::cout << "processed " << index << " searches" << std::endl;
														writeIndex();

													}

													search_file<<"Search#"<<index<<"\n";
													search_file<<"param1:"<<aux<<"\n";
												  	search_file<<"param2:"<<for2a<<"\n";
												  	search_file<<"param3:"<<for3a<<"\n";
												  	search_file<<"param4:"<<for4a<<"\n";
												  	search_file<<"param5:"<<for5a<<"\n";
												  	search_file<<"param6:"<<for6a<<"\n";
												  	search_file<<"param7:"<<for7a<<"\n";
												  	search_file<<"param8:"<<for8a<<"\n";
												  	search_file<<"param9:"<<for9a<<"\n";
												  	search_file<<"param10:"<<for10a<<"\n\n";

												
												}
											}
										}
									}
								}
							}
						}
					}	
				}
			}
		}
		
		std::cout << "busqueda finalizada" << std::endl;
		std::cout << "generadas " << index << " busquedas" << std::endl;		

	  	search_file.close();

		
	}

	void RakedSearch::initialize() {

		this->index = 0;

		my_params.w.min = 6;
		my_params.w.max = 26;
		my_params.w.inc = 3;
		for1 = my_params.w.min;

		my_params.hip_amplitude.min = 0;
		my_params.hip_amplitude.max = 62;
		my_params.hip_amplitude.inc = 15;
		for2 = my_params.hip_amplitude.min;

		my_params.hip_shift.min = -100;
		my_params.hip_shift.max = 25;
		my_params.hip_shift.inc = 35;
		for3 = my_params.hip_shift.min;

		my_params.knee_amplitude.min = 0;
		my_params.knee_amplitude.max = 65;
		my_params.knee_amplitude.inc = 10;
		for4 = my_params.knee_amplitude.min;

		my_params.knee_phase.min = -2*M_PI;
		my_params.knee_phase.max = 2*M_PI;
		my_params.knee_phase.inc = 4;
		for5 = my_params.knee_phase.min;

		my_params.knee_shift.min = 0;
		my_params.knee_shift.max = 130;
		my_params.knee_shift.inc = 40;
		for6 = my_params.knee_shift.min;

		my_params.ankle_amplitude.min = 0;
		my_params.ankle_amplitude.max = 60;
		my_params.ankle_amplitude.inc = 20;
		for7 = my_params.ankle_amplitude.min;

		my_params.ankle_phase.min = -M_PI;
		my_params.ankle_phase.max = M_PI;
		my_params.ankle_phase.inc = 4;
		for8 = my_params.ankle_phase.min;

		my_params.ankle_shift.min = -75;
		my_params.ankle_shift.max = 35;
		my_params.ankle_shift.inc = 45;
		for9 = my_params.ankle_shift.min;

		my_params.balance.min = 0;
		my_params.balance.max = 100;
		my_params.balance.inc = 30;
		for10 = my_params.balance.min;

		
	}





