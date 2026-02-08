
import { defineConfig } from 'eslint/config';

import js from '@eslint/js'
import tseslint from 'typescript-eslint';
import globals from 'globals'

import reactHooks from 'eslint-plugin-react-hooks'
import reactRefresh from 'eslint-plugin-react-refresh'
import stylistic from '@stylistic/eslint-plugin'

export default defineConfig([
  { ignores: ['dist'] },
  {
    extends: [js.configs.recommended, ...tseslint.configs.recommended],
    files: ['**/*.{ts,tsx}'],
    languageOptions: {
      ecmaVersion: 2020,
      globals: globals.browser,
    },
    plugins: {
      'react-hooks': reactHooks,
      'react-refresh': reactRefresh,
      '@stylistic': stylistic
    },
    rules: {
      ...reactHooks.configs.recommended.rules,
      'react-refresh/only-export-components': [
        'warn',
        { allowConstantExport: true },
      ],
      'eol-last': 1,
      '@stylistic/member-delimiter-style': 1,
      '@stylistic/jsx-closing-bracket-location': 1,
      '@stylistic/jsx-closing-tag-location': 1,
      '@stylistic/jsx-wrap-multilines': 1,
      '@stylistic/jsx-curly-newline': 1,
      '@stylistic/jsx-max-props-per-line': [1, { when: 'multiline' }],
      '@stylistic/jsx-first-prop-new-line': [1, 'multiline'],
      "@stylistic/no-mixed-operators": [
        "warn",
        {
          "groups": [
            ["&&", "||"]
          ],
          "allowSamePrecedence": true
        }
      ],

      'no-multiple-empty-lines': [1, { max: 1 }],
      'no-trailing-spaces': 1,
      'semi': 1,
      'no-extra-semi': 1,
      'no-shadow': 1,
      'max-len': [1, { code: 140 }],
      'arrow-parens': 1,
      'curly': [1, 'all'],

      // disable rules which have been enabled by typescript-eslint's recommended config
      "@typescript-eslint/no-explicit-any": 0,
      "@typescript-eslint/no-empty-object-type": 0,
      "@typescript-eslint/no-unused-vars": [
        "warn",
        {
          "argsIgnorePattern": "^_",
          "varsIgnorePattern": "^_",
          "caughtErrorsIgnorePattern": "^_",
          "ignoreRestSiblings": true
        }
      ],
      // FIXME: Re-enable and address react-hooks/set-state-in-effect linting rule
      // turning this off for now, we need to find better patterns in a few places
      // so this can be re-enabled
      "react-hooks/set-state-in-effect": "off"
    },
  },
]);
