import React, { Component } from 'react';
import { createIntl, createIntlCache, RawIntlProvider } from 'react-intl';
import { messages } from './messages';

const defaultLocale = "es";

const cache = createIntlCache()

export const intl = createIntl({
  locale: defaultLocale,
  defaultLocale: defaultLocale,
  messages: messages[defaultLocale]
}, cache)

interface LanguageWrapperState {
  locale: string;
};

class I18n extends Component<{}, LanguageWrapperState> {

  state: LanguageWrapperState = { locale: defaultLocale };

  // load locale from local storage here
  componentDidMount = () => {

  }

  selectLanguage = (locale: string) => {
    intl.locale = locale;
    intl.messages = messages[locale];
    this.setState({ locale })
  }

  render() {
    const { locale } = this.state;
    return (
      <RawIntlProvider key={locale} value={intl}>
        {this.props.children}
      </RawIntlProvider>
    );
  }

}

export default I18n;
