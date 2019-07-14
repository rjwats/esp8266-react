#ifndef DemoProject_h
#define DemoProject_h

#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>

class DemoProject {

  public:

    DemoProject(AsyncWebServer *server, SecurityManager* securityManager) : _server(server), _securityManager(securityManager) {}
    
    void begin();
    void loop();

  private:

    AsyncWebServer* _server;
    SecurityManager* _securityManager;

};

#endif
