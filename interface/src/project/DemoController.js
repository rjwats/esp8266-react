import React, { Component } from 'react';

import { withStyles } from '@material-ui/core/styles';
import SectionContent from '../components/SectionContent';

const styles = theme => ({

});

class DemoController extends Component {

  render() {    
    return (
      <SectionContent title="Controller" titleGutter>
        TODO - This will contain a form which controls the speed of the built in LED.
      </SectionContent>
    )
  }

}

export default withStyles(styles)(DemoController);
