import React, { Component, Fragment } from 'react';

import { withStyles } from 'material-ui/styles';
import Button from 'material-ui/Button';
import { LinearProgress } from 'material-ui/Progress';
import Typography from 'material-ui/Typography';
import List, { ListItem, ListItemText } from 'material-ui/List';
import Avatar from 'material-ui/Avatar';
import Divider from 'material-ui/Divider';
import SettingsInputAntennaIcon from 'material-ui-icons/SettingsInputAntenna';
import DeviceHubIcon from 'material-ui-icons/DeviceHub';
import ComputerIcon from 'material-ui-icons/Computer';

import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent'

import * as Highlight from '../constants/Highlight';
import { CHAMBER_STATUS_ENDPOINT }  from  '../constants/Endpoints';

const styles = theme => ({
  fetching: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  }
});

class ChamberStatus extends Component {

  apStatusHighlight(data){
    return data.active ? Highlight.SUCCESS : Highlight.IDLE;
  }

  apStatus(data){
    return data.active ? "Active" : "Inactive";
  }

  renderChamberStatus(data, classes){
    return  (
      <div>
        <List>
          <Fragment>
            The status stuff goes here.
          </Fragment>
        </List>
        <Button variant="raised" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
      </div>
    );
  }

  render() {
    const { data, fetched, errorMessage, classes }  = this.props;

    return (
      <SectionContent title="Chamber Status">
        {
         !fetched ?
         <div>
           <LinearProgress className={classes.fetching}/>
           <Typography variant="display1" className={classes.fetching}>
             Loading...
           </Typography>
         </div>
       :
        data ? this.renderChamberStatus(data, classes)
       :
        <div>
          <Typography variant="display1" className={classes.fetching}>
            {errorMessage}
          </Typography>
          <Button variant="raised" color="secondary" className={classes.button} onClick={this.props.loadData}>
            Refresh
          </Button>
        </div>
      }
      </SectionContent>
    )
  }
}

export default restComponent(CHAMBER_STATUS_ENDPOINT, withStyles(styles)(ChamberStatus));
