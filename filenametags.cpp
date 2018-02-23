#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* args[])
{
	if (argc < 2)
	{
		cout <<"Error: Input Error, you should input filenametags.exe <dir>" << endl ;
		return -1;
	}
	
	string dirName = string(args[1]);
	//cout << dirName.c_str() << endl;

	//one: excute system cmd and create a file
	string command("cd /d ");
	command.append(dirName.c_str());
	//command.append("&& dir /B /S /A-D /ON .*.vim > filenametags_bak");
	command.append("&& dir /B /S /A-D /ON *.cpp *.js *.c *.h *.hpp *.h++ *.json *.html *.md *.txt *.sh > filenametags_bak");
	
	system(command.c_str());

	//two: create filenametags
	string fileTagsName = string(dirName);
	fileTagsName.append("/filenametags");
	remove(fileTagsName.c_str());
	ofstream fileTagsOstream(fileTagsName.c_str());
	if (!fileTagsOstream.is_open())
	{
		cout << "System error" << endl;
		return -1;
	}
	fileTagsOstream << "!_TAG_FILE_SORTED   2   //2=foldcase//" << endl;

	//three: open bakFileTagsName
	ifstream bakFileTagsOstream;
	string bakFileTagsName = string(dirName);
	bakFileTagsName.append("/filenametags_bak");
	bakFileTagsOstream.open(bakFileTagsName.c_str(), ios::out | ios::app | ios::binary);
	if (!bakFileTagsOstream.is_open())
	{
		cout << "System error" << endl;
		fileTagsOstream.close();
		return -1;
	}

	//four: 循环读入bakFileTagsName，循环写入filenametags
	while (!bakFileTagsOstream.eof())
	{
		int firstPos = 0;
		int lastPos = 0;
		char tmpBuf[1024] = { 0 };
		
		//four.one: get line
		bakFileTagsOstream.getline(tmpBuf, 1024, '\n');
		string tmpStr(tmpBuf);
		firstPos = tmpStr.find_last_of('\\', 1024);
		lastPos = tmpStr.find_last_of('\r', 1024);
		if ((firstPos < 0) || (lastPos < 0))
		{
			continue;
		}
		
		//four.two toWriteStr
		//cout << tmpStr.substr(firstPos + 1, lastPos - firstPos - 1).c_str() << '\t' << tmpStr.substr(0, lastPos - 1).c_str() << '\t' << firstPos + 1 << lastPos - 1 << endl;
		fileTagsOstream << tmpStr.substr(firstPos + 1, lastPos - firstPos - 1).c_str() << '\t' << tmpStr.substr(0, lastPos).c_str() << '\t' << 1 << endl;
	}

	fileTagsOstream.close();
	bakFileTagsOstream.close();
	remove(bakFileTagsName.c_str());

	return 0;
}