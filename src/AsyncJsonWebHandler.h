#ifndef Async_Json_Request_Web_Handler_H_
#define Async_Json_Request_Web_Handler_H_

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#define ASYNC_JSON_REQUEST_DEFAULT_MAX_SIZE 1024
#define ASYNC_JSON_REQUEST_MIMETYPE "application/json"

/*
* Handy little utility for dealing with small JSON request body payloads.
*
* Need to be careful using this as we are somewhat limited by RAM.
*
* Really only of use where there is a determinate payload size.
*/

typedef std::function<void(AsyncWebServerRequest *request, JsonDocument &jsonDocument)> JsonRequestCallback;

class AsyncJsonWebHandler: public AsyncWebHandler {

  private:
    WebRequestMethodComposite _method;
    JsonRequestCallback _onRequest;
    size_t _maxContentLength;

  protected:
    String _uri;

  public:

    AsyncJsonWebHandler() :      
      _method(HTTP_POST|HTTP_PUT|HTTP_PATCH),
      _onRequest(nullptr),
      _maxContentLength(ASYNC_JSON_REQUEST_DEFAULT_MAX_SIZE),
      _uri() {}

    ~AsyncJsonWebHandler() {}

    void setUri(const String& uri) { _uri = uri; }
    void setMethod(WebRequestMethodComposite method) { _method = method; }
    void setMaxContentLength(size_t maxContentLength) { _maxContentLength = maxContentLength; }
    void onRequest(JsonRequestCallback fn) { _onRequest = fn; }

    virtual bool canHandle(AsyncWebServerRequest *request) override final {
      if(!_onRequest)
        return false;

      if(!(_method & request->method()))
        return false;

      if(_uri.length() && (_uri != request->url() && !request->url().startsWith(_uri+"/")))
        return false;

      if (!request->contentType().equalsIgnoreCase(ASYNC_JSON_REQUEST_MIMETYPE))
        return false;

      request->addInterestingHeader("ANY");
      return true;
    }

    virtual void handleRequest(AsyncWebServerRequest *request) override final {
      // no request configured
      if(!_onRequest) {
        Serial.print("No request callback was configured for endpoint: ");
        Serial.println(_uri);           
        request->send(500);
        return;
      }

      // we have been handed too much data, return a 413 (payload too large)
      if (request->contentLength() > _maxContentLength) {
        request->send(413);
        return;
      }

      // parse JSON and if possible handle the request
      if (request->_tempObject) {
        DynamicJsonDocument jsonDocument(_maxContentLength);
        DeserializationError error = deserializeJson(jsonDocument, (uint8_t *) request->_tempObject);
        if (error == DeserializationError::Ok) {
          _onRequest(request, jsonDocument);
        }else{
          request->send(400);
        }
        return;
      }

      // fallthrough, we have a null pointer, return 500.
      // this can be due to running out of memory or never receiving body data.
      request->send(500);
    }

    virtual void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) override final {
      if (_onRequest) {
        // don't allocate if data is too large
        if (total > _maxContentLength){
          return;
        }

        // try to allocate memory on first call
        // NB: the memory allocated here is freed by ~AsyncWebServerRequest
        if(index == 0 && !request->_tempObject){
          request->_tempObject = malloc(total);
        }

        // copy the data into the buffer, if we have a buffer!
        if (request->_tempObject) {
            memcpy((uint8_t *) request->_tempObject+index, data, len);
        }
      }
    }

    virtual bool isRequestHandlerTrivial() override final {
      return _onRequest ? false : true;
    }

};

#endif // end Async_Json_Request_Web_Handler_H_
