import { ActionType } from "../actions";
import { ActionModel } from "../types";

const initialState = {
    type: null,
    id: null,
    origin_id: null,
    payload: null
  }

const reducer = (state = initialState, action:ActionModel) => {
  const payload = action.payload;
  console.log("action",action.type)
  console.log("payload",payload)
  switch (action.type) {
    
    case ActionType.WS_SEND_MESSAGE:
      return {
        ...state,
        ...payload
      };
    case ActionType.WS_MESSAGE:
      return {
        ...state,
        ...payload
      };
    case ActionType.WS_UPDATE_STATE:
      return {
        ...state,
        ...payload
      };
    default:
      return state;
  }
};

export default reducer;