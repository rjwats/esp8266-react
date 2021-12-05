import React, { FC, useEffect, useState } from 'react';
import { ChromePicker, ColorResult } from 'react-color';

import { Button, Dialog, DialogTitle, DialogContent, DialogActions, Box, Slider } from '@mui/material';

import { ValidatedTextField } from '../components';
import { generateGradient, Palette } from './types';
import Schema, { ValidateFieldsError } from 'async-validator';
import { updateValue } from '../utils';
import { validate } from '../validators';

interface PaletteFormProps {
  creating: boolean;
  validator: Schema;

  palette?: Palette;
  setPalette: React.Dispatch<React.SetStateAction<Palette | undefined>>;

  onDoneEditing: () => void;
  onCancelEditing: () => void;
}

const PaletteForm: FC<PaletteFormProps> = ({ creating, validator, palette, setPalette, onDoneEditing, onCancelEditing }) => {

  const [color, setColor] = useState<number>(0);
  const [fieldErrors, setFieldErrors] = useState<ValidateFieldsError>();

  const updateFormValue = updateValue(setPalette);

  const selectColor = (event: Event, value: number | number[]) => setColor(value as number);

  const open = !!palette;

  useEffect(() => {
    if (open) {
      setFieldErrors(undefined);
    }
  }, [open]);

  const renderContent = () => {
    if (!palette) {
      return null;
    }

    const changeColor = (result: ColorResult) => {
      const colors = [...palette.colors];
      colors[color] = result.hex;
      setPalette({ ...palette, colors });
    };

    const validateAndDone = async () => {
      try {
        setFieldErrors(undefined);
        await validate(validator, palette);
        onDoneEditing();
      } catch (errors: any) {
        setFieldErrors(errors);
      }
    };

    return (
      <>
        <DialogTitle id="palette-form-dialog-title">{creating ? 'Add' : 'Modify'} Palette</DialogTitle>
        <DialogContent dividers>
          <ValidatedTextField
            fieldErrors={fieldErrors}
            name="name"
            label="Name"
            fullWidth
            variant="outlined"
            value={palette.name}
            disabled={!creating}
            onChange={updateFormValue}
            margin="normal"
          />
          <Box height={30} width="100%" style={{ background: generateGradient(palette) }} />
          <Slider
            min={0}
            max={15}
            step={1}
            marks
            value={color}
            onChange={selectColor}
          />
          <ChromePicker
            styles={{ default: { picker: { width: "100%" } } }}
            disableAlpha
            color={palette.colors[color]}
            onChange={changeColor}
          />
        </DialogContent>
        <DialogActions>
          <Button variant="contained" color="secondary" onClick={onCancelEditing}>
            Cancel
          </Button>
          <Button variant="contained" color="primary" type="submit" onClick={validateAndDone}>
            Done
          </Button>
        </DialogActions>
      </>
    );
  };

  return (
    <Dialog onClose={onCancelEditing} aria-labelledby="palette-form-dialog-title" open={open} maxWidth="sm" fullWidth>
      {renderContent()}
    </Dialog>
  );

};

export default PaletteForm;
