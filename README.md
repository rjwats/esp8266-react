# ESP8266 React

A simple(ish) framework for getting up and running with the ESP8266 microchip and a react front end. Includes a GUI for configuring WiFi settings, a dynamic access point, NTP, and OTA updates.

Designed to work with the platformio IDE with limited setup.

## Why I made this project

I found I was repeating a lot of work when starting new projects with the ESP8266 chip. Most projects I've had demand a configuration interface for WiFi, the ability to synchronize with NTP, and OTA updates. I plan to use this as a basis for my upcoming personal projects and to extend and improve it as I go.

![Screenshots](/screenshots/screenshots.png?raw=true "Screenshots")

## Getting Started

### Prerequisites

You will need the following before you can get started.

* [PlatformIO](https://platformio.org/) - IDE for development
* [NPM](https://www.npmjs.com/) - For building the interface
* Bash shell, or Git Bash if you are under windows

### Installing in PlatformIO

Pull the project and add it to PlatformIO as a project folder (File > Add Project Folder).

PlatformIO should download the ESP8266 platform and the project library dependencies automatically.

Once the platform and libraries are downloaded the back end should be compiling.

### Building the interface

The interface has been configured with create-react-app and react-app-rewired so I can customize the build for the MCU. The large artefacts are gzipped and source maps and service worker are excluded.

The interface code lives in the interface directory, change to this directory with your bash shell (or Git Bash) and use the standard commands you would with any react app built with create-react-app:

Download and install the node modules:

npm install

Build the interface:

npm run build

NB: The build command will also delete the previously built interface (the ./data/www directory) and replace it with the freshly built one, ready for upload to the device.

## Configuration & Deployment

TODO...

## Design Overview

TODO...

## Libraries Used

* [React](https://reactjs.org/)
* [Material-UI](https://material-ui-next.com/)
* [Time](https://github.com/PaulStoffregen/Time)
* [NtpClient](https://github.com/gmag11/NtpClient)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)

Note that the project doesn't currently fix it's dependencies to a particular revision.

This may be particularly problematic for material-ui-next which is under active development and breaking changes are being made frequently.
