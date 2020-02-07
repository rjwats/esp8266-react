import React, { RefObject } from 'react';
import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';

import { Dialog, DialogTitle, DialogContent, DialogActions, Checkbox } from '@material-ui/core';

import { PasswordValidator, BlockFormControlLabel, FormButton } from '../components';

import { User } from './types';

interface UserFormProps {
  creating: boolean;
  user: User;
  uniqueUsername: (value: any) => boolean;
  handleValueChange: (name: keyof User) => (event: React.ChangeEvent<HTMLInputElement>) => void;
  handleCheckboxChange: (name: keyof User) => (event: React.ChangeEvent<HTMLInputElement>, checked: boolean) => void;
  onDoneEditing: () => void;
  onCancelEditing: () => void;
}

class UserForm extends React.Component<UserFormProps> {

  formRef: RefObject<any> = React.createRef();

  componentDidMount() {
    ValidatorForm.addValidationRule('uniqueUsername', this.props.uniqueUsername);
  }

  submit = () => {
    this.formRef.current.submit();
  }

  render() {
    const { user, creating, handleValueChange, handleCheckboxChange, onDoneEditing, onCancelEditing } = this.props;
    return (
      <ValidatorForm onSubmit={onDoneEditing} ref={this.formRef}>
        <Dialog onClose={onCancelEditing} aria-labelledby="user-form-dialog-title" open={true}>
          <DialogTitle id="user-form-dialog-title">{creating ? 'Add' : 'Modify'} User</DialogTitle>
          <DialogContent dividers={true}>
            <TextValidator
              validators={creating ? ['required', 'uniqueUsername', 'matchRegexp:^[a-zA-Z0-9_\\.]{1,24}$'] : []}
              errorMessages={creating ? ['Username is required', "Username already exists", "Must be 1-24 characters: alpha numeric, '_' or '.'"] : []}
              name="username"
              label="Username"
              fullWidth
              variant="outlined"
              value={user.username}
              disabled={!creating}
              onChange={handleValueChange('username')}
              margin="normal"
            />
            <PasswordValidator
              validators={['required', 'matchRegexp:^.{1,64}$']}
              errorMessages={['Password is required', 'Password must be 64 characters or less']}
              name="password"
              label="Password"
              fullWidth
              variant="outlined"
              value={user.password}
              onChange={handleValueChange('password')}
              margin="normal"
            />
            <BlockFormControlLabel
              control={
                <Checkbox
                  color="primary"
                  value="admin"
                  checked={user.admin}
                  onChange={handleCheckboxChange('admin')}
                />
              }
              label="Admin?"
            />
          </DialogContent>
          <DialogActions>
            <FormButton variant="contained" color="primary" type="submit" onClick={this.submit}>
              Done
            </FormButton>
            <FormButton variant="contained" color="secondary" onClick={onCancelEditing}>
              Cancel
            </FormButton>
          </DialogActions>
        </Dialog>
      </ValidatorForm>
    );
  }
}

export default UserForm;
