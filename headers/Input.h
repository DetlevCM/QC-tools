/*
 * Input.h
 *
 *  Created on: 19.12.2015
 *      Author: DetlevCM
 */

#ifndef HEADERS_INPUT_H_
#define HEADERS_INPUT_H_



void Handle_Filenames(
		string& ,
		int ,
		string&
);

void Process_Scan_Data_Gaussian(
		vector< OptPoints >&,
		string
);

void Process_Scan_Data_ORCA(
		vector< OptPoints >&,
		string
);


void Process_Opt_Data_Gaussian(
		vector< OptPoints >&,
		string
);

void Process_Opt_Data_ORCA(
		vector< OptPoints >&,
		string
);

#endif /* HEADERS_INPUT_H_ */
