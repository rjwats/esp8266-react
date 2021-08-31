import { createStore, applyMiddleware } from "redux";
import createSagaMiddleware from "redux-saga";

import reducer from "./reducers";
import rootSaga from "./sagas";
import { socketMiddleware } from './socketMiddleware';
// create the saga middleware
const sagaMiddleware = createSagaMiddleware();

const bindMiddleware = (middleware:any) => {
    //debugger;
    if (process.env.NODE_ENV !== 'production') {
        console.log("redux-devtools-extension");
        const { composeWithDevTools } = require('redux-devtools-extension')
        return composeWithDevTools(applyMiddleware(...middleware))
    }
    console.log("none redux-devtools-extension");
    return applyMiddleware(...middleware)
}
// mount it on the Store
const store = createStore(reducer, bindMiddleware([socketMiddleware,sagaMiddleware]));
//export default store = createStore(reducer, applyMiddleware(sagaMiddleware));

// then run the saga
sagaMiddleware.run(rootSaga);

export default store;
// // Infer the `RootState` and `AppDispatch` types from the store itself
// export type RootState = ReturnType<typeof store.getState>
// // Inferred type: {posts: PostsState, comments: CommentsState, users: UsersState}
// export type AppDispatch = typeof store.dispatch

