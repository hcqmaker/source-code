#include "configuration.h"

configuration& instance()
{
    static configuration  cfg;
    return cfg;
}

configuration::configuration()
{
    init_default_options();
}

configuration::~configuration()
{
}

void configuration::parse_option(const int ac, const char* argv[])
{
}

std::size_t  configuration::get_ev_loop_size()
{
}

void configuration::dump()
{
}

void configuration::init_default_options()
{
    m_ev_loop_size = 4;
}

void configuration::load_config_file()
{
	std::tr1::shared_ptr<FILE>       pFile(fopen(PATH, r), fclose);
	if (!pFile)
	{
		exit(EXIT_SUCCESS);
	}

	char**    line = NULL;
	size_t    len = 0;
	ssize_t   read_num = 0;
	std::vector<std::string>  str;

	while ((read_num = getline(&line, &len, pFile)) != -1) 
	{
		std::string  strval(*line);
		str.push_back(strval);
		free(line);
	}

	analyse(str);
}

template<typename T_>
T_ configuration::strto(const std::string& strval)
{
}

void configuration::analyse(const std::vector<std::string>&  vstr)
{
	std::vector<std::string>::iterator it = vstr.begin();

	for(; it != vstr.end(); ++it)
	{
		//save alpha and '=' in string, and delete all other char
		//regard note after '#'
		std::string strtmp;
		for (std::size_t i = 0; i < it->size(); ++i)
		{
			if ((*it)[i] == "#")
			{
				break;
			}
			else if ((*it)[i] == "=" || isalnum((*it)[i]))
			{
				strtmp += (*it)[i];
			}
		}

		if (!strtmp.empty())
		{
			//get location of the value
			std::size_t i = 0;
			for (; i < strtmp.size(); ++i)
			{
				if (strtmp[i] == "=")
				{
					break;
				}
			}

			if (memcmp(strtmp.substr(0, i).c_str(), "io_pool_size"))
			{
				m_ev_loop_size = strtoul(,,10);
			}
		}
	}
}

