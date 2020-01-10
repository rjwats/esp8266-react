import { APStatusData } from "../containers/APStatus";
import { Theme } from "@material-ui/core";

export const apStatusHighlight = (apStatus: APStatusData, theme: Theme) => {
  return apStatus.active ? theme.palette.success.main : theme.palette.info.main;
}

export const apStatus = (apStatus: APStatusData) => {
  return apStatus.active ? "Active" : "Inactive";
};
