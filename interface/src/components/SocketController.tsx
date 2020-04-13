import React from 'react';
import Sockette from 'sockette';
import throttle from 'lodash/throttle';
import { withSnackbar, WithSnackbarProps } from 'notistack';

import { addAccessTokenParameter } from '../authentication';
import { extractEventValue } from '.';

export interface SocketControllerProps<D> extends WithSnackbarProps {
  handleValueChange: (name: keyof D) => (event: React.ChangeEvent<HTMLInputElement>) => void;

  setData: (data: D, callback?: () => void) => void;
  saveData: () => void;
  saveDataAndClear(): () => void;

  connected: boolean;
  data?: D;
}

interface SocketControllerState<D> {
  ws: Sockette;
  connected: boolean;
  clientId?: string;
  data?: D;
}

enum SocketMessageType {
  ID = "id",
  PAYLOAD = "payload"
}

interface SocketIdMessage {
  type: typeof SocketMessageType.ID;
  id: string;
}

interface SocketPayloadMessage<D> {
  type: typeof SocketMessageType.PAYLOAD;
  origin_id: string;
  payload: D;
}

export type SocketMessage<D> = SocketIdMessage | SocketPayloadMessage<D>;

export function socketController<D, P extends SocketControllerProps<D>>(wsUrl: string, wsThrottle: number, SocketController: React.ComponentType<P & SocketControllerProps<D>>) {
  return withSnackbar(
    class extends React.Component<Omit<P, keyof SocketControllerProps<D>> & WithSnackbarProps, SocketControllerState<D>> {
      constructor(props: Omit<P, keyof SocketControllerProps<D>> & WithSnackbarProps) {
        super(props);
        this.state = {
          ws: new Sockette(addAccessTokenParameter(wsUrl), {
            onmessage: this.onMessage,
            onopen: this.onOpen,
            onclose: this.onClose,
          }),
          connected: false
        }
      }

      componentWillUnmount() {
        this.state.ws.close();
      }

      onMessage = (event: MessageEvent) => {
        const rawData = event.data;
        if (typeof rawData === 'string' || rawData instanceof String) {
          this.handleMessage(JSON.parse(rawData as string) as SocketMessage<D>);
        }
      }

      handleMessage = (socketMessage: SocketMessage<D>) => {
        switch (socketMessage.type) {
          case SocketMessageType.ID:
            this.setState({ clientId: socketMessage.id });
            break;
          case SocketMessageType.PAYLOAD:
            const { clientId, data } = this.state;
            if (clientId && (!data || clientId !== socketMessage.origin_id)) {
              this.setState(
                { data: socketMessage.payload }
              );
            }
            break;
        }
      }

      onOpen = () => {
        this.setState({ connected: true });
      }

      onClose = () => {
        this.setState({ connected: false, clientId: undefined, data: undefined });
      }

      setData = (data: D, callback?: () => void) => {
        this.setState({ data }, callback);
      }

      saveData = throttle(() => {
        const { ws, connected, data } = this.state;
        if (connected) {
          ws.json(data);
        }
      }, wsThrottle);

      saveDataAndClear = throttle(() => {
        const { ws, connected, data } = this.state;
        if (connected) {
          this.setState({
            data: undefined
          }, () => ws.json(data));
        }
      }, wsThrottle);

      handleValueChange = (name: keyof D) => (event: React.ChangeEvent<HTMLInputElement>) => {
        const data = { ...this.state.data!, [name]: extractEventValue(event) };
        this.setState({ data });
      }

      render() {
        return <SocketController
          handleValueChange={this.handleValueChange}
          setData={this.setData}
          saveData={this.saveData}
          saveDataAndClear={this.saveDataAndClear}
          connected={this.state.connected}
          data={this.state.data}
          {...this.props as P}
        />;
      }

    });
}
