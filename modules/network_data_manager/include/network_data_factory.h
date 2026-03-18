#ifndef NETWORK_DATA_FACTORY_H
#define NETWORK_DATA_FACTORY_H

#include "network_data_manager.h"
#ifdef BUILD_SERVER
    #include "network_data_manager_server.h" 
#else
    #include "network_data_manager_client.h" 
#endif
class NetworkDataManagerFactory
{
public:
    static NetworkDataManager* createNetworkDataManager(QObject *parent = nullptr){ 
        #ifdef BUILD_SERVER 
            return new NetworkDataManagerServer(parent);
        #else 
            return new NetworkDataManagerClient(parent);
        #endif
    };
};  
#endif // NETWORK_DATA_FACTORY_H