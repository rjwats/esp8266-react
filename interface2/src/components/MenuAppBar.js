import React from 'react';
import PropTypes from 'prop-types';
import { Link, withRouter } from 'react-router-dom';

import { withStyles } from '@material-ui/core/styles';
import Drawer from '@material-ui/core/Drawer';
import AppBar from '@material-ui/core/AppBar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import IconButton from '@material-ui/core/IconButton';
import Hidden from '@material-ui/core/Hidden';
import Divider from '@material-ui/core/Divider';
import Button from '@material-ui/core/Button';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import ListItemText from '@material-ui/core/ListItemText';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import Popper from '@material-ui/core/Popper';
import MenuIcon from '@material-ui/icons/Menu';
import WifiIcon from '@material-ui/icons/Wifi';
import SettingsIcon from '@material-ui/icons/Settings';
import AccessTimeIcon from '@material-ui/icons/AccessTime';
import AccountCircleIcon from '@material-ui/icons/AccountCircle';
import SettingsInputAntennaIcon from '@material-ui/icons/SettingsInputAntenna';
import LockIcon from '@material-ui/icons/Lock';
import ClickAwayListener from '@material-ui/core/ClickAwayListener';
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import CardActions from '@material-ui/core/CardActions';
import Avatar from '@material-ui/core/Avatar';

import ProjectMenu from '../project/ProjectMenu';
import { PROJECT_NAME } from '../constants/Env';
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
    flexGrow: 1
  },
  authMenu: {
    zIndex: theme.zIndex.tooltip,
    maxWidth: 400,
  },
  authMenuActions: {
    padding: theme.spacing(2),
    "& > * + *": {
      marginLeft: theme.spacing(2),
    }
  },
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
    const path = this.props.match.url;
    const drawer = (
      <div>
        <Toolbar>
          <Typography variant="h6" color="primary">
            {PROJECT_NAME}
          </Typography>
          <Divider absolute />
        </Toolbar>
        <Divider />
        <ProjectMenu />
        <Divider />
        <List>
          <ListItem to='/wifi/' selected={path.startsWith('/wifi/')} button component={Link}>
            <ListItemIcon>
              <WifiIcon />
            </ListItemIcon>
            <ListItemText primary="WiFi Connection" />
          </ListItem>
          <ListItem to='/ap/' selected={path.startsWith('/ap/')} button component={Link}>
            <ListItemIcon>
              <SettingsInputAntennaIcon />
            </ListItemIcon>
            <ListItemText primary="Access Point" />
          </ListItem>
          <ListItem to='/ntp/' selected={path.startsWith('/ntp/')} button component={Link}>
            <ListItemIcon>
              <AccessTimeIcon />
            </ListItemIcon>
            <ListItemText primary="Network Time" />
          </ListItem>
          <ListItem to='/security/' selected={path.startsWith('/security/')} button component={Link} disabled={!authenticationContext.isAdmin()}>
            <ListItemIcon>
              <LockIcon />
            </ListItemIcon>
            <ListItemText primary="Security" />
          </ListItem>
          <ListItem to='/system/' selected={path.startsWith('/system/')} button component={Link} >
            <ListItemIcon>
              <SettingsIcon />
            </ListItemIcon>
            <ListItemText primary="System" />
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
              <Popper open={authMenuOpen} anchorEl={this.anchorRef.current} transition className={classes.authMenu}>
                <ClickAwayListener onClickAway={this.handleClose}>
                  <Card id="menu-list-grow">
                    <CardContent>
                      <List disablePadding>
                        <ListItem disableGutters>
                          <ListItemAvatar>
                            <Avatar>
                              <AccountCircleIcon />
                            </Avatar>
                          </ListItemAvatar>
                          <ListItemText primary={"Signed in as: " + authenticationContext.user.username} secondary={authenticationContext.isAdmin() ? "Admin User" : undefined} />
                        </ListItem>
                      </List>
                    </CardContent>
                    <Divider />
                    <CardActions className={classes.authMenuActions}>
                      <Button variant="contained" color="primary" onClick={authenticationContext.signOut}>Sign Out</Button>
                    </CardActions>
                  </Card>
                </ClickAwayListener>
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

export default withAuthenticationContext(
  withRouter(
    withStyles(styles, { withTheme: true })(MenuAppBar)
  )
);
