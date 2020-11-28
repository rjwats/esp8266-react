import React, { FC, useEffect } from 'react';

import { makeStyles } from '@material-ui/core/styles';
import Grid from '@material-ui/core/Grid';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import ListItemText from '@material-ui/core/ListItemText';
import Checkbox from '@material-ui/core/Checkbox';
import Button from '@material-ui/core/Button';
import Paper from '@material-ui/core/Paper';

import { ROTATE_AUDIO_LIGHT_MODES, AudioLightModeType, AUDIO_LIGHT_MODE_METADATA } from '../types';

const useStyles = makeStyles((theme) => ({
  root: {
    margin: 'auto',
  },
  paper: {
    width: 200,
    height: 230,
    overflow: 'auto',
  },
  button: {
    margin: theme.spacing(0.5, 0),
  },
}));

function not(a: AudioLightModeType[], b: AudioLightModeType[]) {
  return a.filter((value) => b.indexOf(value) === -1);
}

const intersection = (a: AudioLightModeType[], b: AudioLightModeType[]) => {
  return a.filter((type) => b.includes(type));
}

interface TranserListProps {
  selected: AudioLightModeType[];
  onSelectionChanged: (selected: AudioLightModeType[]) => void;
}

const ModeTransferList: FC<TranserListProps> = ({ selected, onSelectionChanged }) => {
  const classes = useStyles();
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
    <Paper className={classes.paper}>
      <List dense component="div" role="list">
        {items.map((value) => {
          return (
            <ListItem key={value} role="listitem" button onClick={handleToggle(value)}>
              <ListItemIcon>
                <Checkbox
                  checked={checked.indexOf(value) !== -1}
                  tabIndex={-1}
                  disableRipple
                />
              </ListItemIcon>
              <ListItemText primary={AUDIO_LIGHT_MODE_METADATA[value].label} />
            </ListItem>
          );
        })}
        <ListItem />
      </List>
    </Paper>
  );

  return (
    <Grid container spacing={2} justify="center" alignItems="center" className={classes.root}>
      <Grid item>{customList(left)}</Grid>
      <Grid item>
        <Grid container direction="column" alignItems="center">
          <Button
            variant="outlined"
            size="small"
            className={classes.button}
            onClick={handleAllRight}
            disabled={left.length === 0}
            aria-label="move all right"
          >
            &gt;&gt;
          </Button>
          <Button
            variant="outlined"
            size="small"
            className={classes.button}
            onClick={handleCheckedRight}
            disabled={leftChecked.length === 0}
            aria-label="move selected right"
          >
            &gt;
          </Button>
          <Button
            variant="outlined"
            size="small"
            className={classes.button}
            onClick={handleCheckedLeft}
            disabled={rightChecked.length === 0}
            aria-label="move selected left"
          >
            &lt;
          </Button>
          <Button
            variant="outlined"
            size="small"
            className={classes.button}
            onClick={handleAllLeft}
            disabled={right.length === 0}
            aria-label="move all left"
          >
            &lt;&lt;
          </Button>
        </Grid>
      </Grid>
      <Grid item>{customList(right)}</Grid>
    </Grid>
  );
}

export default ModeTransferList;
