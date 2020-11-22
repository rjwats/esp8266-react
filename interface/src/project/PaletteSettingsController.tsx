import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import PaletteSettingsForm from './PaletteSettingsForm';

import { PaletteSettings, PALETTE_SETTINGS_ENDPOINT  } from './types';

type PaletteSettingsControllerProps = RestControllerProps<PaletteSettings>;

class PaletteSettingsController extends Component<PaletteSettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Palette Settings" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <PaletteSettingsForm {...formProps} />}
        />
      </SectionContent>
    )
  }

}

export default restController(PALETTE_SETTINGS_ENDPOINT, PaletteSettingsController);
