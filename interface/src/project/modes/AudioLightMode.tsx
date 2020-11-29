import React from "react";
import { ColorResult } from "react-color";
import { extractEventValue, WebSocketFormProps } from "../../components";
import { AudioLightMode, AudioLightModeSettings } from "../types";

export interface AudioLightModeProps<D extends AudioLightModeSettings> {
    handleValueChange: (name: keyof D, callback?: () => void) => (event: React.ChangeEvent<HTMLInputElement>) => void;
    handleChange: <T extends unknown>(name: keyof D) => (value: T) => void;
    handleSliderChange: (name: keyof D) => (event: React.ChangeEvent<{}>, value: number | number[]) => void;
    handleColorChange: (name: keyof D) => (value: ColorResult) => void;
    data: D;
}

export function audioLightMode<D extends AudioLightModeSettings>(AudioLightModeComponent: React.ComponentType<AudioLightModeProps<D>>) {
    return class extends React.Component<WebSocketFormProps<AudioLightMode>> {

        handleChange = <T extends unknown>(name: keyof D) => (value: T) => {
            const { data, setData, saveData } = this.props;
            setData({ ...data, settings: { ...data.settings, [name]: value } }, saveData);
        }

        handleValueChange = (name: keyof D) => (event: React.ChangeEvent<HTMLInputElement>) => {
            const { data, setData, saveData } = this.props;
            setData({ ...data, settings: { ...data.settings, [name]: extractEventValue(event) } }, saveData)
          }
    
        handleSliderChange = (name: keyof D) => (event: React.ChangeEvent<{}>, value: number | number[]) => {
            const { data, setData, saveData } = this.props;
            setData({ ...data, settings: { ...data.settings, [name]: value } }, saveData);
        }

        handleColorChange = (name: keyof D) => (value: ColorResult) => {
            const { data, setData, saveData } = this.props;
            setData({ ...data, settings: { ...data.settings, [name]: value.hex } }, saveData);
        }

        render() {
            return (
                <AudioLightModeComponent
                    handleChange={this.handleChange}
                    handleValueChange={this.handleValueChange}
                    handleSliderChange={this.handleSliderChange}
                    handleColorChange={this.handleColorChange}
                    data={this.props.data.settings as D} />
            );
        }

    }
}
