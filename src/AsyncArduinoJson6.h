
/**
*  A copy of AsyncJson.h from ESPAsyncWebServer, updated for ArduinoJson6.
*/

#ifndef ASYNC_ARDUINO_JSON_6_H
#define ASYNC_ARDUINO_JSON_6_H

#include <ArduinoJson.h>
#include <WebResponseImpl.h>
#include <ESPAsyncWebServer.h>

constexpr const char* JSON_MIMETYPE = "application/json";

class ChunkPrint : public Print {
  private:
    uint8_t* _destination;
    size_t _to_skip;
    size_t _to_write;
    size_t _pos;
  public:
    ChunkPrint(uint8_t* destination, size_t from, size_t len)
      : _destination(destination), _to_skip(from), _to_write(len), _pos{0} {}
    virtual ~ChunkPrint(){}
    size_t write(uint8_t c){
      if (_to_skip > 0) {
        _to_skip--;
        return 1;
      } else if (_to_write > 0) {
        _to_write--;
        _destination[_pos++] = c;
        return 1;
      }
      return 0;
    }
    size_t write(const uint8_t *buffer, size_t size) {
      size_t written = 0;
      while (written < size && write(buffer[written])) {
        written++;
      }
      return written;
    }  
};

class AsyncJsonResponse: public AsyncAbstractResponse {
  private:
    DynamicJsonDocument _jsonDocument;
    bool _isValid;
    JsonObject _root;    

  public:
    AsyncJsonResponse(int maxSize): _jsonDocument(maxSize), _isValid{false} {
      _code = 200;
      _contentType = JSON_MIMETYPE;
      _root = _jsonDocument.to<JsonObject>();
    }
    ~AsyncJsonResponse() {}    
    JsonObject getRoot() {
      return _root;
    }
    bool _sourceValid() const {
      return _isValid; 
    }
    size_t setLength() {
      _contentLength = measureJson(_jsonDocument);
      if (_contentLength) { _isValid = true; }
      return _contentLength;
    }
    size_t getSize() {
      return _jsonDocument.size();
    }
    size_t _fillBuffer(uint8_t *data, size_t len){
      ChunkPrint dest(data, _sentLength, len);
      serializeJson(_jsonDocument, dest);
      return len;
    }
};

typedef std::function<void(AsyncWebServerRequest *request, JsonVariant json)> ArJsonRequestHandlerFunction;

class AsyncCallbackJsonWebHandler: public AsyncWebHandler {
private:
protected:
  const String _uri;
  WebRequestMethodComposite _method;
  ArJsonRequestHandlerFunction _onRequest;
  size_t _contentLength;
  size_t _maxContentLength;
public:
  AsyncCallbackJsonWebHandler(const String& uri, ArJsonRequestHandlerFunction onRequest) : _uri(uri), _method(HTTP_POST|HTTP_PUT|HTTP_PATCH), _onRequest(onRequest), _maxContentLength(16384) {}
  void setMethod(WebRequestMethodComposite method){ _method = method; }
  void setMaxContentLength(int maxContentLength){ _maxContentLength = maxContentLength; }
  void onRequest(ArJsonRequestHandlerFunction fn){ _onRequest = fn; }
  virtual bool canHandle(AsyncWebServerRequest *request) override final{
    if(!_onRequest)
      return false;

    if(!(_method & request->method()))
      return false;

    if(_uri.length() && (_uri != request->url() && !request->url().startsWith(_uri+"/")))
      return false;

    if (!request->contentType().equalsIgnoreCase(JSON_MIMETYPE))
      return false;

    request->addInterestingHeader("ANY");
    return true;
  }
  virtual void handleRequest(AsyncWebServerRequest *request) override final {
    if(_onRequest) {
      if (request->_tempObject != nullptr) {
        DynamicJsonDocument _jsonDocument(_maxContentLength);
        DeserializationError err = deserializeJson(_jsonDocument, (uint8_t*)(request->_tempObject));
        if (err == DeserializationError::Ok) {
          _onRequest(request, _jsonDocument.as<JsonVariant>());
          return;
        }
      }
      request->send(_contentLength > _maxContentLength ? 413 : 400);
    } else {
      request->send(500);
    }
  }
  virtual void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) override final {
  }
  virtual void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) override final {
    if (_onRequest) {
      _contentLength = total;
      if (total > 0 && request->_tempObject == nullptr && total < _maxContentLength) {
        request->_tempObject = malloc(total);
      }
      if (request->_tempObject != nullptr) {
        memcpy((uint8_t*)(request->_tempObject) + index, data, len);
      }
    }
  }
  virtual bool isRequestHandlerTrivial() override final {return _onRequest ? false : true;}
};


/*
* Listens for a response being destroyed and calls a callback during said distruction.
*
* Used so we can take action after the response has been rendered to the client.
*
* Avoids having to fork ESPAsyncWebServer with a callback feature - still not a nice use of a destructor!
*/

typedef std::function<void()> AsyncJsonCallback;

class AsyncJsonCallbackResponse: public AsyncJsonResponse {

  private:

      AsyncJsonCallback _callback;

  public:

    AsyncJsonCallbackResponse(AsyncJsonCallback callback, int maxSize) : AsyncJsonResponse(maxSize), _callback{callback} {}
    ~AsyncJsonCallbackResponse() {
      _callback();
    }

};

#endif
