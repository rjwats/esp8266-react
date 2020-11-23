import { Container } from '@material-ui/core';
import React, { Component } from 'react';
import { WEB_SOCKET_ROOT } from '../api';
import { SectionContent, webSocketController, WebSocketControllerProps, WebSocketFormLoader, WebSocketFormProps } from '../components';
import { FrequencyData } from './types';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "frequencies";

function hslToRgb(h: number, s: number, l: number) {
  var r, g, b;

  if (s === 0) {
    r = g = b = l;
  } else {
    var hue2rgb = function hue2rgb(p: number, q: number, t: number) {
      if (t < 0) t += 1;
      if (t > 1) t -= 1;
      if (t < 1 / 6) return p + (q - p) * 6 * t;
      if (t < 1 / 2) return q;
      if (t < 2 / 3) return p + (q - p) * (2 / 3 - t) * 6;
      return p;
    }

    var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    var p = 2 * l - q;
    r = hue2rgb(p, q, h + 1 / 3);
    g = hue2rgb(p, q, h);
    b = hue2rgb(p, q, h - 1 / 3);
  }

  return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
}

const calculateColorForPercentage = (percentage: number) => {
  var hue = (60 - (percentage * 60)) / 360;
  var rgb = hslToRgb(hue, 1, 0.5);
  return 'rgb(' + rgb[0] + ',' + rgb[1] + ',' + rgb[2] + ')';
}

type SpectrumAnalyzerWebSocketControllerProps = WebSocketControllerProps<FrequencyData>;

class SpectrumAnalyzerWebSocketController extends Component<SpectrumAnalyzerWebSocketControllerProps> {

  render() {
    return (
      <Container maxWidth="md" disableGutters>
        <SectionContent title='Spectrum Analyzer' titleGutter>
          <WebSocketFormLoader
            {...this.props}
            render={props => (
              <SpectrumAnalyzer {...props} />
            )}
          />
        </SectionContent>
      </Container>
    )
  }

}

export default webSocketController(LIGHT_SETTINGS_WEBSOCKET_URL, 100, SpectrumAnalyzerWebSocketController);

type LightStateWebSocketControllerFormProps = WebSocketFormProps<FrequencyData>;


class SpectrumAnalyzer extends Component<LightStateWebSocketControllerFormProps> {

  private canvas = React.createRef<HTMLCanvasElement>();

  componentDidUpdate() {
    const currentCanvas = this.canvas.current;
    const canvasContext = currentCanvas?.getContext("2d");

    if (!currentCanvas || !canvasContext) {
      return;
    }
    const visualizerData = this.props.data.bands;

    // Make it visually fill the positioned parent
    currentCanvas.style.backgroundColor = "#C5CAE9";
    currentCanvas.style.width = '100%';
    currentCanvas.style.height = '300px';

    // ...then set the internal size to match
    currentCanvas.width = currentCanvas.offsetWidth;
    currentCanvas.height = currentCanvas.offsetHeight;

    if (!visualizerData) {
      return;
    }

    // clear the canvas
    canvasContext.clearRect(0, 0, currentCanvas.width, currentCanvas.height);

    // calculate bar width
    var barWidth = currentCanvas.width / visualizerData.length;

    // output the data
    for (var i = 0; i < visualizerData.length; i++) {
      var barOffset = i * barWidth;
      var barValue = visualizerData[i];
      var barPercentage = barValue > 0 ? barValue / 4096 : 0;
      var barHeight = -currentCanvas.height * barPercentage;
      canvasContext.fillStyle = calculateColorForPercentage(barPercentage);
      canvasContext.fillRect(barOffset, currentCanvas.height, barWidth, barHeight);
    }
  }

  render() {
    return (
      <canvas ref={this.canvas} />
    )
  }

}
