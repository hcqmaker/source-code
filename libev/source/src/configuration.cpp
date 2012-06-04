#include "configuration.h"
#include <memory>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

configuration& configuration::instance()
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
    load_config_file();
    for (int i = 1; i < ac; ++i)
    {
        size_t  len = strlen(argv[i]) - 2;
        if (0 == memcmp(&argv[i][2], "io_pool_size", len))
        {
            m_ev_loop_size = strtoul(&argv[i][2], NULL, 0);
        }
    }
}

std::size_t  configuration::get_ev_loop_size()
{
    return m_ev_loop_size;
}

void configuration::dump()
{
    std::cout << "++++++++++++++++++++++++++++++begin dump configuration information+++++++++++++++++++++" << std::endl;
    std::cout << "io_service_pool's size = " << m_ev_loop_size << std::endl;
    std::cout << "++++++++++++++++++++++++++++++ending dump configuration information+++++++++++++++++++++" << std::endl;
}

void configuration::init_default_options()
{
    m_ev_loop_size = 4;
}

void configuration::load_config_file()
{
	std::shared_ptr<FILE>       pFile(fopen(PATH.c_str(), "r"), fclose);
	if (!pFile)
	{
		exit(EXIT_SUCCESS);
	}

	char*    line = NULL;
	size_t    len = 0;
	ssize_t   read_num = 0;
	std::vector<std::string>  str;

	while ((read_num = getline(&line, &len, pFile.get())) != -1) 
	{
		std::string  strval(line);
		str.push_back(strval);
		free(line);
        line = NULL;
	}

	analyse(str);
}

void configuration::analyse(const std::vector<std::string>&  vstr)
{
	std::vector<std::string>::const_iterator it = vstr.begin();

	for(; it != vstr.end(); ++it)
	{
		//save alpha and '=' in string, and delete all other char
		//regard note after '#'
		std::string strtmp;
		for (std::size_t i = 0; i < it->size(); ++i)
		{
			if ((*it)[i] == '#')
			{
				break;
			}
			else if ((*it)[i] == '=' || isalnum((*it)[i]))
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
				if (strtmp[i] == '=' || i == strtmp.size() - 1)
				{
					break;
				}
			}

            const char*  sub = strtmp.substr(0, i).c_str();
			if (0 == memcmp(sub, "io_pool_size", i))
			{
				m_ev_loop_size = strtoul(sub, NULL, 0);
			}
		}
	}
}

