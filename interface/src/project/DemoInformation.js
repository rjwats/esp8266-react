import React, { Component } from 'react';

import { withStyles } from '@material-ui/core/styles';
import Table from '@material-ui/core/Table';
import TableHead from '@material-ui/core/TableHead';
import TableCell from '@material-ui/core/TableCell';
import TableBody from '@material-ui/core/TableBody';
import TableRow from '@material-ui/core/TableRow';
import Typography from '@material-ui/core/Typography';

import SectionContent from '../components/SectionContent';

const styles = theme => ({
  fileTable: {
    marginBottom: theme.spacing(2)
  }
});

class DemoInformation extends Component {

  render() {
    const { classes } = this.props;
    return (
      <SectionContent title="Demo Project - Blink Speed Controller" titleGutter>
        <Typography variant="body1" paragraph>
          This simple demo project allows you to control the blink speed of the built-in LED. 
          It demonstrates how the esp8266-react framework may be extended for your own IoT project.
        </Typography>
        <Typography variant="body1" paragraph>
          It is recommended that you keep your project interface code under the 'project' directory.
          This serves to isolate your project code from the from the rest of the user interface which should
          simplify merges should you wish to update your project with future framework changes.
        </Typography>
        <Typography variant="body1" paragraph>
          The demo project interface code stored in the interface/project directory:
        </Typography>
        <Table className={classes.fileTable}>
          <TableHead>
            <TableRow>
              <TableCell>
                File
              </TableCell>
              <TableCell>
                Description
              </TableCell>
            </TableRow>
          </TableHead>
          <TableBody>
            <TableRow>
              <TableCell>
                ProjectMenu.js
              </TableCell>
              <TableCell>
                You can add your project's screens to the side bar here.
              </TableCell>
            </TableRow>
            <TableRow>
              <TableCell>
                ProjectRouting.js
              </TableCell>
              <TableCell>
                The routing which controls the screens of your project.
              </TableCell>
            </TableRow>
            <TableRow>
              <TableCell>
                DemoProject.js
              </TableCell>
              <TableCell>
                This screen, with tabs and tab routing.
              </TableCell>
            </TableRow>
            <TableRow>
              <TableCell>
                DemoInformation.js
              </TableCell>
              <TableCell>
                The demo information tab.
              </TableCell>
            </TableRow>
            <TableRow>
              <TableCell>
                DemoController.js
              </TableCell>
              <TableCell>
                The demo controller tab, to control the built-in LED.
              </TableCell>
            </TableRow>                    
          </TableBody>
        </Table>
        <Typography variant="body1" paragraph>
          See the project <a href="https://github.com/rjwats/esp8266-react/">README</a> for a full description of the demo project.
        </Typography>
      </SectionContent>
    )
  }

}

export default withStyles(styles)(DemoInformation);
