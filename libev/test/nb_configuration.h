/**************************************************************************
**
**  Copyright 2010 Duke Inc.
**
**************************************************************************/

#ifndef _NB_CONFIGURATION_H_
#define _NB_CONFIGURATION_H_

#include <iostream>
#include <string>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "nb_version.h"
#include "nb_net_tool.h"
#include "default_value.h"

//config filename
const std::string Default_Config_Filename = "config.ini";

enum nb_server_type
{
    nb_core_server,
    nb_bridge_server,
    nb_inspector_server,
};

const std::string nb_server_type_str[] = 
{
    "bridge",
    "master",
    "slave",
    "unkown"
};

class nb_configuration
{
private:
    nb_configuration(void)
    {
        init_default_options();
    }

    ~nb_configuration(void)
    {
    }
    
    nb_configuration(const nb_configuration&);
    nb_configuration& operator=(const nb_configuration&);

public:
    static nb_configuration& instance()
    {
        static nb_configuration config;
        return config;
    }

public:
    void parse_option(int ac, char* av[])
    {
        // load options from config file first
        load_config_file();

        // overwrite the configuration with cmdline options
        if (ac > 1)
        {
            boost::program_options::options_description options("Nebutown server Daemon start options");
            boost::program_options::variables_map vm;

            try
            {            
                options.add_options()
                    ("help", "print help message")
                    
                    ("version", "print version")

                    ("addr",
                     boost::program_options::value<std::string>(&m_addr),
                     "set core listen address for core communication")

                    ("port",
                     boost::program_options::value<std::string>(&m_port),
                     "set core listen port number for core communication")

                    ("ac_pool",
                     boost::program_options::value<std::size_t>(&m_actor_thread_pool),
                     "set actor thread pool size")
                    
                    ("sock_pool",
                     boost::program_options::value<std::size_t>(&m_socket_thread_pool),
                     "set socket thread pool size")
                    
                    ("gc",
                     boost::program_options::value<bool>(&m_is_gc),
                     "set gabage collection threshold in MB")

                    ("gc_size",
                     boost::program_options::value<std::size_t>(&m_gc_size),
                     "set gabage collection threshold in MB")
                    
                    ("gc_rate",
                     boost::program_options::value<std::size_t>(&m_gc_rate),
                     "set gabage collection threshold in percent")
                    
                    ("st",
                     boost::program_options::value<bool>(&m_is_statistics),
                     "enable statistics thread for ac_manager")
                    
                    ("interval",
                     boost::program_options::value<std::size_t>(&m_statistics_interval),
                     "set statistics thread execution interval")
                    
                    ("log",
                     boost::program_options::value<std::size_t>(&m_log_level),
                     "set log level")
                    
                    ("chkpoint",
                     boost::program_options::value<std::size_t>(&m_db_chkpoint_interval),
                     "set database checkpoint interval")
                    
                    ("baddr",
                     boost::program_options::value<std::string>(&m_bridge_addr),
                     "set core listen address for bridge entry connection")

                    ("bport",
                     boost::program_options::value<std::string>(&m_bridge_port),
                     "set core listen port for bridge entry connection")

                    // Bridge Entry Server
                    ("be_addr",
                     boost::program_options::value<std::string>(&m_be_server_addr),
                     "set address for bridge entry server")

                    ("be_port",
                     boost::program_options::value<std::string>(&m_be_server_port),
                     "set port for bridge entry server")
                    
                    // Hostkeeper
                    ("haddr",
                     boost::program_options::value<std::string>(&m_hostkeeper_addr),
                     "set hostkeeper server address")
                    
                    ("hport",
                     boost::program_options::value<std::string>(&m_hostkeeper_port),
                     "set hostkeeper server port")

                    ("hcache",
                     boost::program_options::value<bool>(&m_hostkeeper_cache),
                     "enable local cache on hostkeeper")

                    // Inspector
                    ("iaddr",
                     boost::program_options::value<std::string>(&m_inspector_addr),
                     "set inspector address")
                    
                    ("iport",
                     boost::program_options::value<std::string>(&m_inspector_port),
                     "set inspector port")


                    // Debug options
                    ("ms",
                     boost::program_options::value<int>(&m_is_measurement),
                     "set measurement level for actor message handle")

                    // Parallel Debug options
                    ("local_host_id",
                     boost::program_options::value<std::string>(&m_local_host_id),
                     "set local host id")

                    ("get_host_id",
                     boost::program_options::value<std::size_t>(&m_get_host_id),
                     "if local host id is not set, the way to get hostid : 0 to use local IP, 1 to request, 2 to read from db,if no then request")

                    ("legacy_hosts",
                     boost::program_options::value<std::string>(&m_legacy_hosts),
                     "claim legacy host ids (for db backward compatibility)")

                    ("parallel_msg_st",
                     boost::program_options::value<bool>(&m_parallel_msg_st),
                     "enbale statistics for distributed messages");

                boost::program_options::store(boost::program_options::parse_command_line(ac, av, options), vm);
                boost::program_options::notify(vm);

                if (vm.count("help"))
                {
                    std::cout << options << std::endl;
                    exit(0);
                }
                else if (vm.count("version"))
                {
                    std::cout <<"==============================================================="<<std::endl;
                    std::cout <<"                Nebutown Server Daemon"<<std::endl
                              <<"    Copyright © 2011 Nebutown Corp."<<std::endl
                              <<"    Version: "<< nb_version_string << std::endl
                              <<"    Build  Time: "<< nb_build_time_string << std::endl
                              <<"    Change Time: "<< nb_version_time_string << std::endl;
                    std::cout <<"==============================================================="<<std::endl;
                    exit(0);
                }
            }
            catch(std::exception& e)
            {
                std::cerr << "error: " << e.what() << ".\n";
                std::cout << options << "\n";
                exit(0);
            }
            catch(...)
            {
                std::cerr << "Exception of unknown type.\n";
                std::cout << options << "\n";
                exit(0);
            }

            //check the option is valid or not
            if(!check_option(vm))
            {
                //exit(0);
            }
        }
        
        process_option();
    }

