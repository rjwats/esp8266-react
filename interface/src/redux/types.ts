export type ActionModel = { 
    type: string,
    payload: any 
}
export type MessageModel = {
    type: string,
    id: string,
    origin_id: string,
    payload: any
};