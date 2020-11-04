#include "ini.h"

namespace Ini
{
	Section& Document::AddSection(std::string name)
	{
		return sections[name];
	}

	const Section& Document::GetSection(const std::string& name) const
	{
		return sections.at(name);
	}

	size_t Document::SectionCount() const
	{
		return sections.size();
	}

	
	Document Load(std::istream& input)
	{
		Document doc;
		std::string line, section_name;
		while (getline(input, line))
		{
			if (!line.empty())
			{
				if (line[0] == '[')
				{
					section_name = line.substr(1, line.size() - 2);
					doc.AddSection(section_name);
				}
				else
				{
					std::size_t del = line.find('=');
					std::string key = line.substr(0, del);
					std::string val = line.substr(del + 1);
					doc.AddSection(section_name).insert({ key, val });
				}
			}
		}

		return doc;
	}
}
