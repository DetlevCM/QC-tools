/*
 * main.cpp
 *
 *  Created on: 22 July 2015
 *      Author: DetlevCM
 */

#include<Headers.h>


// Not sure if I should consider the locale...
// http://stackoverflow.com/questions/15220861/how-can-i-set-the-comma-to-be-a-decimal-point

int main(int argc, char* argv[])
{
	//string filename;
	string input;
	int type = 0;	// output type 0 = csv , 1 = txt (default)

	bool InputIsFilename = true;


	while(true) // loop until user quits
	{
		cout << "Please enter the filename.\n";// - the ending may be included or omitted.\n";
		cout << "The filetype output can be changed by inputting 'csv' (default) or 'txt'. \n";

		vector< OptPoints > GaussianData;
		//GaussianData.clear();
		//cin >> input;
		getline(cin, input);

		if(strcmp(input.c_str(),"quit")==0) // user wants to quit
		{
			//EndProgram = true;
			return 0;
		}
		InputIsFilename = true;

		if(strcmp(input.c_str(),"csv")==0) // user wants csv, default
		{
			type = 0;
			InputIsFilename = false;
		}
		if(strcmp(input.c_str(),"txt")==0) // user wants csv, default
		{
			type = 1;
			InputIsFilename = false;
		}

		if(InputIsFilename)// || !filename.empty())
		{

			string input_filename, output_filename;

			input_filename = input;

			// variables by reference, hence filenames updated in void
			Handle_Filenames(input_filename, type, output_filename);



			ifstream DataInputFile;
			DataInputFile.open(input_filename.c_str());


			if (DataInputFile.is_open()) {


				// check if scan or opt
				bool IsScan;
				IsScan = false;

				// expanding to support two QC codes, quick simple fix
				bool IsGaussian = false;
				bool IsORCA = false;

				while(DataInputFile.good())
				{

					string line;
					getline(DataInputFile,line);


					// this is the first line for a Gaussian Input
					if(line.find("Entering Gaussian System")!=string::npos){
						IsGaussian = true;
					}
					// This is the start of an ORCA file
					if(line.find("* O   R   C   A *")!=string::npos){
						IsORCA = true;
					}

					// the scan keyword exists for both ORCA and Gaussian, so retain
					if(line.find("Scan")!=string::npos)
					{
						IsScan = true;
					}

				}
				DataInputFile.close();

				// These two functions are the previous model for Gaussian
				if(IsScan == true && IsGaussian == true)
				{
					Process_Scan_Data_Gaussian(GaussianData, input_filename.c_str());
				}
				if(IsScan == false && IsGaussian == true)
				{
					Process_Opt_Data_Gaussian(GaussianData, input_filename.c_str());
				}

				if(IsScan == true && IsORCA == true)
				{
					Process_Scan_Data_ORCA(GaussianData, input_filename.c_str());
				}
				if(IsScan == false && IsORCA == true)
				{
					Process_Opt_Data_ORCA(GaussianData, input_filename.c_str());
				}

				// Now we can print the data, from 'GaussianData' and 'Atom_Pairs'
				//PrintOutput_Scan(GaussianData, Atom_Pairs, output_filename, type);
				if(GaussianData.size() > 0) // seems to fail if GaussianData is empty
				{


					// needs a "jagged array"
					// 2 atoms = distance, 3 atoms = valence angle, 4 atoms = dihedral
					vector< vector < int > > SelectedAtoms;

					/* Now we need to process the read in data to print
					 * the output the user desires:
					 */

					cout 	<< "Energy is output by default.\n"
							<< "Enter the atom IDs to be investigated.\n"
							<< "e.g. 1 2 'enter' for the distance\n"
							<< "e.g. 1 2 3 'enter' for the valence angle on 2\n"
							<< "e.g. 1 2 3 4 'enter' for the dihedral angle\n"
							<< "Finish the input by submitting 'print'.\n";


					// Handling user input of atom IDs
					while(true)
					{

						getline(cin,input);

						// user wants to print output, break loop
						if(input.compare("print") == 0 || input.compare("p") == 0 )
						{
							break;
						}

						// user wants to quit
						if(input.compare("quit") == 0)
						{
							cout << "Really close program?\n";
							getline(cin,input);
							// verify user really wants to quit
							if(input.compare("yes") == 0)
							{
								return 0;
							}
						}

						// user isn't quiting, so time to handle the input
						char * cstr, *p;
						string str = input;
						vector< double > temp;

						cstr = new char [str.size()+1];
						strcpy (cstr, str.c_str());

						p=strtok (cstr," 	,;.:");
						while (p!=NULL)
						{
							temp.push_back(strtod(p,NULL));
							p=strtok(NULL," 	,;.:");
						}
						delete[] cstr;
						delete[] p;

						// strtod is 0 where no valid conversion is possible
						// Atom number must 2 for distance, 3 for valence angle, 4 for dihedral
						if( (int)temp.size() >= 2 && (int) temp.size() <=4)
						{
							bool NoZeroEntry=true;
							int i;
							for(i=0;i<(int)temp.size();i++)
							{
								if(temp[i] == 0) // check all converted correctly, Gaussian starts atom counts at 1
								{
									NoZeroEntry = false;
								}
							}

							if(NoZeroEntry)
							{
								vector< int > AtomPicks;
								for(i=0;i<(int)temp.size();i++)
								{
									AtomPicks.push_back(temp[i]);
								}
								SelectedAtoms.push_back(AtomPicks);
								AtomPicks.clear();
							}
						}
						else
						{
							if(temp.size() != 0) // zero lenght would mean "just print energies"
							{
								cout << "Number of atoms entered is too large or small.\n";
							}
						}
					}


					if(IsScan && IsGaussian)
					{
						PrintOutput_Scan(GaussianData, SelectedAtoms, output_filename, type);
					}
					else
					{
						PrintOutput_Opt(GaussianData, SelectedAtoms, output_filename, type);
					}
					GaussianData.clear();
				}
				else
				{
					cout << "Error reading in logfile - molecule definitions are empty.\n";
				}

			}
			else cout << "Unable to open input file\n";
		}
	}

	return 0;
}






