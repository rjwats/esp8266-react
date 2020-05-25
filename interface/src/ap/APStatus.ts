import { Theme } from "@material-ui/core";
import { APStatus } from "./types";

const ACTIVE = 1;
const INACTIVE = 2;
const WAITING_CLIENTS_DISCONNECT = 3;

export const apStatusHighlight = ({ status }: APStatus, theme: Theme) => {
  switch (status) {
    case ACTIVE:
      return theme.palette.success.main;
    case INACTIVE:
      return theme.palette.info.main;
    case WAITING_CLIENTS_DISCONNECT:
      return theme.palette.warning.main;
    default:
      return theme.palette.error.main;
  }
}

export const apStatus = ({ status }: APStatus) => {
  switch (status) {
    case ACTIVE:
      return "Active";
    case INACTIVE:
      return "Inactive";
    case WAITING_CLIENTS_DISCONNECT:
      return "Waiting clients to disconnect";
    default:
      return "Unknown";
  }
};
