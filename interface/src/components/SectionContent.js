import React from 'react';
import PropTypes from 'prop-types';

import Paper from '@material-ui/core/Paper';
import { withStyles } from '@material-ui/core/styles';
import Typography from '@material-ui/core/Typography';

const styles = theme => ({
  content: {
    padding: theme.spacing(2),
    margin: theme.spacing(3),
  }
});

function SectionContent(props) {
  const { children, classes, title, titleGutter } = props;
  return (
      <Paper className={classes.content}>
        <Typography variant="h6" gutterBottom={titleGutter}>
          {title}
        </Typography>
        {children}
      </Paper>
  );
}

SectionContent.propTypes = {
  classes: PropTypes.object.isRequired,
	children: PropTypes.oneOfType([
        PropTypes.arrayOf(PropTypes.node),
        PropTypes.node
    ]).isRequired,
  title: PropTypes.string.isRequired,
  titleGutter: PropTypes.bool
};

export default withStyles(styles)(SectionContent);
