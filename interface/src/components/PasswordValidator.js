import React from 'react';
import { TextValidator } from 'react-material-ui-form-validator';
import { InputAdornment } from '@material-ui/core';
import Visibility from '@material-ui/icons/Visibility';
import VisibilityOff from '@material-ui/icons/VisibilityOff';
import IconButton from '@material-ui/core/IconButton';

export default class PasswordValidator extends React.Component {

  state = {
    showPassword: false
  };

  toggleShowPassword = () => {
    this.setState({
      showPassword: !this.state.showPassword
    });
  }

  render() {
    return (
      <TextValidator
        {...this.props}
        type={this.state.showPassword ? 'text' : 'password'}
        InputProps={{
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