    //get option
    std::string get_port_number()
    {
        return m_port;
    }

    std::string get_addr()
    {
        return m_addr;
    }

    std::size_t get_socket_thread_pool()
    {
        return m_socket_thread_pool;
    }

    std::size_t get_actor_thread_pool()
    {
        return m_actor_thread_pool;
    }

    bool get_is_statistics()
    {
        return m_is_statistics;
    }

    std::size_t get_statistics_interval()
    {
        return m_statistics_interval;
    }

    bool get_is_gc()
    {
        return m_is_gc;
    }

    std::size_t get_gc_size()
    {
        return m_gc_size;
    }

    std::size_t get_gc_rate()
    {
        return m_gc_rate;
    }

    int get_log_level() const
    {
        return m_log_level;
    }

    std::string get_local_host_id()
    {
        return m_local_host_id;
    }

    std::size_t get_db_chkpoint_interval()
    {
        return m_db_chkpoint_interval;
    }
    
    std::string get_bridge_addr()
    {
        return m_bridge_addr;
    }
    
    std::string get_bridge_port()
    {
        return m_bridge_port;
    }

    std::string get_be_server_addr()
    {
        return m_be_server_addr;
    }

    std::string get_be_server_port()
    {
        return m_be_server_port;
    }
    
    std::string get_hostkeeper_addr()
    {
        return m_hostkeeper_addr;
    }

    std::string get_hostkeeper_port()
    {
        return m_hostkeeper_port;
    }

    bool get_hostkeeper_cache_option() const
    {
        return m_hostkeeper_cache;
    }

    std::string get_local_host_id() const
    {
        return m_local_host_id;
    }

    std::size_t get_host_id_option()
    {
        return m_get_host_id;
    }
    
    std::string get_legacy_hosts() const
    {
        return m_legacy_hosts;
    }
    
    int&  get_is_measurement()
    {
        return m_is_measurement;
    }

    bool get_parallel_msg_statistics() const
    {
        return m_parallel_msg_st;
    }
    
    std::string get_inspector_addr() const
    {
        return m_inspector_addr;
    }

