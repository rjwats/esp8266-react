#ifndef FrequencyTransmitter_h
#define FrequencyTransmitter_h

#define FREQUENCY_TRANSMITTER_WS_PATH "/ws/frequencies"

#include <WebSocketTxRx.h>
#include <StatefulService.h>
#include <FrequencySampler.h>

class FrequencyTransmitter : public StatefulService<FrequencyData> {
 public:
  FrequencyTransmitter(AsyncWebServer* server, SecurityManager* securityManager, FrequencySampler* frequencySampler);

 private:
  FrequencySampler* _frequencySampler;
  WebSocketTx<FrequencyData> _webSocketTx;
  void handleSample();
};

#endif  // end FrequencyTransmitter_h
