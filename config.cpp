#include "config.h"

using namespace std;

namespace config
{
	std::string mode = "";
	
	std::string menu_file = "menu.txt";
	std::string group_file = "group.txt";
	std::string output_file = "";
	unsigned int limit = 0;
	bool allow_repeat = false;

	void initProgramOption(ProgramOption& po)
	{
		po.reset();
		po.addOption(Option(mode, load<string>)
			.is_default_arg("mode")
			.description("running mode, accept: Group(G), Calculation(Calc, C)")
		);
		po.addOption(Option(menu_file, load<string>)
			.keys("menu", 'm')
			.name("menu_file")
			.is_optional()
			.description("menu file to load, default: menu.txt")
		);
		po.addOption(Option(group_file, load<string>)
			.keys("group", 'g')
			.name("group_file")
			.is_optional()
			.description("group file to load, default: group.txt")
		);
		po.addOption(Option(output_file, load<string>)
			.keys("output", 'o')
			.name("output_file")
			.is_optional()
			.description("output file to write result, default: stdout")
		);
		po.addOption(Option(limit, load<unsigned int>)
			.keys("limit", 'l')
			.name("limit")
			.is_optional()
			.description("maximum difference from 180, default: 0")
		);
		po.addOption(Option(new ValueSetter<bool>(allow_repeat, true))
			.keys("repeat", 'r')
			.is_optional()
			.no_arg()
			.description("allow repeat item in order, default: false")
		);
		
		po.addOption(Option(po.invoke_help(cerr))
			.keys("help", 'h')
			.is_optional()
			.no_arg()
			.description("show this text and exit")
		);
	}

}