import { setupI18n } from "@lingui/core"

import en from '../locales/en/messages.js';
import es from '../locales/es/messages.js';

export const locales: { [locale: string]: string } = {
  en: "English",
  es: "Español"
}

export const i18n = setupI18n({
  catalogs: {
    en, es
  },
  language: navigator.language
});

export default i18n;
