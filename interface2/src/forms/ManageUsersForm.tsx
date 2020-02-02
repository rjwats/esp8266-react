import React, { Fragment } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Table, TableBody, TableCell, TableHead, TableFooter, TableRow } from '@material-ui/core';
import { Box, Button, Typography, } from '@material-ui/core';

import EditIcon from '@material-ui/icons/Edit';
import DeleteIcon from '@material-ui/icons/Delete';
import CloseIcon from '@material-ui/icons/Close';
import CheckIcon from '@material-ui/icons/Check';
import IconButton from '@material-ui/core/IconButton';
import SaveIcon from '@material-ui/icons/Save';
import PersonAddIcon from '@material-ui/icons/PersonAdd';

import { withAuthenticatedContext, AuthenticatedContextProps } from '../authentication/AuthenticationContext';
import { RestFormProps } from '../components/RestFormLoader';
import { SecuritySettings, User } from '../containers/SecuritySettingsController';
import UserForm from './UserForm';
import FormActions from '../components/FormActions';
import FormButton from '../components/FormButton';

function compareUsers(a: User, b: User) {
  if (a.username < b.username) {
    return -1;
  }
  if (a.username > b.username) {
    return 1;
  }
  return 0;
}

type ManageUsersFormProps = RestFormProps<SecuritySettings> & AuthenticatedContextProps;

type ManageUsersFormState = {
  creating: boolean;
  user?: User;
}

class ManageUsersForm extends React.Component<ManageUsersFormProps, ManageUsersFormState> {

  state: ManageUsersFormState = {
    creating: false
  };

  createUser = () => {
    this.setState({
      creating: true,
      user: {
        username: "",
        password: "",
        admin: true
      }
    });
  };

  uniqueUsername = (username: string) => {
    return !this.props.data.users.find(u => u.username === username);
  }

  noAdminConfigured = () => {
    return !this.props.data.users.find(u => u.admin);
  }

  removeUser = (user: User) => {
    const { data } = this.props;
    const users = data.users.filter(u => u.username !== user.username);
    this.props.setData({ ...data, users });
  }

  startEditingUser = (user: User) => {
    this.setState({
      creating: false,
      user
    });
  };

  cancelEditingUser = () => {
    this.setState({
      user: undefined
    });
  }

  doneEditingUser = () => {
    const { user } = this.state;
    if (user) {
      const { data } = this.props;
      const users = data.users.filter(u => u.username !== user.username);
      users.push(user);
      this.props.setData({ ...data, users });
      this.setState({
        user: undefined
      });
    }
  };

  handleUserValueChange = (name: keyof User) => (event: React.ChangeEvent<HTMLInputElement>) => {
    this.setState({ user: { ...this.state.user!, [name]: event.target.value } });
  };

  handleUserCheckboxChange = (name: keyof User) => (event: React.ChangeEvent<HTMLInputElement>) => {
    this.setState({ user: { ...this.state.user!, [name]: event.target.checked } });
  }

  onSubmit = () => {
    this.props.saveData();
    this.props.authenticatedContext.refresh();
  }

  render() {
    const { data, loadData } = this.props;
    const { user, creating } = this.state;
    return (
      <Fragment>
        <ValidatorForm onSubmit={this.onSubmit}>
          <Table size="small">
            <TableHead>
              <TableRow>
                <TableCell>Username</TableCell>
                <TableCell align="center">Admin?</TableCell>
                <TableCell />
              </TableRow>
            </TableHead>
            <TableBody>
              {data.users.sort(compareUsers).map(user => (
                <TableRow key={user.username}>
                  <TableCell component="th" scope="row">
                    {user.username}
                  </TableCell>
                  <TableCell align="center">
                    {
                      user.admin ? <CheckIcon /> : <CloseIcon />
                    }
                  </TableCell>
                  <TableCell align="center">
                    <IconButton aria-label="Delete" onClick={() => this.removeUser(user)}>
                      <DeleteIcon />
                    </IconButton>
                    <IconButton aria-label="Edit" onClick={() => this.startEditingUser(user)}>
                      <EditIcon />
                    </IconButton>
                  </TableCell>
                </TableRow>
              ))}
            </TableBody>
            <TableFooter>
              <TableRow>
                <TableCell colSpan={2} />
                <TableCell align="center">
                  <Button startIcon={<PersonAddIcon />} variant="contained" color="secondary" onClick={this.createUser}>
                    Add User
                  </Button>
                </TableCell>
              </TableRow>
            </TableFooter>
          </Table>
          {
            this.noAdminConfigured() &&
            <Typography component="div" variant="body1">
              <Box bgcolor="error.main" color="error.contrastText" p={2} mt={2} mb={2}>
                You must have at least one admin user configured.
              </Box>
            </Typography>
          }
          <FormActions>
            <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit" disabled={this.noAdminConfigured()}>
              Save
            </FormButton>
            <FormButton variant="contained" color="secondary" onClick={loadData}>
              Reset
            </FormButton>
          </FormActions>
        </ValidatorForm>
        {
          user &&
          <UserForm
            user={user}
            creating={creating}
            onDoneEditing={this.doneEditingUser}
            onCancelEditing={this.cancelEditingUser}
            handleValueChange={this.handleUserValueChange}
            handleCheckboxChange={this.handleUserCheckboxChange}
            uniqueUsername={this.uniqueUsername}
          />
        }
      </Fragment>
    );
  }

}

export default withAuthenticatedContext(ManageUsersForm);
