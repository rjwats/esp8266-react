import React, { RefObject } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Dialog, DialogTitle, DialogContent, DialogActions, Box, Slider } from '@material-ui/core';

import { FormButton } from '../components';
import { generateGradient, Palette } from './types';
import { ChromePicker, ColorResult } from 'react-color';

interface PaletteFormProps {
  creating: boolean;
  palette: Palette;
  uniqueId: (value: any) => boolean;
  updatePalette: (palette: Palette) => void;
  onDoneEditing: () => void;
  onCancelEditing: () => void;
}

interface PaletteFormState {
  color: number;
}

class PaletteForm extends React.Component<PaletteFormProps, PaletteFormState> {

  state: PaletteFormState = {
    color: 0
  }

  formRef: RefObject<any> = React.createRef();

  componentDidMount() {
    ValidatorForm.addValidationRule('uniqueId', this.props.uniqueId);
  }

  submit = () => {
    this.formRef.current.submit();
  }

  selectColor = (event: React.ChangeEvent<{}>, value: number | number[]) => {
    this.setState({ color: value as number });
  }

  changeColor = (result: ColorResult) => {
    const { color } = this.state;
    const { updatePalette, palette } = this.props;
    const colors = [...palette.colors];
    colors[color] = result.hex;
    updatePalette({ ...palette, colors });
  }

  render() {
    const { palette, creating, onDoneEditing, onCancelEditing } = this.props;
    const { color } = this.state;

    return (
      <ValidatorForm onSubmit={onDoneEditing} ref={this.formRef}>
        <Dialog onClose={onCancelEditing} aria-labelledby="palette-form-dialog-title" open maxWidth="sm" fullWidth>
          <DialogTitle id="palette-form-dialog-title">{creating ? 'Add' : 'Modify'} Palette</DialogTitle>
          <DialogContent dividers>
            <Box height={30} width="100%" style={{ background: generateGradient(palette) }} />
            <Slider
              min={0}
              max={15}
              step={1}
              marks
              value={color}
              onChange={this.selectColor}
            />
            <ChromePicker styles={{ default: { picker: { width: "100%" } } }} disableAlpha color={palette.colors[color]} onChange={this.changeColor} />
          </DialogContent>
          <DialogActions>
            <FormButton variant="contained" color="secondary" onClick={onCancelEditing}>
              Cancel
            </FormButton>
            <FormButton variant="contained" color="primary" type="submit" onClick={this.submit}>
              Done
            </FormButton>
          </DialogActions>
        </Dialog>
      </ValidatorForm>
    );
  }
}

export default PaletteForm;
