import React from 'react';
import PropTypes from 'prop-types';

import { ValidatorForm } from 'react-material-ui-form-validator';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';

import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import IconButton from '@material-ui/core/IconButton';

import EditIcon from '@material-ui/icons/Edit';
import DeleteIcon from '@material-ui/icons/Delete';
import Chip from '@material-ui/core/Chip';

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
    '& td, & th': {padding: theme.spacing.unit}    
  }    
});

class ManageUsersForm extends React.Component {

  render() {
    const { classes, users, usersFetched, errorMessage, onSubmit, onReset } = this.props;
    return (
      <div>
        {
          !usersFetched ?

            <div className={classes.loadingSettings}>
              <LinearProgress className={classes.loadingSettingsDetails} />
              <Typography variant="h4" className={classes.loadingSettingsDetails}>
                Loading...
              </Typography>
            </div>

            : users ?

              <ValidatorForm onSubmit={onSubmit}>
                <Table className={classes.table}>
                  <TableHead>
                    <TableRow>
                      <TableCell>Username</TableCell>
                      <TableCell>Password</TableCell>
                      <TableCell align="center">Role(s)</TableCell>
                      <TableCell align="center">Action</TableCell>
                    </TableRow>
                  </TableHead>
                  <TableBody>
                    {users.users.map(user => (
                      <TableRow key={user.username}>
                        <TableCell component="th" scope="row">
                          {user.username}
                        </TableCell>
                        <TableCell>{user.password}</TableCell>
                        <TableCell align="center">
                          <Chip label={user.role} className={classes.chip} />
                        </TableCell>
                        <TableCell align="center">
                          <IconButton aria-label="Delete">
                            <DeleteIcon />
                          </IconButton>
                          <IconButton aria-label="Edit">
                            <EditIcon />
                          </IconButton>
                        </TableCell>
                      </TableRow>
                    ))}
                  </TableBody>
                </Table>

                <Button variant="contained" color="primary" className={classes.button} type="submit">
                  Save
                </Button>
                <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
                  Reset
      		      </Button>

              </ValidatorForm>

              :

              <div className={classes.loadingSettings}>
                <Typography variant="h4" className={classes.loadingSettingsDetails}>
                  {errorMessage}
                </Typography>
                <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
                  Reset
      		</Button>
              </div>
        }
      </div>
    );
  }
}

ManageUsersForm.propTypes = {
  classes: PropTypes.object.isRequired,
  users: PropTypes.object,
  usersFetched: PropTypes.bool.isRequired,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  handleCheckboxChange: PropTypes.func.isRequired,
};

export default withStyles(styles)(ManageUsersForm);
