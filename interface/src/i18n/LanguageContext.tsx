import React from 'react';

export interface LanguageContext {
  language: string;
  selectLanguage: (language: string) => void;
}

const LanguageContextDefaultValue = {} as LanguageContext
export const LanguageContext = React.createContext(
  LanguageContextDefaultValue
);

export type WithLanguageProps = LanguageContext;

export function withLanguage<T extends WithLanguageProps>(Component: React.ComponentType<T>) {
  return class extends React.Component<Omit<T, keyof WithLanguageProps>> {
    render() {
      return (
        <LanguageContext.Consumer>
          {language => <Component {...this.props as T}  {...language} />}
        </LanguageContext.Consumer>
      );
    }
  };
}

