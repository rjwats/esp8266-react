import React from "react";
import { ColorResult } from "react-color";
import { WebSocketFormProps } from "../../components";
import { AudioLightMode } from "../types";

export interface AudioLightModeProps<D extends AudioLightMode> {
    handleValueChange: (name: keyof D, callback?: () => void) => (event: React.ChangeEvent<HTMLInputElement>) => void;
    handleChange: <T extends unknown>(name: keyof D) => (value: T) => void;
    handleSliderChange: (name: keyof D) => (event: React.ChangeEvent<{}>, value: number | number[]) => void;
    handleColorChange: (name: keyof D) => (value: ColorResult) => void;
    data: D;
}

export function audioLightMode<D extends AudioLightMode>(AudioLightMode: React.ComponentType<AudioLightModeProps<D>>) {
    return class extends React.Component<WebSocketFormProps<D>> {

        handleChange = <T extends unknown>(name: keyof D) => (value: T) => {
            const { data, setData, saveData } = this.props;
            setData({ ...data, [name]: value }, saveData);
        }

        handleValueChange = (name: keyof D) => {
            const { handleValueChange, saveData } = this.props;
            return handleValueChange(name, saveData)
        }

        handleSliderChange = (name: keyof D) => (event: React.ChangeEvent<{}>, value: number | number[]) => {
            const { setData, saveData } = this.props;
            setData({ ...this.props.data!, [name]: value }, saveData);
        }

        handleColorChange = (name: keyof D) => (value: ColorResult) => {
            const { setData, saveData } = this.props;
            setData({ ...this.props.data!, [name]: value.hex }, saveData);
        }

        render() {
            return (
                <AudioLightMode
                    handleChange={this.handleChange}
                    handleValueChange={this.handleValueChange}
                    handleSliderChange={this.handleSliderChange}
                    handleColorChange={this.handleColorChange}
                    data={this.props.data} />
            );
        }

    }
}
