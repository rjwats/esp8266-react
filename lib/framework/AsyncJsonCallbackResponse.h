#ifndef _AsyncJsonCallbackResponse_H_
#define _AsyncJsonCallbackResponse_H_

#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>

/*
 * Listens for a response being destroyed and calls a callback during said distruction.
 * used so we can take action after the response has been rendered to the client.
 *
 * Avoids having to fork ESPAsyncWebServer with a callback feature, but not nice!
 */

typedef std::function<void()> AsyncJsonCallback;

class AsyncJsonCallbackResponse : public AsyncJsonResponse {
 private:
  AsyncJsonCallback _callback;

 public:
  AsyncJsonCallbackResponse(AsyncJsonCallback callback,
                            bool isArray = false,
                            size_t maxJsonBufferSize = DYNAMIC_JSON_DOCUMENT_SIZE) :
      AsyncJsonResponse(isArray, maxJsonBufferSize),
      _callback{callback} {
  }

  ~AsyncJsonCallbackResponse() {
    _callback();
  }
};

#endif  // end _AsyncJsonCallbackResponse_H_
