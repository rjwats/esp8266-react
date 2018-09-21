import React, {Fragment} from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Checkbox from '@material-ui/core/Checkbox';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Typography from '@material-ui/core/Typography';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import ListItemSecondaryAction from '@material-ui/core/ListItemSecondaryAction';

import Avatar from '@material-ui/core/Avatar';
import IconButton from '@material-ui/core/IconButton';
import LockIcon from '@material-ui/icons/Lock';
import LockOpenIcon from '@material-ui/icons/LockOpen';
import DeleteIcon from '@material-ui/icons/Delete';

import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';
import { isNetworkOpen, networkSecurityMode } from '../constants/WiFiSecurityModes';

import isIP from '../validators/isIP';
import isHostname from '../validators/isHostname';
import optional from '../validators/optional';

const styles = theme => ({
  loadingSettings: {
    margin: theme.spacing.unit,
  },
  loadingSettingsDetails: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  textField: {
    width: "100%"
  },
  checkboxControl: {
    width: "100%"
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  }
});

class WiFiSettingsForm extends React.Component {

  componentWillMount() {
    ValidatorForm.addValidationRule('isIP', isIP);
    ValidatorForm.addValidationRule('isHostname', isHostname);
    ValidatorForm.addValidationRule('isOptionalIP', optional(isIP));
  }

  renderSelectedNetwork() {
    const { selectedNetwork, deselectNetwork } = this.props;
    return (
      <List>
        <ListItem>
          <ListItemAvatar>
            <Avatar>
              {isNetworkOpen(selectedNetwork) ? <LockOpenIcon /> : <LockIcon />}
            </Avatar>
          </ListItemAvatar>
          <ListItemText
            primary={selectedNetwork.ssid}
            secondary={"Security: "+ networkSecurityMode(selectedNetwork) + ", Ch: " + selectedNetwork.channel}
          />
          <ListItemSecondaryAction>
            <IconButton aria-label="Manual Config" onClick={deselectNetwork}>
              <DeleteIcon />
            </IconButton>
          </ListItemSecondaryAction>
        </ListItem>
      </List>
    );
  }

  render() {
    const { classes, formRef, wifiSettingsFetched, wifiSettings, errorMessage, selectedNetwork, handleValueChange, handleCheckboxChange, onSubmit, onReset } = this.props;
    return (
      <div ref={formRef}>
        {
         !wifiSettingsFetched ?

         <div className={classes.loadingSettings}>
           <LinearProgress className={classes.loadingSettingsDetails}/>
           <Typography variant="display1" className={classes.loadingSettingsDetails}>
             Loading...
           </Typography>
         </div>

         : wifiSettings ?

      	 <ValidatorForm onSubmit={onSubmit} ref="WiFiSettingsForm">
           {
             selectedNetwork ? this.renderSelectedNetwork() :
              <TextValidator
                validators={['required', 'matchRegexp:^.{0,32}$']}
                errorMessages={['SSID is required', 'SSID must be 32 characeters or less']}
                name="ssid"
                label="SSID"
                className={classes.textField}
                value={wifiSettings.ssid}
                onChange={handleValueChange('ssid')}
                margin="normal"
              />
            }
              {
                !isNetworkOpen(selectedNetwork) &&
         		<TextValidator
                  validators={['matchRegexp:^.{0,64}$']}
                  errorMessages={['Password must be 64 characters or less']}
                  name="password"
                  label="Password"
                  className={classes.textField}
                  value={wifiSettings.password}
                  onChange={handleValueChange('password')}
                  margin="normal"
                />
              }

              <TextValidator
                    validators={['required', 'isHostname']}
                    errorMessages={['Hostname is required', "Not a valid hostname"]}
                    name="hostname"
                    label="Hostname"
                    className={classes.textField}
                    value={wifiSettings.hostname}
                    onChange={handleValueChange('hostname')}
                    margin="normal"
                  />

        		<FormControlLabel className={classes.checkboxControl}
                  control={
                    <Checkbox
                      value="static_ip_config"
                      checked={wifiSettings.static_ip_config}
                      onChange={handleCheckboxChange("static_ip_config")}
                    />
                  }
                  label="Static IP Config?"
                />

        		{
          		wifiSettings.static_ip_config &&
          		<Fragment>
          			<TextValidator
          			  validators={['required', 'isIP']}
          			  errorMessages={['Local IP is required', 'Must be an IP address']}
          			  name="local_ip"
          			  label="Local IP"
          			  className={classes.textField}
          			  value={wifiSettings.local_ip}
          			  onChange={handleValueChange('local_ip')}
          			  margin="normal"
          			/>
                <TextValidator
                  validators={['required', 'isIP']}
                  errorMessages={['Gateway IP is required', 'Must be an IP address']}
                  name="gateway_ip"
                  label="Gateway"
                  className={classes.textField}
                  value={wifiSettings.gateway_ip}
                  onChange={handleValueChange('gateway_ip')}
                  margin="normal"
                />
                <TextValidator
          			  validators={['required', 'isIP']}
          			  errorMessages={['Subnet mask is required', 'Must be an IP address']}
          			  name="subnet_mask"
          			  label="Subnet"
          			  className={classes.textField}
          			  value={wifiSettings.subnet_mask}
          			  onChange={handleValueChange('subnet_mask')}
                  margin="normal"
          			/>
                <TextValidator
          			  validators={['isOptionalIP']}
          			  errorMessages={['Must be an IP address']}
          			  name="dns_ip_1"
          			  label="DNS IP #1"
          			  className={classes.textField}
          			  value={wifiSettings.dns_ip_1}
          			  onChange={handleValueChange('dns_ip_1')}
                  margin="normal"
          			/>
                <TextValidator
          			  validators={['isOptionalIP']}
          			  errorMessages={['Must be an IP address']}
          			  name="dns_ip_2"
          			  label="DNS IP #2"
          			  className={classes.textField}
          			  value={wifiSettings.dns_ip_2}
          			  onChange={handleValueChange('dns_ip_2')}
                  margin="normal"
          			/>
          		</Fragment>
      		  }

          <Button variant="raised" color="primary" className={classes.button} type="submit">
            Save
          </Button>
          <Button variant="raised" color="secondary" className={classes.button} onClick={onReset}>
      		  Reset
      		</Button>

        </ValidatorForm>

        :

        <div className={classes.loadingSettings}>
          <Typography variant="display1" className={classes.loadingSettingsDetails}>
            {errorMessage}
          </Typography>
          <Button variant="raised" color="secondary" className={classes.button} onClick={onReset}>
      		  Reset
      		</Button>
        </div>
      }
      </div>
    );
  }
}

WiFiSettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  wifiSettingsFetched: PropTypes.bool.isRequired,
  wifiSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  deselectNetwork: PropTypes.func,
  selectedNetwork: PropTypes.object,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  handleCheckboxChange: PropTypes.func.isRequired
};

export default withStyles(styles)(WiFiSettingsForm);
