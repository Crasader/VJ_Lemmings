#include "TextProcessor.h"





TextProcessor::~TextProcessor() {}

void TextProcessor::loadFileAndProcess(string path) {
	try {
		file.close();
		file.open(path);
		processText();
	}
	catch (int e) {
		cout << "unable to open file." << endl;
	}
}

void TextProcessor::processText() {

	string line;
	getline(file, line);
	if (line == "DOCTYPE LEMMINGSMAP") {
		while (getline(file, line))
		{
			if (line == "ENDLEMMINGSMAP") {
				break;
			}
			else {
				std::istringstream iss(line);
				string tag;
				iss >> tag;
				if (tag == "LVLNUMB") {
					string var;
					iss >> var;
					copyStringToVar(var, levelNumber);
					cout <<"level"<< levelNumber << " loaded" << endl;
				}
				else if (tag == "LVLNAME") {
					string var;
					iss >> var;
					copyStringToString(var, levelName);
				}
				else if (tag == "LEMMINGSNUM") {
					string var;
					iss >> var;
					copyStringToVar(var, lemmings);
				}
				else if (tag == "MINLEMMINGS") {
					string var;
					iss >> var;
					copyStringToVar(var, minLemmings);
				}
				else if (tag == "MAXTIME") {
					string var;
					iss >> var;
					copyStringToVar(var, maxTime);
				}
				else if (tag == "SPAWNRATE") {
					string var;
					iss >> var;
					copyStringToVar(var, spawnrate);
				}
				else if (tag == "MWIDTH") {
					string var;
					iss >> var;
					copyStringToVar(var, width);
				}
				else if (tag == "MHEIGHT") {
					string var;
					iss >> var;
					copyStringToVar(var, height);
				}
				else if (tag == "PATH") {
					string var;
					iss >> var;
					copyStringToString(var, path);
				}
				else if (tag == "MPATH") {
					string var;
					iss >> var;
					copyStringToString(var, mPath);
				}
				else if (tag == "CAM") {
					string var1, var2;
					iss >> var1;
					iss >> var2;
					copyStringTovec2(var1, var2,camPos);
				}
				else if (tag == "DSTART") {
					string var1, var2;
					iss >> var1;
					iss >> var2;
					copyStringTovec2(var1, var2, startDoor);
				}
				else if (tag == "DEND") {
					string var1, var2;
					iss >> var1;
					iss >> var2;
					copyStringTovec2(var1, var2, endDoor);
				}
				else if (tag == "DSCOLOR") {
					string var;
					iss >> var;
					copyStringToVar(var, doorStartColor);
				}
				else if (tag == "DECOLOR") {
					string var;
					iss >> var;
					copyStringToVar(var, doorEndColor);
				}
				else if (tag == "LEMSTART") {
					string var1, var2;
					iss >> var1;
					iss >> var2;
					copyStringTovec2(var1, var2, lemmingsStart);
				}
				else if (tag == "LEMEND") {
					string var1, var2;
					iss >> var1;
					iss >> var2;
					copyStringTovec2(var1, var2, lemmingsEnd);
				}
				else if (tag == "NUMDIG") {
					string var;
					iss >> var;
					copyStringToVar(var, numbDig);
				}
				else if (tag == "NUMSTOP") {
					string var;
					iss >> var;
					copyStringToVar(var, numbStop);
				} 
				else if (tag == "NUMBASH") {
					string var;
					iss >> var;
					copyStringToVar(var, numbBash);
				}
				else if (tag == "NUMBCLI") {
					string var;
					iss >> var;
					copyStringToVar(var, numbCli);
				}
				else if (tag == "NUMBFLO") {
					string var;
					iss >> var;
					copyStringToVar(var, numbFlo);
				}
				else if (tag == "NUMBBOM") {
					string var;
					iss >> var;
					copyStringToVar(var, numbBomb);
				}
				else if (tag == "NUMBBUI") {
					string var;
					iss >> var;
					copyStringToVar(var, numbBuild);
				}
				else if (tag == "BOMBPOS") {
					string var1, var2;
					iss >> var1;
					iss >> var2;
					copyStringTovec2(var1, var2, bombPos);
				}
				else if (tag == "ISBOMB") {
					string var;
					iss >> var;
					copyStringToVar(var, isBomb);
				}
				else if (tag == "MUSIC"){
					string var;
					iss >> var;
					copyStringToString(var, musicPath);
				}
				else if (tag == "BLUEPOS") {
					string var1, var2;
					iss >> var1;
					iss >> var2;
					copyStringTovec2(var1, var2, bluePos);
				}
				else if (tag == "ORANGEPOS") {
					string var1, var2;
					iss >> var1;
					iss >> var2;
					copyStringTovec2(var1, var2, orangePos);
				}
				else if (tag == "ISPORTAL") {
					string var;
					iss >> var;
					copyStringToVar(var, isPortal);
				}
			}
		}
	}
	else cout << "this is not a lemmings map" << endl;
}

void TextProcessor::copyStringToVar(string var, int & variable)
{
	std::string::size_type sz;
	variable = std::stoi(var, &sz);
}

void TextProcessor::copyStringToString(string var, string & variable)
{
	variable = var;
}

void TextProcessor::copyStringTovec2(string var1, string var2, glm::ivec2 & vector)
{
	std::string::size_type sz;
	vector.x = std::stoi(var1, &sz);
	vector.y = std::stoi(var2, &sz);
	
}
