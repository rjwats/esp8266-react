import React, { Component } from 'react';
import { Typography, TableRow, TableBody, TableCell, TableHead, Table, Box } from '@material-ui/core';
import { SectionContent } from '../components';

class DemoInformation extends Component {

  render() {
    return (
      <SectionContent title='Demo Information' titleGutter>
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
        <Table>
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
        <Box mt={2}>
          <Typography variant="body1">
            See the project <a href="https://github.com/rjwats/esp8266-react/">README</a> for a full description of the demo project.
          </Typography>
        </Box>
      </SectionContent>
    )
  }

}

export default DemoInformation;
