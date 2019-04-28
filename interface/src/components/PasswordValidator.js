import React from 'react';
import { TextValidator } from 'react-material-ui-form-validator';
import { withStyles } from '@material-ui/core/styles';
import { InputAdornment } from '@material-ui/core';
import Visibility from '@material-ui/icons/Visibility';
import VisibilityOff from '@material-ui/icons/VisibilityOff';
import IconButton from '@material-ui/core/IconButton';

const styles = theme => (
  {
    input: {
      "&::-ms-reveal": {
        display: "none"
      }
    }
  });

class PasswordValidator extends React.Component {

  state = {
    showPassword: false
  };

  toggleShowPassword = () => {
    this.setState({
      showPassword: !this.state.showPassword
    });
  }

  render() {
    const { classes, ...rest } = this.props;
    return (
      <TextValidator
        {...rest}
        type={this.state.showPassword ? 'text' : 'password'}
        InputProps={{
          classes,
          endAdornment:
            <InputAdornment position="end">
              <IconButton
                aria-label="Toggle password visibility"
                onClick={this.toggleShowPassword}
              >
                {this.state.showPassword ? <Visibility /> : <VisibilityOff />}
              </IconButton>
            </InputAdornment>
        }}
      />
    );
  }

}

export default withStyles(styles)(PasswordValidator);
