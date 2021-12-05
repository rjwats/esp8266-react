import { FC, useState } from 'react';

import { Button, Container, IconButton, Table, TableBody, TableCell, TableFooter, TableHead, TableRow } from '@mui/material';
import EditIcon from '@mui/icons-material/Edit';
import DeleteIcon from '@mui/icons-material/Delete';
import SaveIcon from '@mui/icons-material/Save';
import PersonAddIcon from '@mui/icons-material/PersonAdd';

import { ButtonRow, FormLoader, SectionContent } from '../components';
import { useRest } from '../utils';

import * as ProjectApi from "./api";
import { DEFAULT_PALETTE, generateGradient, Palette, PaletteSettings } from './types';
import PaletteForm from './PaletteForm';
import { createPaletteValidator } from './validators';

const PaletteSettingsForm: FC = () => {
  const { loadData, data, setData, saveData, errorMessage } = useRest<PaletteSettings>(
    { read: ProjectApi.readPaletteSettings, update: ProjectApi.updatePaletteSettings }
  );

  const [creating, setCreating] = useState<boolean>(false);
  const [palette, setPalette] = useState<Palette>();

  if (!data) {
    return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
  }

  const removePalette = (toRemove: Palette) => {
    const palettes = data.palettes.filter((p) => p.name !== toRemove.name);
    setData({ ...data, palettes });
  };

  const startEditingPalette = (toEdit: Palette) => {
    setCreating(false);
    // TODO - copy here?
    setPalette(toEdit);
  };

  const cancelEditingPalette = () => {
    setPalette(undefined);
  };

  const doneEditingPalette = () => {
    if (palette) {
      const palettes = data.palettes.filter((p) => p.name !== palette.name);
      palettes.push(palette);
      setData({ ...data, palettes });
      setPalette(undefined);
    }
  };

  const createPalette = () => {
    setCreating(true);
    setPalette({
      name: "",
      colors: [...DEFAULT_PALETTE]
    });
  };

  const renderPalettes = () => {
    return data.palettes.sort((a, b) => a.name.localeCompare(b.name)).map((p) => (
      <TableRow key={p.name}>
        <TableCell component="th" scope="row">
          {p.name}
        </TableCell>
        <TableCell component="th" scope="row" style={{ background: generateGradient(p) }} />
        <TableCell align="center">
          <IconButton size="small" aria-label="Delete" onClick={() => removePalette(p)}>
            <DeleteIcon />
          </IconButton>
          <IconButton size="small" aria-label="Edit" onClick={() => startEditingPalette(p)}>
            <EditIcon />
          </IconButton>
        </TableCell>
      </TableRow>
    ));
  };

  return (
    <Container maxWidth="md" disableGutters>
      <SectionContent title="Palette Settings" titleGutter>
        <Table size="small">
          <TableHead>
            <TableRow>
              <TableCell>Name</TableCell>
              <TableCell>Palette</TableCell>
              <TableCell />
            </TableRow>
          </TableHead>
          <TableBody>
            {renderPalettes()}
          </TableBody>
          <TableFooter>
            <TableRow>
              <TableCell colSpan={2} />
              <TableCell align="center" padding="normal">
                <Button startIcon={<PersonAddIcon />} variant="contained" color="secondary" onClick={createPalette}>
                  Add
                </Button>
              </TableCell>
            </TableRow>
          </TableFooter>
        </Table>
        <ButtonRow mt={2}>
          <Button startIcon={<SaveIcon />} variant="contained" color="primary" type="submit" onClick={saveData}>
            Save
          </Button>
        </ButtonRow>
        {
          palette &&
          // TODO - render dialog properly
          <PaletteForm
            creating={creating}
            validator={createPaletteValidator(data.palettes, creating)}
            palette={palette}
            setPalette={setPalette}
            onDoneEditing={doneEditingPalette}
            onCancelEditing={cancelEditingPalette}
          />
        }
      </SectionContent>
    </Container>
  );
};

export default PaletteSettingsForm;
