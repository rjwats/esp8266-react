import React from 'react';
import PropTypes from 'prop-types';

import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';

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
import Chip from '@material-ui/core/Chip';

import EditIcon from '@material-ui/icons/Edit';
import DeleteIcon from '@material-ui/icons/Delete';
import IconButton from '@material-ui/core/IconButton';

import SectionContent from '../components/SectionContent';
import UserForm from './UserForm';

const styles = theme => ({
  loadingSettings: {
    margin: theme.spacing.unit,
  },
  loadingSettingsDetails: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  switchControl: {
    width: "100%",
    marginTop: theme.spacing.unit * 2,
    marginBottom: theme.spacing.unit
  },
  textField: {
    width: "100%"
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  },
  chip: {
    margin: theme.spacing.unit,
  },
  table: {
    '& td, & th': { padding: theme.spacing.unit }
  },
  actions: {
    color: theme.palette.text.secondary,
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
        roles: []
      }
    });
  };

  uniqueUsername = username => {
    return !this.props.userData.users.find(u => u.username === username);
  }

  usersValid = username => {
    return !!this.props.userData.users.find(u => u.roles.includes("admin"));
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

  sortedUsers(users) {
    return users.sort(compareUsers);
  }

  doneEditingUser = () => {
    const { user } = this.state;
    const { userData } = this.props;
    let { users } = userData;
    users = users.filter(u => u.username !== user.username);
    users.push(user);
    this.props.setData({ ...userData, users });
    this.setState({
      user: undefined
    });
  };

  handleUserValueChange = name => event => {
    const { user } = this.state;
    if (user) {
      this.setState({
        user: {
          ...user, [name]: event.target.value
        }
      });
    }
  };

  render() {
    const { classes, userData, userDataFetched, errorMessage, onSubmit, onReset, handleValueChange } = this.props;
    const { user, creating } = this.state;
    return (
      <SectionContent title="Manage Users">
        {
          !userDataFetched ?
            <div className={classes.loadingSettings}>
              <LinearProgress className={classes.loadingSettingsDetails} />
              <Typography variant="h4" className={classes.loadingSettingsDetails}>
                Loading...
              </Typography>
            </div>
            :
            userData ?
              user ?
                <UserForm
                  user={user}
                  creating={creating}
                  roles={userData.roles}
                  onDoneEditing={this.doneEditingUser}
                  onCancelEditing={this.cancelEditingUser}
                  handleValueChange={this.handleUserValueChange}
                  uniqueUsername={this.uniqueUsername}
                />
                :
                <ValidatorForm onSubmit={onSubmit}>  
                  <Table className={classes.table}>
                    <TableHead>
                      <TableRow>
                        <TableCell>Username</TableCell>
                        <TableCell align="center">Role(s)</TableCell>
                        <TableCell align="center">Action</TableCell>
                      </TableRow>
                    </TableHead>
                    <TableBody>
                      {this.sortedUsers(userData.users).map(user => (
                        <TableRow key={user.username}>
                          <TableCell component="th" scope="row">
                            {user.username}
                          </TableCell>
                          <TableCell align="center">
                            {
                              user.roles.map(role => (
                                <Chip label={role} className={classes.chip} />
                              ))
                            }
                          </TableCell>
                          <TableCell align="center">
                            <IconButton aria-label="Delete">
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
                        <TableCell colSpan={2}>
                          {
                            !this.usersValid() &&
                            <Typography variant="body1" color="error">
                              You must have at least one admin user configured.
                            </Typography>
                          }
                        </TableCell>
                        <TableCell align="center">
                          <Button variant="contained" color="secondary" className={classes.button} onClick={this.createUser}>
                            Add User
                        </Button>
                        </TableCell>
                      </TableRow>
                    </TableFooter>
                  </Table>
                  <Button variant="contained" color="primary" className={classes.button} type="submit">
                    Save
                  </Button>
                  <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
                    Reset
      		        </Button>
                </ValidatorForm>
              :
              <SectionContent title="Manage Users">
                <Typography variant="h4" className={classes.loadingSettingsDetails}>
                  {errorMessage}
                </Typography>
                <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
                  Reset
      		      </Button>
              </SectionContent>
        }
      </SectionContent>
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
  handleValueChange: PropTypes.func.isRequired
};

export default withStyles(styles)(ManageUsersForm);
