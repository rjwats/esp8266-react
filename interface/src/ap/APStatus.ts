import { Theme } from "@material-ui/core";
import { APStatus } from "./types";

export const apStatusHighlight = ({ active }: APStatus, theme: Theme) => {
  return active ? theme.palette.success.main : theme.palette.info.main;
}

export const apStatus = ({ active }: APStatus) => {
  return active ? "Active" : "Inactive";
};
