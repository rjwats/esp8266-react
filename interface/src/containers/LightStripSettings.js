import React, { Component } from 'react';

import { LIGHT_STRIP_ENDPOINT }  from  '../constants/Endpoints';
import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent';
import { CirclePicker } from 'react-color'

class LightStripSettings extends Component {

  componentDidMount() {
      this.props.loadData();
  }

  handleColorChange = ({ hex }) => console.log(hex);

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="Light Strip Settings">
        {data && JSON.stringify(data)}
        {fetched}
        {errorMessage}

        <CirclePicker
          color="#333"
          onChangeComplete={ this.handleColorChange }
        />

      </SectionContent>
    );
  }

}

export default restComponent(LIGHT_STRIP_ENDPOINT, LightStripSettings);
