import React from 'react';
import PropTypes from 'prop-types';
import { Link } from 'react-router-dom';

import { withStyles } from '@material-ui/core/styles';
import Drawer from '@material-ui/core/Drawer';
import AppBar from '@material-ui/core/AppBar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import IconButton from '@material-ui/core/IconButton';
import Hidden from '@material-ui/core/Hidden';
import Divider from '@material-ui/core/Divider';
import Grow from '@material-ui/core/Grow';
import MenuItem from '@material-ui/core/MenuItem';
import MenuList from '@material-ui/core/MenuList';

import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import ListItemText from '@material-ui/core/ListItemText';

import Popper from '@material-ui/core/Popper';
import MenuIcon from '@material-ui/icons/Menu';
import WifiIcon from '@material-ui/icons/Wifi';
import SystemUpdateIcon from '@material-ui/icons/SystemUpdate';
import AccessTimeIcon from '@material-ui/icons/AccessTime';
import AccountCircleIcon from '@material-ui/icons/AccountCircle';
import SettingsInputAntennaIcon from '@material-ui/icons/SettingsInputAntenna';
import LockIcon from '@material-ui/icons/Lock';

import ClickAwayListener from '@material-ui/core/ClickAwayListener';

import Paper from '@material-ui/core/Paper';

import { APP_NAME } from '../constants/App';
import { withAuthenticationContext } from '../authentication/Context.js';

const drawerWidth = 290;

const styles = theme => ({
  root: {
    display: 'flex',
  },
  drawer: {
    [theme.breakpoints.up('md')]: {
      width: drawerWidth,
      flexShrink: 0,
    },
  },
  title: {
    flexGrow: 1
  },
  appBar: {
    marginLeft: drawerWidth,
    [theme.breakpoints.up('md')]: {
      width: `calc(100% - ${drawerWidth}px)`,
    },
  },
  menuButton: {
    marginRight: theme.spacing(2),
    [theme.breakpoints.up('md')]: {
      display: 'none',
    },
  },
  toolbar: theme.mixins.toolbar,
  drawerPaper: {
    width: drawerWidth,
  },
  content: {
    flexGrow: 1,
    padding: theme.spacing(),
  },
  popper: {
    zIndex: 3300
  }
});

class MenuAppBar extends React.Component {
  state = {
    mobileOpen: false,
    authMenuOpen: false
  };

  anchorRef = React.createRef();

  handleToggle = () => {
    this.setState({ authMenuOpen: !this.state.authMenuOpen });
  }

  handleClose = (event) => {
    if (this.anchorRef.current && this.anchorRef.current.contains(event.target)) {
      return;
    }

    this.setState({ authMenuOpen: false });
  }

  handleDrawerToggle = () => {
    this.setState({ mobileOpen: !this.state.mobileOpen });
  };

  render() {
    const { classes, theme, children, sectionTitle, authenticationContext } = this.props;
    const { mobileOpen, authMenuOpen } = this.state;

    const drawer = (
      <div>
        <Toolbar>
          <Typography variant="h6" color="primary">
            {APP_NAME}
          </Typography>
          <Divider absolute />
        </Toolbar>
        <Divider />
        <List>
          <ListItem button component={Link} to='/wifi/'>
            <ListItemIcon>
              <WifiIcon />
            </ListItemIcon>
            <ListItemText primary="WiFi Connection" />
          </ListItem>
          <ListItem button component={Link} to='/ap-configuration'>
            <ListItemIcon>
              <SettingsInputAntennaIcon />
            </ListItemIcon>
            <ListItemText primary="Access Point" />
          </ListItem>
          <ListItem button component={Link} to='/ntp-configuration'>
            <ListItemIcon>
              <AccessTimeIcon />
            </ListItemIcon>
            <ListItemText primary="Network Time" />
          </ListItem>
          <ListItem button component={Link} to='/ota-configuration'>
            <ListItemIcon>
              <SystemUpdateIcon />
            </ListItemIcon>
            <ListItemText primary="OTA Updates" />
          </ListItem>
          <ListItem button component={Link} to='/security/'>
            <ListItemIcon>
              <LockIcon />
            </ListItemIcon>
            <ListItemText primary="Security" />
          </ListItem>
        </List>
      </div>
    );

    return (
      <div className={classes.root}>
        <AppBar position="fixed" className={classes.appBar}>
          <Toolbar>
            <IconButton
              color="inherit"
              aria-label="Open drawer"
              edge="start"
              onClick={this.handleDrawerToggle}
              className={classes.menuButton}
            >
              <MenuIcon />
            </IconButton>
            <Typography variant="h6" color="inherit" noWrap className={classes.title}>
              {sectionTitle}
            </Typography>
            <div>
              <IconButton
                ref={this.anchorRef}
                aria-owns={authMenuOpen ? 'menu-list-grow' : undefined}
                aria-haspopup="true"
                onClick={this.handleToggle}
                color="inherit"
              >
                <AccountCircleIcon />
              </IconButton>
              <Popper open={authMenuOpen} anchorEl={this.anchorRef.current} transition disablePortal>
                {({ TransitionProps, placement }) => (
                  <Grow
                    {...TransitionProps}
                    style={{ transformOrigin: placement === 'bottom' ? 'center top' : 'center bottom' }}
                  >
                    <Paper id="menu-list-grow">
                      <ClickAwayListener onClickAway={this.handleClose}>
                        <MenuList>
                          <MenuItem button onClick={authenticationContext.signOut}>
                            <ListItemIcon>
                              <AccountCircleIcon />
                            </ListItemIcon>
                            <ListItemText primary="Sign Out" secondary={"Signed in as: " + authenticationContext.jwt.username} />
                          </MenuItem>
                        </MenuList>
                      </ClickAwayListener>
                    </Paper>
                  </Grow>
                )}
              </Popper>
            </div>
          </Toolbar>
        </AppBar>
        <nav className={classes.drawer}>
          <Hidden mdUp implementation="css">
            <Drawer
              variant="temporary"
              anchor={theme.direction === 'rtl' ? 'right' : 'left'}
              open={mobileOpen}
              onClose={this.handleDrawerToggle}
              classes={{
                paper: classes.drawerPaper,
              }}
              ModalProps={{
                keepMounted: true,
              }}
            >
              {drawer}
            </Drawer>
          </Hidden>
          <Hidden smDown implementation="css">
            <Drawer
              classes={{
                paper: classes.drawerPaper,
              }}
              variant="permanent"
              open
            >
              {drawer}
            </Drawer>
          </Hidden>
        </nav>
        <main className={classes.content}>
          <div className={classes.toolbar} />
          {children}
        </main>
      </div>
    );
  }
}

MenuAppBar.propTypes = {
  classes: PropTypes.object.isRequired,
  theme: PropTypes.object.isRequired,
  sectionTitle: PropTypes.string.isRequired,
};

export default withAuthenticationContext(withStyles(styles, { withTheme: true })(MenuAppBar));
