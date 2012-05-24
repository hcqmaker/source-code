/**************************************************************************
**
**  Copyright 2012 Nebutown Inc.
**
**************************************************************************/
#include "nb_configuration.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

// Set all the default configurations here
void nb_configuration::init_default_options()
{
    // default core listen port
    m_port = "10199";

    // default core listen addr
    m_addr = "";

    // default socket thread pool size
    m_socket_thread_pool = 2;
    
    // default actor thread pool size
    m_actor_thread_pool = 1;
    
    // statistics option
    m_is_statistics = false;
    m_statistics_interval = 10;
    m_is_measurement = 0;

    // log level
    m_log_level = 7;

    // database checkpint
    m_db_chkpoint_interval = 3 * 60;

    // garbage collection option
    m_is_gc = false;
    m_gc_size = 2000;            //Default_Gabage_Collection_Mem_Size : in MB, default 2G
    m_gc_rate = 1;               //Default_Gabage_Collection_Mem_Rate : in %, default 1%

    // for bridge server
    m_bridge_addr = "";
    m_bridge_port = "11098";                //Default_Bridge_Listen_Port : for bridge server connection
    m_be_server_addr = "192.168.1.18";      //Default Bridge Entry Server's address
    m_be_server_port = "1026";              //Default Bridge Entry Server's Listen Port

    // for hostkeeper
    m_hostkeeper_addr = "192.168.1.18";
    m_hostkeeper_port = "12580";
    m_hostkeeper_cache = true;

    // for inspector
    m_inspector_addr = "";
    m_inspector_port = "11111";

    // parallel debug options
    m_local_host_id = ""; 

    // The way to get self host_id:
    // 0 : use IP address and inform the hostkeeper
    // 1 : always request from hostkeeper
    // 2 : read from db first, if not then do requests
    m_get_host_id = 0;

    // if true, this nbd will also inform hostkeeper
    // "192.168.1.16", "192.168.1.21" as its host id
    m_legacy_hosts = "";

    // show distributed message statistics
    m_parallel_msg_st = false;
}

// A helper func to simplify retriving field and suppress exception
template<typename T>
bool get_field(boost::property_tree::ptree& pt, const std::string& field_name, T* dest)
{
    try
    {
        *dest = pt.get<T>(field_name);
    }
    catch (boost::property_tree::ptree_bad_path& e)
    {
        // the specified filed not exists, we just ignore
        return false;
    }
    return true;
}

void nb_configuration::load_config_file()
{
    if(!boost::filesystem::exists(Default_Config_Filename))
        return;

    // Create an empty property tree object
    using boost::property_tree::ptree;
    ptree pt;

    // Load the ini file into the property tree
    read_ini(Default_Config_Filename, pt);

    // Parse the options
    get_field(pt, "Log.level", &m_log_level);

    get_field(pt, "Network.socket_pool", &m_socket_thread_pool);
    get_field(pt, "Network.core_addr", &m_addr);
    get_field(pt, "Network.core_port", &m_port);
    get_field(pt, "Network.bridge_addr", &m_bridge_addr);
    get_field(pt, "Network.bridge_port", &m_bridge_port);
    
    get_field(pt, "BridgeEntry.be_server_addr", &m_be_server_addr);
    get_field(pt, "BridgeEntry.be_server_port", &m_be_server_port);

    get_field(pt, "Hostkeeper.addr",            &m_hostkeeper_addr);
    get_field(pt, "Hostkeeper.port",            &m_hostkeeper_port);
    get_field(pt, "Hostkeeper.cache",           &m_hostkeeper_cache);

    get_field(pt, "Framework.actor_poo", &m_actor_thread_pool);

    get_field(pt, "Statistics.enable", &m_is_statistics);
    get_field(pt, "Statistics.interval", &m_statistics_interval);

    get_field(pt, "Measurement.level", &m_is_measurement);

    get_field(pt, "GC.enable", &m_is_gc);
    get_field(pt, "GC.threshold", &m_gc_size);
    get_field(pt, "GC.rate", &m_gc_rate);

    get_field(pt, "Databse.checkpoint", &m_db_chkpoint_interval);

    get_field(pt, "Inspector.addr", &m_inspector_addr);
    get_field(pt, "Inspector.port", &m_inspector_port);

    get_field(pt, "Debug.local_host_id", &m_local_host_id);
    get_field(pt, "Debug.get_host_id", &m_get_host_id);
    get_field(pt, "Debug.legacy_hosts", &m_legacy_hosts);

    get_field(pt, "Parallel.message_statistics", &m_parallel_msg_st);
}

