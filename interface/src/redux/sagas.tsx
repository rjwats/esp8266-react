import { fork,all, call, put, takeEvery, takeLatest ,take , select} from "redux-saga/effects";
import {  ActionType,WebSocketMessageType} from "./actions";
import { ActionModel,MessageModel } from "./types";
import { ENDPOINT_ROOT,fetchAPI } from '../api';
export const LIGHT_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "lightState";


function* setLED() {
  try {
      const led_state = yield select((state) => state.led);
      const data = yield call(fetchAPI,{url:LIGHT_SETTINGS_ENDPOINT,data:led_state});
      yield console.log("data",data);
      yield put({type:ActionType.LED_UPDATE_STATE, payload :data});
  } catch (e) {
    console.log(e);
  }
}
function* ws_message(action: ActionModel) {
    const message: MessageModel = action.payload as MessageModel;
    // Now we can act on incoming messages
    switch (message.type) {
      case WebSocketMessageType.ID:
        yield put({type:ActionType.WS_UPDATE_STATE, payload :message });
        break;
      case WebSocketMessageType.PAYLOAD:
        const ws_state = yield select((state) => state.ws);
        yield console.log("message id=",ws_state.id,message.origin_id);
        if (ws_state.id && (!ws_state.payload || ws_state.id !== message.origin_id)) {
          yield put({type:ActionType.WS_UPDATE_STATE, payload :message  });
          yield put({type:ActionType.LED_UPDATE_STATE, payload :message.payload});
        }
        break;
    }
}

function* watchAndLog() {
    const action = yield take('*');
    const state = yield select();
    yield console.log("action",action);
    yield console.log("state",state);
}
export default function* rootSaga() {
  
  yield all([
     call(watchAndLog),
     takeLatest(ActionType.LED_SET_STATE,setLED),
     takeLatest(ActionType.WS_MESSAGE,ws_message)
  ])
  
}
