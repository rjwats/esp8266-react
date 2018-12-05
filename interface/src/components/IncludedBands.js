import React from 'react';

import { withStyles } from '@material-ui/core/styles';
import Switch from '@material-ui/core/Switch';

const styles = theme => ({
  "included_bands": {
    "margin-top": theme.spacing.unit,
    "margin-bottom": theme.spacing.unit*2
  }
});

class IncludedBands extends React.Component {

  handleChange = ordinal => event => {
    const {value, onChange} = this.props;
    const newValue = [...value];
    newValue[ordinal] = event.target.checked;
    onChange(newValue);
  };

  render() {

    const {
      value,
      classes
    } = this.props;

    var bandInputs = [];

    value.forEach((v,i) => {
      bandInputs.push(
        <Switch 
          key={"band" + i}
          checked={v}
          onChange={this.handleChange(i)}
          value="enabled"
          color="primary"
        />     
      );
    });
  
    return (
      <div className={classes.included_bands}>
        {bandInputs}
      </div>
    );
  }
}

export default withStyles(styles)(IncludedBands);

