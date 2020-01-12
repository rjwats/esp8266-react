import { APStatusData } from "../containers/APStatus";
import { Theme } from "@material-ui/core";

export const apStatusHighlight = ({ active }: APStatusData, theme: Theme) => {
  return active ? theme.palette.success.main : theme.palette.info.main;
}

export const apStatus = ({ active }: APStatusData) => {
  return active ? "Active" : "Inactive";
};
