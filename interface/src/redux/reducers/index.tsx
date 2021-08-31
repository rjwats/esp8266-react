import { combineReducers } from "redux";
import led from "./led";
import ws from "./ws";
export default combineReducers({
  led,
  ws
});