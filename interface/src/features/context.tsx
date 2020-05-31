import React from 'react';
import { Features } from './types';

export interface FeaturesContext {
  features: Features;
}

const FeaturesContextDefaultValue = {} as FeaturesContext
export const FeaturesContext = React.createContext(
  FeaturesContextDefaultValue
);
