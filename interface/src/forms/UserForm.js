import React from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';

import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';
import PasswordValidator from '../components/PasswordValidator';

import FormControlLabel from '@material-ui/core/FormControlLabel';
import Switch from '@material-ui/core/Switch';
import FormGroup from '@material-ui/core/FormGroup';

const styles = theme => ({
  textField: {
    width: "100%"
  },
  checkboxControl: {
    width: "100%"
  },
  chips: {
    display: 'flex',
    flexWrap: 'wrap',
  },
  chip: {
    marginRight: theme.spacing.unit,
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  }
});

class UserForm extends React.Component {

  componentWillMount() {
    ValidatorForm.addValidationRule('uniqueUsername', this.props.uniqueUsername);
  }

  render() {
    const { classes, user, creating, handleValueChange, handleCheckboxChange, onDoneEditing, onCancelEditing } = this.props;
    return (
      <ValidatorForm onSubmit={onDoneEditing}>
        <TextValidator
          validators={creating ? ['required', 'uniqueUsername', 'matchRegexp:^[a-zA-Z0-9_\\.]{1,24}$'] : []}
          errorMessages={creating ? ['Username is required', "That username already exists", "Must be 1-24 characters: alpha numberic, '_' or '.'"] : []}
          name="username"
          label="Username"
          className={classes.textField}
          value={user.username}
          disabled={!creating}
          onChange={handleValueChange('username')}
          margin="normal"
        />
        <PasswordValidator
          validators={['required', 'matchRegexp:^.{0,64}$']}
          errorMessages={['Password is required', 'Password must be 64 characters or less']}
          name="password"
          label="Password"
          className={classes.textField}
          value={user.password}
          onChange={handleCheckboxChange('password')}
          margin="normal"
        />
        <FormGroup>
          <FormControlLabel
            control={<Switch checked={user.admin} onChange={handleCheckboxChange('admin')} id="admin" />}
            label="Admin?"
          />
        </FormGroup>
        <Button variant="contained" color="primary" className={classes.button} type="submit">
          Save
        </Button>
        <Button variant="contained" color="secondary" className={classes.button} onClick={onCancelEditing}>
          Back
      	</Button>
      </ValidatorForm>
    );
  }
}

UserForm.propTypes = {
  classes: PropTypes.object.isRequired,
  user: PropTypes.object.isRequired,
  creating: PropTypes.bool.isRequired,
  onDoneEditing: PropTypes.func.isRequired,
  onCancelEditing: PropTypes.func.isRequired,
  uniqueUsername: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  handleCheckboxChange: PropTypes.func.isRequired
};

export default withStyles(styles)(UserForm);
