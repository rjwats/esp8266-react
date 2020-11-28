import { Container } from '@material-ui/core';
import React, { Component } from 'react';

import { restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import LedSettingsForm from './LedSettingsForm';

import { LedSettings, LED_SETTINGS_ENDPOINT } from './types';

type LedSettingsControllerProps = RestControllerProps<LedSettings>;

class LedSettingsController extends Component<LedSettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <Container maxWidth="md" disableGutters>
        <SectionContent title="Led Settings" titleGutter>
          <RestFormLoader
            {...this.props}
            render={formProps => <LedSettingsForm {...formProps} />}
          />
        </SectionContent>
      </Container>
    )
  }

}

export default restController(LED_SETTINGS_ENDPOINT, LedSettingsController);
