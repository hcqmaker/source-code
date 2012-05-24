#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#include <memory>
#include <vector>
#include <string>
#include <stdlib.h>

const std::string PATH = "config.ini";

class configuration
{
public:
    static configuration& instance();
    void parse_option(const int ac, const char* argv[]);
    std::size_t  get_ev_loop_size();
    void dump();

private:
    configuration();
    ~configuration();
    configuration(const configuration&);
    configuration operator=(const configuration&);

private:
    void init_default_options();

    void load_config_file();

	void analyse(const std::vector<std::string>&  vstr);

private:
    std::size_t      m_ev_loop_size;
};

#endif //CONFIGURATION_H_