    std::string get_inspector_port() const
    {
        return m_inspector_port;
    }

    void dump_configuration()
    {
        std::cout<<"------------------dump configuration----------------"<<std::endl;
        std::cout<<"port = "<<m_port<<std::endl;
        std::cout<<"addr = "<<m_addr<<std::endl;
        std::cout<<"actor_thread_pool = "<<m_actor_thread_pool<<std::endl;
        std::cout<<"socket_thread_pool = "<<m_socket_thread_pool<<std::endl;
        std::cout<<"gabage collection option = "<<(m_is_gc  ? "true" : "false")<<std::endl;
        std::cout<<"gabage collection threshold = "<<m_gc_size<<" MB"<<std::endl;
        std::cout<<"gabage collection rate = "<<m_gc_rate<<"%"<<std::endl;
        std::cout<<"statistics_option = "<<(m_is_statistics ? "true" : "false")<<std::endl;
        std::cout<<"statistics_interval = "<<m_statistics_interval<<std::endl;
        std::cout<<"log level = "<<m_log_level<<std::endl;
        std::cout<<"local host id = "<<m_local_host_id<<std::endl;
        std::cout<<"db checkpoint interval = "<<m_db_chkpoint_interval<<std::endl;
        std::cout<<"bridge address = "<<m_bridge_addr<<std::endl;
        std::cout<<"bridge port = "<<m_bridge_port<<std::endl;
        std::cout<<"bridge server address = "<<m_be_server_addr<<std::endl;
        std::cout<<"bridge server port = "<<m_be_server_port<<std::endl;
        std::cout<<"hostkeeper server address = "<<m_hostkeeper_addr<<std::endl;
        std::cout<<"hostkeeper server port = "<<m_hostkeeper_port<<std::endl;
        std::cout<<"hostkeeper enable local cache = "<<(m_hostkeeper_cache ? "true" : "false")<<std::endl;
        std::cout<<"measurement option = "<<m_is_measurement<<std::endl;
        std::cout<<"get host id option = "<<m_get_host_id<<std::endl;
        std::cout<<"legacy hosts = "<<m_legacy_hosts<<std::endl;
        std::cout<<"parallel message statistics = "<<(m_parallel_msg_st ? "true" : "false")<<std::endl;
        std::cout<<"inspector address = "<<m_inspector_addr<<std::endl;
        std::cout<<"inspector port = "<<m_inspector_port<<std::endl;
        std::cout<<"---------------------------------------------------"<<std::endl;        
    }
    
private:
    void init_default_options();

    void load_config_file();

    bool check_option(const boost::program_options::variables_map& vm)
    {
        return true;
    }

    void process_option()
    {
        std::string local = nb_getifaddr_v4();
        //1. if the server missing the listen address
        if(m_addr.empty())
        {
            m_addr = local;
        }

        //2. if the bridge missing the listen address
        if(m_bridge_addr.empty())
        {
            m_bridge_addr = local;
        }

        //3. if the inspector missing the listen address
        if(m_inspector_addr.empty())
        {
            m_inspector_addr = local;
        }
    }        
    
private:
    std::string m_port;
    std::string m_addr;
    std::size_t m_socket_thread_pool;

    std::size_t m_actor_thread_pool;
    
    bool m_is_statistics;
    std::size_t m_statistics_interval;

    bool m_is_gc;
    std::size_t m_gc_size;
    std::size_t m_gc_rate;

    std::size_t m_log_level;
    
    std::size_t m_db_chkpoint_interval;
    
    std::string m_bridge_addr;
    std::string m_bridge_port;

    std::string m_be_server_addr;
    std::string m_be_server_port;
    
    std::string m_hostkeeper_addr;
    std::string m_hostkeeper_port;
    bool        m_hostkeeper_cache;

    int        m_is_measurement;

    std::size_t m_get_host_id;
    std::string m_legacy_hosts;
    std::string m_local_host_id;

    bool        m_parallel_msg_st;

    std::string m_inspector_addr;
    std::string m_inspector_port;
};

#endif /* _NB_CONFIGURATION_H_ */
