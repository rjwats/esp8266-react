import React, { Component } from 'react';

import { restController, RestControllerProps, RestFormLoader, RestFormProps } from '../components';
import { PaletteSettingsContext } from './PaletteSettingsContext';
import { PaletteSettings, PALETTE_SETTINGS_ENDPOINT } from './types';

class PaletteSettingsLoader extends Component<RestControllerProps<PaletteSettings>> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <RestFormLoader
        {...this.props}
        render={formProps => <PaletteSettingsContextProvider {...formProps} />}
      />
    )
  }

}

class PaletteSettingsContextProvider extends Component<RestFormProps<PaletteSettings>> {

  render() {
    return (
      <PaletteSettingsContext.Provider value={{
        paletteSettings: this.props.data,
        paletteSettingsUpdated: this.props.setData
      }} >
        {this.props.children}
      </PaletteSettingsContext.Provider>
    )
  }

}

export default restController(PALETTE_SETTINGS_ENDPOINT, PaletteSettingsLoader);
