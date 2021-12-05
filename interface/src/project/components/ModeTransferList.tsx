/*
import React, { FC, useEffect } from 'react';
import { Button, Checkbox, Grid, List, ListItem, ListItemText, Paper, styled } from '@mui/material';

import { ROTATE_AUDIO_LIGHT_MODES, AudioLightModeType, AUDIO_LIGHT_MODE_METADATA } from '../types';

const MiniButton = styled(Button)({
  my: .5,
  minWidth: 35
});

function not(a: AudioLightModeType[], b: AudioLightModeType[]) {
  return a.filter((value) => b.indexOf(value) === -1);
}

const intersection = (a: AudioLightModeType[], b: AudioLightModeType[]) => {
  return a.filter((type) => b.includes(type));
};

interface TranserListProps {
  selected: AudioLightModeType[];
  onSelectionChanged: (selected: AudioLightModeType[]) => void;
}

const ModeTransferList: FC<TranserListProps> = ({ selected, onSelectionChanged }) => {
  const [checked, setChecked] = React.useState<AudioLightModeType[]>([]);
  const [left, setLeft] = React.useState(not(selected, ROTATE_AUDIO_LIGHT_MODES));
  const [right, setRight] = React.useState(selected);

  const leftChecked = intersection(checked, left);
  const rightChecked = intersection(checked, right);

  useEffect(() => {
    setLeft(not(ROTATE_AUDIO_LIGHT_MODES, selected));
    setRight(selected);
  }, [selected]);

  const handleToggle = (value: AudioLightModeType) => () => {
    const currentIndex = checked.indexOf(value);
    const newChecked = [...checked];

    if (currentIndex === -1) {
      newChecked.push(value);
    } else {
      newChecked.splice(currentIndex, 1);
    }

    setChecked(newChecked);
  };

  const handleAllRight = () => {
    setChecked(not(checked, leftChecked));
    onSelectionChanged(ROTATE_AUDIO_LIGHT_MODES);
  };

  const handleCheckedRight = () => {
    onSelectionChanged(right.concat(leftChecked));
    setChecked(not(checked, leftChecked));
  };

  const handleCheckedLeft = () => {
    onSelectionChanged(not(right, rightChecked));
    setChecked(not(checked, rightChecked));
  };

  const handleAllLeft = () => {
    setChecked(not(checked, rightChecked));
    onSelectionChanged([]);
  };

  const customList = (items: AudioLightModeType[]) => (
    <Paper
      sx={{
        mt: 2,
        width: 170,
        height: 230,
        overflow: 'auto'
      }}
    >
      <List dense component="div" role="list">
        {items.map((value) => {
          return (
            <ListItem disableGutters key={value} role="listitem" button onClick={handleToggle(value)}>
              <Checkbox
                checked={checked.indexOf(value) !== -1}
                tabIndex={-1}
                disableRipple
              />
              <ListItemText primary={AUDIO_LIGHT_MODE_METADATA[value].label} />
            </ListItem>
          );
        })}
        <ListItem />
      </List>
    </Paper>
  );

  return (
    <Grid container spacing={2} alignItems="center">
      <Grid item>{customList(left)}</Grid>
      <Grid item>
        <Grid container direction="column" alignItems="center">
          <MiniButton
            variant="outlined"
            size="small"
            onClick={handleAllRight}
            disabled={left.length === 0}
            aria-label="move all right"
          >
            &gt;&gt;
          </MiniButton>
          <MiniButton
            variant="outlined"
            size="small"
            onClick={handleCheckedRight}
            disabled={leftChecked.length === 0}
            aria-label="move selected right"
          >
            &gt;
          </MiniButton>
          <MiniButton
            variant="outlined"
            size="small"
            onClick={handleCheckedLeft}
            disabled={rightChecked.length === 0}
            aria-label="move selected left"
          >
            &lt;
          </MiniButton>
          <MiniButton
            variant="outlined"
            size="small"
            onClick={handleAllLeft}
            disabled={right.length === 0}
            aria-label="move all left"
          >
            &lt;&lt;
          </MiniButton>
        </Grid>
      </Grid>
      <Grid item>{customList(right)}</Grid>
    </Grid>
  );
};

export default ModeTransferList;
*/

const TEMP = 1;
export default TEMP;
