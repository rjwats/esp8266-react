import { useDispatch, useSelector } from 'react-redux';
// import {AppDispatch,RootState} from './store';
// export type ActionModel = { 
//     type: string,
//     payload: any 
// }
// export type MessageModel = {
//     type: string,
//     id: string,
//     origin_id: string,
//     payload: any
// };
export enum WebSocketMessageType {
  ID = "id",
  PAYLOAD = "payload"
}
export enum ActionType{
    LED_SET_STATE = "LED_SET_STATE",
    LED_UPDATE_STATE = "LED_UPDATE_STATE",
    WS_CONNECTED = 'WS_CONNECTED',
    WS_DISCONNECTED = 'WS_DISCONNECTED',
    WS_MESSAGE = 'WS_MESSAGE',
    WS_SEND_MESSAGE = 'WS_SEND_MESSAGE',
    WS_UPDATE_STATE = 'WS_UPDATE_STATE'
}


// export  enum ActionType {
//     WS_CONNECTED = 'WS_CONNECTED',
//     WS_DISCONNECTED = 'WS_DISCONNECTED',
//     WS_MESSAGE = 'WS_MESSAGE',
//     WS_SEND_MESSAGE = 'WS_SEND_MESSAGE',
// };

// Use throughout your app instead of `useDispatch` and `useSelector`
export const useAppDispatch = () => useDispatch();
export const useAppSelector = (state:any) => {
    const data = useSelector(state);
    console.log("useSelector=",data);
    return data;
}

// export const REQUEST_HELLO_WORLD = "REQUEST_HELLO_WORLD";
// export const RECEIVE_HELLO_WORLD = "RECEIVE_HELLO_WORLD";



// export const requestHelloWorld = () => ({type: REQUEST_HELLO_WORLD });
// export const receiveHelloWorld = (text:String) => ({type : RECEIVE_HELLO_WORLD, text : text});
// export const led_set_state = (led_state) => ({ type: LED_SET_STATE });