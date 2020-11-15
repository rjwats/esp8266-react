#include <FrequencyTransmitter.h>

FrequencyTransmitter::FrequencyTransmitter(AsyncWebServer* server,
                                           SecurityManager* securityManager,
                                           FrequencySampler* frequencySampler) :
    _frequencySampler(frequencySampler),
    _webSocketTx(FrequencyData::read,
                 this,
                 server,
                 FREQUENCY_TRANSMITTER_WS_PATH,
                 securityManager,
                 AuthenticationPredicates::IS_AUTHENTICATED) {
  frequencySampler->addUpdateHandler([&](const String& originId) { handleSample(); }, false);
}

void FrequencyTransmitter::handleSample() {
  EVERY_N_MILLISECONDS(200) {
    update(
        [&](FrequencyData& state) {
          FrequencyData* frequencyData = _frequencySampler->getFrequencyData();
          std::copy(std::begin(frequencyData->bands), std::end(frequencyData->bands), std::begin(state.bands));
          return StateUpdateResult::CHANGED;
        },
        "system");
  }
}
