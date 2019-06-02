import React, { Fragment } from 'react';
import PropTypes from 'prop-types';

import { ValidatorForm } from 'react-material-ui-form-validator';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableFooter from '@material-ui/core/TableFooter';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import Box from '@material-ui/core/Box';

import EditIcon from '@material-ui/icons/Edit';
import DeleteIcon from '@material-ui/icons/Delete';
import CloseIcon from '@material-ui/icons/Close';
import CheckIcon from '@material-ui/icons/Check';
import IconButton from '@material-ui/core/IconButton';

import UserForm from './UserForm';
import { withAuthenticationContext } from '../authentication/Context';

const styles = theme => ({
  loadingSettings: {
    margin: theme.spacing(0.5),
  },
  loadingSettingsDetails: {
    margin: theme.spacing(4),
    textAlign: "center"
  },
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  },
  table: {
    '& td, & th': { padding: theme.spacing(0.5) }
  },
  actions: {
    whiteSpace: "nowrap"
  }
});

function compareUsers(a, b) {
  if (a.username < b.username) {
    return -1;
  }
  if (a.username > b.username) {
    return 1;
  }
  return 0;
}

class ManageUsersForm extends React.Component {

  constructor(props) {
    super(props);
    this.state = {};
  }

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

  uniqueUsername = username => {
    return !this.props.userData.users.find(u => u.username === username);
  }

  noAdminConfigured = () => {
    return !this.props.userData.users.find(u => u.admin);
  }

  removeUser = user => {
    const { userData } = this.props;
    const users = userData.users.filter(u => u.username !== user.username);
    this.props.setData({ ...userData, users });
  }

  startEditingUser = user => {
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
    const { userData } = this.props;
    const users = userData.users.filter(u => u.username !== user.username);
    users.push(user);
    this.props.setData({ ...userData, users });
    this.setState({
      user: undefined
    });
  };

  handleUserValueChange = name => event => {
    const { user } = this.state;
    this.setState({
      user: {
        ...user, [name]: event.target.value
      }
    });
  };

  handleUserCheckboxChange = name => event => {
    const { user } = this.state;
    this.setState({
      user: {
        ...user, [name]: event.target.checked
      }
    });
  }

  onSubmit = () => {
    this.props.onSubmit();
    this.props.authenticationContext.refresh();
  }

  render() {
    const { classes, userData, userDataFetched, errorMessage, onReset } = this.props;
    const { user, creating } = this.state;
    return (
      !userDataFetched ?
        <div className={classes.loadingSettings}>
          <LinearProgress className={classes.loadingSettingsDetails} />
          <Typography variant="h4" className={classes.loadingSettingsDetails}>
            Loading...
          </Typography>
        </div>
        :
        userData ?
          <Fragment>
            <ValidatorForm onSubmit={this.onSubmit}>
              <Table className={classes.table}>
                <TableHead>
                  <TableRow>
                    <TableCell>Username</TableCell>
                    <TableCell align="center">Admin?</TableCell>
                    <TableCell />
                  </TableRow>
                </TableHead>
                <TableBody>
                  {userData.users.sort(compareUsers).map(user => (
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
                      <Button variant="contained" color="secondary" onClick={this.createUser}>
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
              <Button variant="contained" color="primary" className={classes.button} type="submit" disabled={this.noAdminConfigured()}>
                Save
              </Button>
              <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
                Reset
      		    </Button>
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
          :
          <div className={classes.loadingSettings}>
            <Typography variant="h4" className={classes.loadingSettingsDetails}>
              {errorMessage}
            </Typography>
            <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
              Reset
      		  </Button>
          </div>
    );
  }

}

ManageUsersForm.propTypes = {
  classes: PropTypes.object.isRequired,
  userData: PropTypes.object,
  userDataFetched: PropTypes.bool.isRequired,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  setData: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  authenticationContext: PropTypes.object.isRequired,
};

export default withAuthenticationContext(withStyles(styles)(ManageUsersForm));
