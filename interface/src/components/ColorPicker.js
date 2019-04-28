import React from 'react';

import { withStyles } from '@material-ui/core/styles';
import { SimpleColors } from '../constants/Colors.js'
import { HuePicker, TwitterPicker } from 'react-color'

const styles = theme => ({
  picker: {
    "margin-top": theme.spacing.unit,
    "margin-bottom": theme.spacing.unit*2
  }
});

class ColorPicker extends React.Component {

  render() {

    const {
      classes,
      color,
      onChange
    } = this.props;
  
    return (
      <div  className={classes.picker}>
        <HuePicker
          width="100%"
          color={ color }
          onChange={onChange}
        />
        <TwitterPicker
          width="100%"
          triangle="hide"
          color={ color }
          onChange={ onChange }
          colors={ SimpleColors }
        />
      </div>
    );
  }
}

export default withStyles(styles)(ColorPicker);

