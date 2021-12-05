import React from "react";
import { ColorResult } from "react-color";
import { extractEventValue } from "../../utils";
import { AudioLightMode, AudioLightModeSettings } from "../types";

export interface AudioLightModeRendererProps {
  data: AudioLightMode;
  updateData: (newData: AudioLightMode, transmitData?: boolean, clearData?: boolean) => void;
}

export const useAudioLightMode = <D extends AudioLightModeSettings>(
  data: AudioLightMode,
  updateData: (newData: AudioLightMode, transmitData?: boolean, clearData?: boolean) => void
) => {

  const handleChange = <T extends unknown>(name: keyof D) => (value: T) => {
    updateData({ ...data, settings: { ...data.settings, [name]: value } });
  };

  const handleValueChange = (name: keyof D) => (event: React.ChangeEvent<HTMLInputElement>) => {
    updateData({ ...data, settings: { ...data.settings, [name]: extractEventValue(event) } });
  };

  const handleSliderChange = (name: keyof D) => (event: Event, value: number | number[]) => {
    updateData({ ...data, settings: { ...data.settings, [name]: value } });
  };

  const handleColorChange = (name: keyof D) => (value: ColorResult) => {
    updateData({ ...data, settings: { ...data.settings, [name]: value.hex } });
  };

  return { settings: data.settings as D, handleChange, handleValueChange, handleSliderChange, handleColorChange } as const;
};

