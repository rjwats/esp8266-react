#ifndef MODE_H
#define MODE_H

#include <Arduino.h>

#include <lightstrip/IRCodes.h>
#include <lightstrip/Strip.h>
#include <ArduinoJson.h>

class Mode {
  protected:
    Strip *strip;
    void (*configChangeCallback)();

  public:

   Mode(Strip *strip){
     this->strip = strip;
     this->configChangeCallback = configChangeCallback;
   }

   /*
   * Recieve a ir command, repeated indicates the button is held.
   *
   * Repeat intervals will be 100ms approx.
   */
   virtual void handleIrCode(uint64_t irCode, boolean repeated) {
       // no-op implementation of irEvent, not all implementations require this.
   }

   /*
   * Get the code for the mode
   */
   virtual uint64_t getModeCode() = 0;

   /*
   * Called every time the main loop rotates, there is no guarantee of the
   * frequency this is called but it should maintain sub millisecond.
   *
   * The returned boolean indicates that the effect is requesting a re-render.
   */
   virtual void tick() = 0;

   /**
   * Called when the effect is enabled.
   */
   virtual void enable() = 0;

    // to update from the web server
   virtual void updateConfig(JsonObject& root) = 0;
   virtual void writeConfig(JsonObject& root) = 0;

};

#endif
