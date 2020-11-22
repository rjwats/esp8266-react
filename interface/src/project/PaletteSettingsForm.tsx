import { Button, IconButton, isWidthDown, Table, TableBody, TableCell, TableFooter, TableHead, TableRow, withWidth, WithWidthProps } from '@material-ui/core';
import React, { Fragment } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import EditIcon from '@material-ui/icons/Edit';
import DeleteIcon from '@material-ui/icons/Delete';
import SaveIcon from '@material-ui/icons/Save';
import PersonAddIcon from '@material-ui/icons/PersonAdd';

import { FormActions, FormButton, RestFormProps } from '../components';
import { DEFAULT_PALETTE, generateGradient, Palette, PaletteSettings } from './types';
import PaletteForm from './PaletteForm';

type PaletteSettingsFormProps = RestFormProps<PaletteSettings> & WithWidthProps;

interface PaletteSettingsFormState {
  creating: boolean;
  palette?: Palette;
}

class PaletteSettingsForm extends React.Component<PaletteSettingsFormProps, PaletteSettingsFormState> {

  state: PaletteSettingsFormState = {
    creating: false
  };

  uniqueId = (id: string) => {
    return !this.props.data.palettes.find(p => p.id = id);
  }

  removePalette = (palette: Palette) => {
    const { data } = this.props;
    const palettes = data.palettes.filter(p => p.id !== palette.id);
    this.props.setData({ ...data, palettes });
  }

  startEditingPalette = (palette: Palette) => {
    this.setState({
      creating: false,
      palette
    });
  };

  cancelEditingPalette = () => {
    this.setState({
      palette: undefined
    });
  }

  doneEditingPalette = () => {
    const { palette } = this.state;
    if (palette) {
      const { data } = this.props;
      const palettes = data.palettes.filter(p => p.id !== palette.id);
      palettes.push(palette);
      this.props.setData({ ...data, palettes });
      this.setState({
        palette: undefined
      });
    }
  };

  createPalette = () => {
    this.setState({
      creating: true,
      palette: {
        id: "",
        colors: [...DEFAULT_PALETTE]
      }
    });
  };

  updatePalette = (palette: Palette) => {
    this.setState({ palette });
  };

  renderPalettes = () => {
    const { data } = this.props;
    return data.palettes.sort((a, b) => a.id.localeCompare(b.id)).map(palette => (
      <TableRow key={palette.id}>
        <TableCell component="th" scope="row">
          {palette.id}
         </TableCell>
        <TableCell component="th" scope="row" style={{ background: generateGradient(palette) }} />
        <TableCell align="center">
          <IconButton size="small" aria-label="Delete" onClick={() => this.removePalette(palette)}>
            <DeleteIcon />
          </IconButton>
          <IconButton size="small" aria-label="Edit" onClick={() => this.startEditingPalette(palette)}>
            <EditIcon />
          </IconButton>
        </TableCell>
      </TableRow>
    ));
  }

  render() {
    const { saveData, width } = this.props;
    const { creating, palette } = this.state;

    return (
      <Fragment>
        <ValidatorForm onSubmit={saveData}>
          <Table size="small" padding={isWidthDown('xs', width!) ? "none" : "default"}>
            <TableHead>
              <TableRow>
                <TableCell>Palette</TableCell>
                <TableCell>Gradient</TableCell>
                <TableCell />
              </TableRow>
            </TableHead>
            <TableBody>
              {this.renderPalettes()}
            </TableBody>
            <TableFooter >
              <TableRow>
                <TableCell colSpan={2} />
                <TableCell align="center" padding="default">
                  <Button startIcon={<PersonAddIcon />} variant="contained" color="secondary" onClick={this.createPalette}>
                    Add
                  </Button>
                </TableCell>
              </TableRow>
            </TableFooter>
          </Table>
          <FormActions>
            <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
              Save
            </FormButton>
          </FormActions>
        </ValidatorForm>
        {
          palette &&
          <PaletteForm
            palette={palette}
            creating={creating}
            onDoneEditing={this.doneEditingPalette}
            onCancelEditing={this.cancelEditingPalette}
            updatePalette={this.updatePalette}
            uniqueId={this.uniqueId}
          />
        }
      </Fragment>
    );
  }

}

export default withWidth()(PaletteSettingsForm);
