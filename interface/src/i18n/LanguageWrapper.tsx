import React, { Component } from 'react';
import { I18nProvider } from '@lingui/react'

import { LanguageContext } from './LanguageContext';
import { i18n } from '.';

interface LanguageWrapperState {
  language: string;
};

class LanguageWrapper extends Component<{}, LanguageWrapperState> {

  state: LanguageWrapperState = { language: "en" };

  // could load locale from local storage here
  componentDidMount = () => {

  }

  selectLanguage = (language: string) => {
    this.setState({ language })
    i18n.activate(language);
  }

  render() {
    const { language } = this.state;
    return (
      <LanguageContext.Provider value={{
        language,
        selectLanguage: this.selectLanguage
      }}>
        <I18nProvider language="" i18n={i18n}>
          {this.props.children}
        </I18nProvider>
      </LanguageContext.Provider>
    );
  }

}

export default LanguageWrapper;
