import { ActionType } from "../actions";
import { ActionModel } from "../types";

const initialState = {
    led_on : false
  }

const reducer = (state = initialState, action:ActionModel) => {
  const payload = action.payload;
  console.log("action",action.type)
  console.log("payload",payload)
  switch (action.type) {
    
    case ActionType.LED_SET_STATE:
      return {
        ...state,
        led_on : payload.led_on
      };
    case ActionType.LED_UPDATE_STATE:
      return {
        ...state,
        led_on : payload.led_on
      };
    default:
      return state;
  }
};

export default reducer;