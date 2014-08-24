#ifndef CONFIG_H
#define CONFIG_H

#include "ProgramOption/ProgramOption.h"
#include <string>

namespace config
{
	extern std::string mode;
	
	extern std::string menu_file;
	extern std::string group_file;
	extern std::string output_file;

	extern unsigned int limit ;
	
	extern bool allow_repeat;
	
	extern void initProgramOption(ProgramOption& po);

}

#endif // CONFIG_H
