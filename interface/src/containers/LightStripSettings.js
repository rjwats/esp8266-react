import React, { Component } from 'react';

import { LIGHT_STRIP_ENDPOINT }  from  '../constants/Endpoints';
import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent';

import LightStripSettingsForm from '../forms/LightStripSettingsForm';

class LightStripSettings extends Component {

  componentDidMount() {
      this.props.loadData();
  }

  handleColorChange = ({ hex }) => console.log(hex);

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="Light Strip Settings">
        <LightStripSettingsForm
          lightStripSettings={data}
          lightStripSettingsFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={this.props.saveData}
          onReset={this.props.loadData}
          handleValueChange={this.props.handleValueChange}
          handleColorChange={this.props.handleColorChange}
        />
      </SectionContent>
    );
  }

}

export default restComponent(LIGHT_STRIP_ENDPOINT, LightStripSettings);
