import React from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';

import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';
import PasswordValidator from '../components/PasswordValidator';

import Input from '@material-ui/core/Input';
import InputLabel from '@material-ui/core/InputLabel';
import MenuItem from '@material-ui/core/MenuItem';
import FormControl from '@material-ui/core/FormControl';
import Select from '@material-ui/core/Select';
import Chip from '@material-ui/core/Chip';

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
    const { classes, user, roles, creating, handleValueChange, onDoneEditing, onCancelEditing } = this.props;
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
          onChange={handleValueChange('password')}
          margin="normal"
        />
        <FormControl className={classes.textField}>
          <InputLabel htmlFor="roles">Roles</InputLabel>
          <Select
            multiple
            value={user.roles}
            onChange={handleValueChange('roles')}
            input={<Input id="roles" />}
            renderValue={selected => (
              <div className={classes.chips}>
                {selected.map(value => (
                  <Chip key={value} label={value} className={classes.chip} />
                ))}
              </div>
            )}
          >
            {roles.map(name => (
              <MenuItem key={name} value={name}>
                {name}
              </MenuItem>
            ))}
          </Select>
        </FormControl>
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
  roles: PropTypes.array.isRequired,
  onDoneEditing: PropTypes.func.isRequired,
  onCancelEditing: PropTypes.func.isRequired,
  uniqueUsername: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired
};

export default withStyles(styles)(UserForm);
