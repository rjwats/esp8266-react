import React, { Component } from 'react';
import SectionContent from '../components/SectionContent'
import { FREQUENCY_STREAM_WEBSOCKET_URI } from '../constants/Endpoints';
import { wsReceiver } from '../components/WSReceiver';

function hslToRgb(h, s, l) {
  var r, g, b;

  if (s === 0) {
    r = g = b = l;
  } else {
    var hue2rgb = function hue2rgb(p, q, t) {
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

function calculateColorForPercentage(percentage) {
  var hue = (60 - (percentage * 60)) / 360;
  var rgb = hslToRgb(hue, 1, 0.5);
  return 'rgb(' + rgb[0] + ',' + rgb[1] + ',' + rgb[2] + ')';
}

class AudioLightFrequencyStatus extends Component {

  constructor(props) {
    super(props);

    this.canvas = React.createRef();
  }

  componentDidUpdate() {
    const canvas = this.canvas.current;
    const canvasContext = canvas.getContext("2d")
    const visualizerData = this.props.data;

    // Make it visually fill the positioned parent
    canvas.style["background-color"] = "#C5CAE9";
    canvas.style.width ='100%';
    canvas.style.height='300px';

    // ...then set the internal size to match
    canvas.width  = canvas.offsetWidth;
    canvas.height = canvas.offsetHeight;

    if (!visualizerData){
      return;
    }

    // clear the canvas
    canvasContext.clearRect(0, 0, canvas.width, canvas.height);

    // calculate bar width
    var barWidth = canvas.width / visualizerData.length;

    // output the data
    for (var i = 0; i < visualizerData.length; i++) {
      var barOffset = i * barWidth;
      var barValue = visualizerData[i];
      var barPercentage = barValue > 0 ? barValue / 4096 : 0;
      var barHeight = -canvas.height * barPercentage;
      canvasContext.fillStyle = calculateColorForPercentage(barPercentage);
      canvasContext.fillRect(barOffset, canvas.height, barWidth, barHeight);
    }
  }

  render() {
    return (
      <SectionContent title="Frequency Status">
        <canvas ref={this.canvas} />
      </SectionContent>
    )
  }

}

export default wsReceiver(FREQUENCY_STREAM_WEBSOCKET_URI, AudioLightFrequencyStatus);
