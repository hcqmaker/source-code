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
}

