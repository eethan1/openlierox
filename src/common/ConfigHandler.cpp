/////////////////////////////////////////
//
//   OpenLieroX
//
//   Auxiliary Software class library
//
//   based on the work of JasonB
//   enhanced by Dark Charlie and Albert Zeyer
//
//   code under LGPL
//
/////////////////////////////////////////


// Config file handler
// Created 30/9/01
// By Jason Boettcher


#include "defs.h"
#include "LieroX.h"


int			NumKeywords = 0;
keyword_t	Keywords[MAX_KEYWORDS];


///////////////////
// Add a keyword to the list
int AddKeyword(std::string& key, int value)
{
	// Check for enough spaces
	if(NumKeywords >= MAX_KEYWORDS-1)
		return false;

	Keywords[NumKeywords].key = key;
	Keywords[NumKeywords++].Value = value;

	return true;
}



///////////////////
// Read a keyword from a file
int ReadKeyword(const std::string& filename, const std::string& section, const std::string& key, int *value, int defaultv)
{
	int n;
	static std::string string;
	
	*value = defaultv;

	if(!GetString(filename,section,key,string))
		return false;

	// Try and find a keyword with matching keys
	for(n=0;n<NumKeywords;n++) {
		if(stringcasecmp(string,Keywords[n].key) == 0) {
			*value = Keywords[n].Value;
			return true;
		}
	}

	return false;
}

///////////////////
// Read a keyword from a file (bool version)
bool ReadKeyword(const std::string& filename, const std::string& section, const std::string& key, bool *value, bool defaultv)
{
	int n;
	static std::string string;
	
	*value = defaultv;

	if(!GetString(filename,section,key,string))
		return false;

	// Try and find a keyword with matching keys
	for(n=0;n<NumKeywords;n++) {
		if(stringcasecmp(string,Keywords[n].key) == 0) {
			*value = Keywords[n].Value != 0;
			return true;
		}
	}

	return false;
}


///////////////////
// Read an interger from a file
int ReadInteger(const std::string& filename, const std::string& section, const std::string& key, int *value, int defaultv)
{
	static std::string string;

	*value = defaultv;
	
	if(!GetString(filename,section,key,string))
		return false;
	

	// TODO
	*value = atoi(string.c_str());//from_string(string);

	return true;
}


///////////////////
// Read a string from a file
int ReadString(const std::string& filename, const std::string& section, const std::string& key, std::string& value, const std::string& defaultv)
{
	value = defaultv;

	return GetString(filename,section,key,value);

	/*int result = GetString(filename,section,key,value);

	if (strlen(value) <= 0)
		strcpy(value,defaultv);

	return result;*/
}


///////////////////
// Read a float from a file
int ReadFloat(const std::string& filename, const std::string& section, const std::string& key, float *value, float defaultv)
{
	static std::string string;

	*value = defaultv;
	
	if(!GetString(filename,section,key,string))
		return false;

	*value = (float)atof(string);

	return true;
}


//////////////////
// Read a colour
int ReadColour(const std::string& filename, const std::string& section, const std::string& key, Uint32 *value, Uint32 defaultv)
{
	static std::string string;

	*value = defaultv;
	
	if(!GetString(filename,section,key,string))
		return false;

	*value = StrToCol(string);

	return true;
	
}

//////////////////
// Reads an array of integers
int ReadIntArray(const std::string& filename, const std::string& section, const std::string& key, int *array, int num_items)
{
	static std::string string;

	if (!GetString(filename,section,key,string,MAX_MINOR_LENGTH))
		return false;

	// TODO: use explode instead
	std::string tok = strtok(string.c_str(),",");
	int i=0;
	while(tok != "" && i < num_items)  {
		array[i++] = atoi(tok);
		tok = strtok(NULL,",");
	}

	return i == num_items-1;
}



///////////////////
// Read a string
// HINT: string has to be MAX_MINOR_LENGTH long
int GetString(const std::string& filename, const std::string& section, const std::string& key, std::string& string)
{
	FILE	*config;
	// TODO!
	static std::string	Line;
	static std::string	tmpLine;
	static std::string	curSection;
	static std::string	temp;
	static std::string	curKey;
	size_t	chardest = 0;
	int		Position;
	int		found = false;
	
	if(filename == "")
		return false;

	config = OpenGameFile(filename,"rt");
	if(!config)	
		return false;

	string="";
	curSection="";
	temp="";
	curKey="";


	while(!feof(config))
	{
		// Parse the lines
		tmpLine = ReadUntil(config, '\n');
		//fscanf(config,"%[^\n]\n",tmpLine);
		TrimSpaces(tmpLine);
		Line = tmpLine;
		
		///////////////////
		// Comment, Ignore
		if(Line.size() == 0 || Line[0] == '#')				
			continue;

		////////////
		// Sections
		if(Line[0] == '[' && Line[Line.size()-2] == ']')
		{
			temp = Line.substr(1);
			temp.erase(temp.size()-2);
			curSection = temp;
			continue;
		}

		////////
		// Keys
		chardest = Line.find('=');
		if(chardest != std::string::npos)
		{
			// Key
			Position = chardest;
			tmpLine = Line;
			tmpLine.erase(Position);
			TrimSpaces(tmpLine);
			curKey = tmpLine;
			
			// Check if this is the key were looking for under the section were looking for
			if(stringcasecmp(curKey,key) == 0 && stringcasecmp(curSection,section) == 0)
			{				
				// Get the value
				tmpLine = Line.substr(Position+1);
				TrimSpaces(tmpLine);
				string = tmpLine;
				found = true;
				break;
			}
			continue;
		}
	}

	fclose(config);

	return found;
}



///////////////////
// Trim the spaces from the start & end of a string
/*void TrimSpaces(std::string& str)
{
	if(!str)
		return;

	char temp[MAX_STRING_LENGTH];
	unsigned int n = 0;
	unsigned int i = 0;
	strcpy(temp,"");


	// Leading spaces
	for(n=0,i=0;n<strlen(str);n++)
	{
		if(str[n] != ' ' && temp[n] != '\t')
			break;
		i++;
	}
	fix_strncpy(temp,str+i);


	// proceeding spaces
	for(n=strlen(temp)-1;n>0;n--)
		if(temp[n] != ' ' && temp[n] != '\t')
		{
			temp[n+1] = '\0';
			break;
		}

	strcpy(str,temp);
}*/
